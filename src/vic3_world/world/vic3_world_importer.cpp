#include "src/vic3_world/world/vic3_world_importer.h"

#include <filesystem>
#include <fstream>
#include <numeric>
#include <ranges>
#include <sstream>

#include "external/commonItems/Color.h"
#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Date.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/ModLoader/Mod.h"
#include "external/commonItems/ModLoader/ModLoader.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "external/rakaly/rakaly.h"
#include "src/support/date_fmt.h"
#include "src/support/progress_manager.h"
#include "src/vic3_world/buildings/buildings_importer.h"
#include "src/vic3_world/characters/vic3_character_manager.h"
#include "src/vic3_world/countries/country_definitions_importer.h"
#include "src/vic3_world/countries/vic3_countries_importer.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/country_rankings/country_rankings.h"
#include "src/vic3_world/country_rankings/country_rankings_importer.h"
#include "src/vic3_world/cultures/culture_definitions_importer.h"
#include "src/vic3_world/cultures/cultures_importer.h"
#include "src/vic3_world/elections/elections_importer.h"
#include "src/vic3_world/ideologies/ideologies_importer.h"
#include "src/vic3_world/institutions/institutions_importer.h"
#include "src/vic3_world/interest_groups/interest_groups_importer.h"
#include "src/vic3_world/laws/laws_importer.h"
#include "src/vic3_world/military/military_formations_importer.h"
#include "src/vic3_world/pacts/pacts_importer.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"
#include "src/vic3_world/states/state_regions_importer.h"
#include "src/vic3_world/states/vic3_state.h"
#include "src/vic3_world/states/vic3_states_importer.h"
#include "src/vic3_world/technology/vic3_technology_importer.h"
#include "src/vic3_world/wars/wars_importer.h"


namespace
{

std::string ReadSave(std::string_view save_filename)
{
   std::ifstream save_file(std::filesystem::u8path(save_filename), std::ios::in | std::ios::binary);
   const auto save_size = static_cast<std::streamsize>(std::filesystem::file_size(save_filename));
   std::string save_string(save_size, '\0');
   save_file.read(save_string.data(), save_size);

   return save_string;
}


std::istringstream GetSaveMeta(const rakaly::GameFile& save, const std::string& save_string)
{
   std::string save_meta;

   if (save.is_binary())
   {
      const auto melt = save.meltMeta();
      if (!melt || melt->has_unknown_tokens())
      {
         throw std::runtime_error("Unable to melt ironman save's metadata");
      }

      melt->writeData(save_meta);
   }
   else
   {
      save_meta = save_string;
   }

   return std::istringstream{save_meta};
}


std::vector<Mod> GetModsFromSave(const std::vector<std::string>& mod_names)
{
   std::vector<Mod> mods;
   for (const auto& mod_name: mod_names)
   {
      Mod mod(mod_name, "");
      mods.push_back(mod);
   }

   return mods;
}


std::istringstream MeltSave(const rakaly::GameFile& save, const std::string& save_string)
{
   const auto melt = save.melt();
   if (melt.has_unknown_tokens())
   {
      Log(LogLevel::Warning) << "Save had unmeltable tokens.";
   }

   std::string melted_save_string;
   melt.writeData(melted_save_string);

   if (melted_save_string.empty())
   {
      return std::istringstream{save_string};
   }
   return std::istringstream{melted_save_string};
}


void AssignCulturesToCountries(std::map<int, vic3::Country>& countries, const std::map<int, std::string>& cultures)
{
   for (auto& [country_id, country]: countries)
   {
      for (const auto& id: country.GetPrimaryCultureIds())
      {
         if (const auto culture_itr = cultures.find(id); culture_itr != cultures.end())
         {
            country.AddPrimaryCulture(culture_itr->second);
         }
         else
         {
            Log(LogLevel::Warning) << fmt::format("Country: {} could not find a match for Culture: {}.",
                country_id,
                id);
         }
      }
   }
}

void AssignCulturesToCharacters(std::map<int, vic3::Character>& characters, const std::map<int, std::string>& cultures)
{
   for (auto& character: characters | std::ranges::views::values)
   {
      if (const auto culture_itr = cultures.find(character.GetCultureId()); culture_itr != cultures.end())
      {
         character.SetCulture(culture_itr->second);
      }
      else
      {
         Log(LogLevel::Warning) << fmt::format("Character {} {} could not find a match for Culture: {}.",
             character.GetFirstName(),
             character.GetLastName(),
             character.GetCultureId());
      }
   }
}


void AssignOwnersToStates(const std::map<int, vic3::Country>& countries, std::map<int, vic3::State>& states)
{
   for (auto& [state_number, state]: states)
   {
      const auto& possible_owner_number = state.GetOwnerNumber();
      if (!possible_owner_number.has_value())
      {
         continue;
      }

      if (const auto country_itr = countries.find(*possible_owner_number); country_itr != countries.end())
      {
         state.SetOwnerTag(country_itr->second.GetTag());
      }
      else
      {
         Log(LogLevel::Warning) << fmt::format("State {} had an owner with no definition.", state_number);
      }
   }
}

void AssignIgsToCountries(std::map<int, vic3::Country>& countries, const std::map<int, vic3::InterestGroup>& igs)
{
   for (const auto& [ig_id, ig]: igs)
   {
      if (const auto country_itr = countries.find(ig.GetCountryId()); country_itr != countries.end())
      {
         country_itr->second.AddInterestGroupId(ig_id);
      }
      else
      {
         Log(LogLevel::Warning) << fmt::format("Country: {} not found. Ignoring {} with ID: {}.",
             ig.GetCountryId(),
             ig.GetType(),
             ig_id);
      }
   }
}


void AssignCharactersToCountries(const std::map<int, vic3::Character>& characters,
    const std::map<int, std::vector<int>>& country_character_map,
    std::map<int, vic3::Country>& countries)
{
   for (const auto& [country_id, character_ids]: country_character_map)
   {
      if (const auto country_itr = countries.find(country_id); country_itr != countries.end())
      {
         std::vector<int> important_character_ids;
         std::ranges::copy_if(character_ids, std::back_inserter(important_character_ids), [characters](const int id) {
            if (const auto& character_itr = characters.find(id); character_itr != characters.end())
            {
               const auto& roles = character_itr->second.GetRoles();
               if (roles.size() > 1 || (!roles.contains("general") && !roles.contains("admiral")))
               {
                  return true;
               }
               return character_itr->second.IsCommander();  // Only employed officers please
            }
            return false;
         });
         country_itr->second.SetCharacterIds(important_character_ids);
      }
      else
      {
         Log(LogLevel::Warning) << fmt::format("Country: {} not found. can't place character ids", country_id);
      }
   }
}


void AssignMilitaryFormationsToCountries(const std::map<int, vic3::MilitaryFormation>& military_formations,
    std::map<int, vic3::Country>& countries)
{
   std::map<int, std::map<int, vic3::MilitaryFormation>> army_formations_by_country;
   std::map<int, std::map<int, vic3::MilitaryFormation>> navy_formations_by_country;
   for (const auto& [formation_number, formation]: military_formations)
   {
      if (formation.type == vic3::MilitaryFormationType::kArmy)
      {
         auto [iterator, success] = army_formations_by_country.emplace(formation.country,
             std::map<int, vic3::MilitaryFormation>{{formation_number, formation}});
         if (!success)
         {
            iterator->second.emplace(formation_number, formation);
         }
      }
      else
      {
         auto [iterator, success] = navy_formations_by_country.emplace(formation.country,
             std::map<int, vic3::MilitaryFormation>{{formation_number, formation}});
         if (!success)
         {
            iterator->second.emplace(formation_number, formation);
         }
      }
   }

   for (const auto& [country_number, army_formations]: army_formations_by_country)
   {
      auto country = countries.find(country_number);
      if (country == countries.end())
      {
         Log(LogLevel::Warning) << fmt::format("Could not find country {} to assign army formations.", country_number);
         continue;
      }
      country->second.SetArmyFormations(army_formations);
   }
   for (const auto& [country_number, navy_formations]: navy_formations_by_country)
   {
      auto country = countries.find(country_number);
      if (country == countries.end())
      {
         Log(LogLevel::Warning) << fmt::format("Could not find country {} to assign navy formations.", country_number);
         continue;
      }
      country->second.SetNavyFormations(navy_formations);
   }
}


void ApplySubjectRelationships(const std::map<int, vic3::Pact>& pacts, std::map<int, vic3::Country>& countries)
{
   for (const vic3::Pact& pact: pacts | std::views::values)
   {
      if (pact.isSubjectRelationship())
      {
         auto overlord = countries.find(pact.GetFirstId());
         auto subject = countries.find(pact.GetSecondId());
         if (overlord != countries.end() && subject != countries.end())
         {
            overlord->second.AddPuppet(pact.GetSecondId());
            subject->second.AddOverlord(pact.GetFirstId());
            if (subject->second.GetColor() == commonItems::Color{})
            {
               subject->second.SetColor(overlord->second.GetColor());
            }
         }
      }
   }
}


std::map<std::string, int> MapCountryTagsToId(std::map<int, vic3::Country>& countries)
{
   std::map<std::string, int> tag_to_id_map;
   for (const auto& [id, country]: countries)
   {
      tag_to_id_map.emplace(country.GetTag(), id);
   }
   return tag_to_id_map;
}

void AssignHomeCountriesToExiledAgitators(const std::map<std::string, int>& tag_to_id_map,
    std::map<int, vic3::Character>& characters)
{
   for (auto& character: characters | std::views::values)
   {
      if (character.GetOriginTag().empty())
      {
         continue;
      }

      if (const auto& id_itr = tag_to_id_map.find(character.GetOriginTag()); id_itr != tag_to_id_map.end())
      {
         character.SetOriginCountryId(id_itr->second);
      }
   }
}

int MungePlaythroughIdIntoInteger(const std::string& playthrough_id_string)
{
   return std::accumulate(playthrough_id_string.begin(), playthrough_id_string.end(), 0, [](int id, const char& digit) {
      return id + static_cast<int>(digit);
   });
}
void CheckProvinceTerrainsGrouping(const std::vector<std::string>& all_provinces,
    const std::map<std::string, vic3::StateRegion>& state_regions)
{
   for (const auto& [region_name, region]: state_regions)
   {
      auto provinces_in_state = region.GetProvinces();
      std::vector<int> provinces_indexes;
      for (const auto& province: provinces_in_state)
      {
         auto it = std::find(all_provinces.begin(), all_provinces.end(), province);
         if (it != all_provinces.end())
         {
            int index = std::distance(all_provinces.begin(), it);
            provinces_indexes.push_back(index);
         }
      }
      std::sort(provinces_indexes.begin(), provinces_indexes.end());
      for (size_t i = 1; i < provinces_indexes.size(); ++i)
      {
         if (provinces_indexes[i] - provinces_indexes[i - 1] != 1)
         {
            std::string province_warning;
            for (size_t j = 0; j < provinces_indexes.size(); ++j)
            {
               if (j != 0)
               {
                  province_warning += ", ";
               }
               province_warning +=
                   fmt::format("{} (index: {})", all_provinces[provinces_indexes[j]], provinces_indexes[j]);
            }
            Log(LogLevel::Debug) << fmt::format(
                "Provinces: [{}] are not next to each other in province_terrain.txt but are in the same state: {}\n",
                province_warning,
                region_name);
            break;
         }
      }
   }
}
}  // namespace


vic3::World vic3::ImportWorld(const configuration::Configuration& configuration,
    const commonItems::ConverterVersion& converter_version)
{
   WorldOptions world_options;
   Log(LogLevel::Info) << "*** Hello Vic3, loading World. ***";
   std::string save_string = ReadSave(configuration.save_game);
   const rakaly::GameFile save = rakaly::parseVic3(save_string);

   std::istringstream meta_stream = GetSaveMeta(save, save_string);

   Log(LogLevel::Info) << "-> Reading Vic3 save metadata.";
   std::vector<std::string> mod_names;

   commonItems::parser meta_parser;
   meta_parser.registerKeyword("game_date", [&mod_names](std::istream& input_stream) {
      date game_date(commonItems::getString(input_stream));
      Log(LogLevel::Info) << fmt::format("Converting at {}.", game_date);
   });
   meta_parser.registerKeyword("mods", [&mod_names](std::istream& input_stream) {
      mod_names = commonItems::stringList(input_stream).getStrings();
   });
   meta_parser.registerKeyword("version", [&converter_version](std::istream& input_stream) {
      const auto str_version = commonItems::getString(input_stream);
      GameVersion version = GameVersion(str_version);
      Log(LogLevel::Info) << "Savegame version: " << version;

      if (converter_version.getMinSource() > version)
      {
         Log(LogLevel::Error) << "Converter requires a minimum save from v"
                              << converter_version.getMinSource().toShortString();
         throw std::runtime_error("Savegame vs converter version mismatch!");
      }
      if (!converter_version.getMaxSource().isLargerishThan(version))
      {
         Log(LogLevel::Error) << "Converter requires a maximum save from v"
                              << converter_version.getMaxSource().toShortString();
         throw std::runtime_error("Savegame vs converter version mismatch!");
      }
   });
   meta_parser.IgnoreUnregisteredItems();
   meta_parser.parseStream(meta_stream);

   Log(LogLevel::Info) << "-> Loading Vic3 mods.";
   commonItems::ModLoader mod_loader;
   mod_loader.loadMods(std::vector<std::string>{configuration.vic3_mod_path, configuration.vic3_steam_mod_path},
       GetModsFromSave(mod_names));

   Log(LogLevel::Info) << "-> Reading Vic3 install.";
   commonItems::ModFilesystem mod_filesystem(fmt::format("{}/game", configuration.vic3_directory),
       mod_loader.getMods());
   world_options.province_definitions = LoadProvinceDefinitions();
   world_options.state_regions = ImportStateRegions(mod_filesystem);
   CheckProvinceTerrainsGrouping(world_options.province_definitions.GetProvinceDefinitions(),
       world_options.state_regions);
   commonItems::LocalizationDatabase localizations("english",
       {"braz_por",
           "french",
           "german",
           "japanese",
           "korean",
           "polish",
           "russian",
           "simp_chinese",
           "spanish",
           "turkish"});
   localizations.ScrapeLocalizations(mod_filesystem, "localization");
   world_options.localizations = localizations;
   world_options.culture_definitions = ImportCultureDefinitions(mod_filesystem);
   ProgressManager::AddProgress(5);

   Log(LogLevel::Info) << "-> Reading Vic3 save.";
   std::istringstream save_stream = MeltSave(save, save_string);
   ProgressManager::AddProgress(1);
   Log(LogLevel::Info) << "-> Processing Vic3 save.";
   const std::map<std::string, commonItems::Color> color_definitions = ImportCountryColorDefinitions(mod_filesystem);
   std::map<int, std::string> cultures;
   std::map<int, std::vector<int>> country_character_map;
   std::map<int, MilitaryFormation> military_formations;

   commonItems::parser save_parser;
   save_parser.registerKeyword("playthrough_id", [&world_options](std::istream& input_stream) {
      world_options.playthrough_id = MungePlaythroughIdIntoInteger(commonItems::getString(input_stream));
   });
   save_parser.registerKeyword("country_manager", [&world_options, color_definitions](std::istream& input_stream) {
      world_options.countries = ImportCountries(color_definitions, input_stream);
   });
   save_parser.registerKeyword("states", [&world_options](std::istream& input_stream) {
      world_options.states = ImportStates(input_stream);
   });
   save_parser.registerKeyword("technology", [&world_options](std::istream& input_stream) {
      world_options.acquired_technologies = ImportAcquiredTechnologies(input_stream);
   });
   save_parser.registerKeyword("country_rankings", [&world_options](std::istream& input_stream) {
      world_options.country_rankings = ImportCountryRankings(input_stream);
   });
   save_parser.registerKeyword("laws", [&world_options](std::istream& input_stream) {
      for (const auto& [country_number, active_laws]: ImportLaws(input_stream))
      {
         auto country_itr = world_options.countries.find(country_number);
         if (country_itr == world_options.countries.end())
         {
            continue;
         }
         country_itr->second.SetActiveLaws(active_laws);
      }
   });

   save_parser.registerKeyword("institutions", InstitutionsImporter(world_options.institutions));
   save_parser.registerKeyword("cultures", [&cultures](std::istream& input_stream) {
      cultures = ImportCultures(input_stream);
   });
   save_parser.registerKeyword("character_manager",
       [&world_options, &country_character_map](std::istream& input_stream) {
          const CharacterManager character_manager(input_stream);
          world_options.characters = character_manager.GetCharacters();
          country_character_map = character_manager.GetCountryCharacterMap();
       });
   save_parser.registerKeyword("interest_groups", [&world_options](std::istream& input_stream) {
      world_options.igs = ImportInterestGroups(input_stream);
   });
   save_parser.registerKeyword("building_manager", [&world_options](std::istream& input_stream) {
      world_options.buildings = ImportBuildings(input_stream);
   });
   save_parser.registerKeyword("military_formation_manager", [&military_formations](std::istream& input_stream) {
      military_formations = ImportMilitaryFormations(input_stream);
   });
   save_parser.registerKeyword("election_manager", [&world_options](std::istream& input_stream) {
      for (const auto& [country_number, last_election]: ImportElections(input_stream))
      {
         auto country_itr = world_options.countries.find(country_number);
         if (country_itr == world_options.countries.end())
         {
            continue;
         }
         country_itr->second.SetLastElection(last_election);
      }
   });
   save_parser.registerKeyword("pacts", [&world_options](std::istream& input_stream) {
      world_options.pacts = ImportPacts(input_stream);
   });
   save_parser.registerKeyword("diplomatic_plays", [&world_options](std::istream& input_stream) {
      world_options.wars = ImportWars(input_stream);
   });
   save_parser.registerRegex("SAV.*", [](const std::string& unused, std::istream& input_stream) {
   });
   save_parser.IgnoreUnregisteredItems();

   save_parser.parseStream(save_stream);
   Log(LogLevel::Info) << fmt::format("\t{} countries imported", world_options.countries.size());
   Log(LogLevel::Info) << fmt::format("\t{} states imported", world_options.states.size());
   Log(LogLevel::Info) << fmt::format("\t{} countries acquired technologies",
       world_options.acquired_technologies.size());
   Log(LogLevel::Info) << fmt::format("\t{} in goods being sold",
       world_options.buildings.GetTotalGoodSalesValueInWorld());
   ProgressManager::AddProgress(10);

   AssignCulturesToCountries(world_options.countries, cultures);
   ProgressManager::AddProgress(1);
   AssignCulturesToCharacters(world_options.characters, cultures);
   ProgressManager::AddProgress(1);
   AssignOwnersToStates(world_options.countries, world_options.states);
   ProgressManager::AddProgress(1);
   const auto& country_tag_to_id_map = MapCountryTagsToId(world_options.countries);
   AssignHomeCountriesToExiledAgitators(country_tag_to_id_map, world_options.characters);
   ProgressManager::AddProgress(1);
   AssignIgsToCountries(world_options.countries, world_options.igs);
   ProgressManager::AddProgress(1);
   AssignCharactersToCountries(world_options.characters, country_character_map, world_options.countries);
   ProgressManager::AddProgress(1);
   AssignMilitaryFormationsToCountries(military_formations, world_options.countries);
   ProgressManager::AddProgress(1);
   ApplySubjectRelationships(world_options.pacts, world_options.countries);
   ProgressManager::AddProgress(1);
   vic3::IdeologiesImporter ideologies_importer;
   world_options.ideologies = ideologies_importer.ImportIdeologies(mod_filesystem);
   ProgressManager::AddProgress(1);
   return World(world_options);
}