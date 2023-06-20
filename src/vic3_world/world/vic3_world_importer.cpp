#include "src/vic3_world/world/vic3_world_importer.h"

#include <filesystem>
#include <fstream>
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
#include "src/vic3_world/buildings/buildings_importer.h"
#include "src/vic3_world/characters/character_manager.h"
#include "src/vic3_world/countries/country_definitions_importer.h"
#include "src/vic3_world/countries/vic3_countries_importer.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/country_rankings/country_rankings.h"
#include "src/vic3_world/country_rankings/country_rankings_importer.h"
#include "src/vic3_world/cultures/culture_definitions_importer.h"
#include "src/vic3_world/cultures/cultures_importer.h"
#include "src/vic3_world/elections/elections_importer.h"
#include "src/vic3_world/interest_groups/interest_groups_importer.h"
#include "src/vic3_world/laws/laws_importer.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"
#include "src/vic3_world/states/state_regions_importer.h"
#include "src/vic3_world/states/vic3_state.h"
#include "src/vic3_world/states/vic3_states_importer.h"
#include "src/vic3_world/technology/vic3_technology_importer.h"



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
   std::string melted_save_string;
   if (save.is_binary())
   {
      const auto melt = save.melt();
      if (melt.has_unknown_tokens())
      {
         throw std::runtime_error("Unable to melt ironman save");
      }

      melt.writeData(melted_save_string);
   }
   else
   {
      melted_save_string = save_string;
   }

   return std::istringstream{melted_save_string};
}


void AssignCulturesToCountries(std::map<int, vic3::Country>& countries, const std::map<int, std::string>& cultures)
{
   for (auto& country: countries | std::ranges::views::values)
   {
      for (const auto& id: country.GetPrimaryCultureIds())
      {
         if (const auto culture_itr = cultures.find(id); culture_itr != cultures.end())
         {
            country.AddPrimaryCulture(culture_itr->second);
         }
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

std::map<int, std::vector<int>> CreateCountryIGMap(const std::map<int, vic3::InterestGroup>& igs)
{
   std::map<int, std::vector<int>> country_ig_map;
   for (const auto& [ig_id, ig]: igs)
   {
      if (const auto country_id_itr = country_ig_map.find(ig.GetCountryId()); country_id_itr != country_ig_map.end())
      {
         country_id_itr->second.push_back(ig_id);
      }
      else
      {
         country_ig_map.emplace(country_id_itr->first, ig_id);
      }
   }
}

}  // namespace


vic3::World vic3::ImportWorld(const configuration::Configuration& configuration)
{
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
   meta_parser.IgnoreUnregisteredItems();
   meta_parser.parseStream(meta_stream);

   Log(LogLevel::Info) << "-> Loading Vic3 mods.";
   commonItems::ModLoader mod_loader;
   mod_loader.loadMods(std::vector<std::string>{configuration.vic3_mod_path, configuration.vic3_steam_mod_path},
       GetModsFromSave(mod_names));

   Log(LogLevel::Info) << "-> Reading Vic3 install.";
   commonItems::ModFilesystem mod_filesystem(fmt::format("{}/game", configuration.vic3_directory),
       mod_loader.getMods());
   const auto province_definitions = LoadProvinceDefinitions(mod_filesystem);
   const auto state_regions = ImportStateRegions(mod_filesystem);
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
   const auto culture_definitions = ImportCultureDefinitions(mod_filesystem);
   Log(LogLevel::Progress) << "5 %";

   Log(LogLevel::Info) << "-> Reading Vic3 save.";
   Log(LogLevel::Progress) << "7 %";
   std::istringstream save_stream = MeltSave(save, save_string);

   Log(LogLevel::Info) << "-> Processing Vic3 save.";
   const std::map<std::string, commonItems::Color> color_definitions = ImportCountryColorDefinitions(mod_filesystem);
   std::map<int, Country> countries;
   std::map<int, State> states;
   std::map<int, std::set<std::string>> acquired_technologies;
   Buildings buildings;
   CountryRankings country_rankings;
   std::map<int, std::string> cultures;
   std::map<int, Character> characters;
   std::map<int, std::vector<int>> country_character_map;
   std::map<int, InterestGroup> igs;

   commonItems::parser save_parser;
   save_parser.registerKeyword("country_manager", [&countries, color_definitions](std::istream& input_stream) {
      countries = ImportCountries(color_definitions, input_stream);
   });
   save_parser.registerKeyword("states", [&states](std::istream& input_stream) {
      states = ImportStates(input_stream);
   });
   save_parser.registerKeyword("technology", [&acquired_technologies](std::istream& input_stream) {
      acquired_technologies = ImportAcquiredTechnologies(input_stream);
   });
   save_parser.registerKeyword("country_rankings", [&country_rankings](std::istream& input_stream) {
      country_rankings = ImportCountryRankings(input_stream);
   });
   save_parser.registerKeyword("laws", [&countries](std::istream& input_stream) {
      for (const auto& [country_number, active_laws]: ImportLaws(input_stream))
      {
         auto country_itr = countries.find(country_number);
         if (country_itr == countries.end())
         {
            continue;
         }
         country_itr->second.SetActiveLaws(active_laws);
      }
   });
   save_parser.registerKeyword("cultures", [&cultures](std::istream& input_stream) {
      cultures = ImportCultures(input_stream);
   });
   save_parser.registerKeyword("character_manager", [&characters, &country_character_map](std::istream& input_stream) {
      const CharacterManager character_manager(input_stream);
      characters = character_manager.GetCharacters();
      country_character_map = character_manager.GetCountryCharacterMap();
   });
   save_parser.registerKeyword("interest_groups", [&igs](std::istream& input_stream) {
      igs = ImportInterestGroups(input_stream);
   });
   save_parser.registerKeyword("building_manager", [&buildings](std::istream& input_stream) {
      buildings = ImportBuildings(input_stream);
   });
   save_parser.registerKeyword("election_manager", [&countries](std::istream& input_stream) {
      for (const auto& [country_number, last_election]: ImportElections(input_stream))
      {
         auto country_itr = countries.find(country_number);
         if (country_itr == countries.end())
         {
            continue;
         }
         country_itr->second.SetLastElection(last_election);
      }
   });
   save_parser.registerRegex("SAV.*", [](const std::string& unused, std::istream& input_stream) {
   });
   save_parser.IgnoreUnregisteredItems();

   save_parser.parseStream(save_stream);
   Log(LogLevel::Info) << fmt::format("\t{} countries imported", countries.size());
   Log(LogLevel::Info) << fmt::format("\t{} states imported", states.size());
   Log(LogLevel::Info) << fmt::format("\t{} countries acquired technologies", acquired_technologies.size());
   Log(LogLevel::Info) << fmt::format("\t{} in goods being sold", buildings.GetTotalGoodSalesValueInWorld());
   Log(LogLevel::Progress) << "15 %";

   AssignCulturesToCountries(countries, cultures);
   AssignOwnersToStates(countries, states);
   const auto country_ig_map = CreateCountryIGMap(igs);

   return World({.countries = countries,
       .states = states,
       .state_regions = state_regions,
       .province_definitions = province_definitions,
       .acquired_technologies = acquired_technologies,
       .buildings = buildings,
       .country_rankings = country_rankings,
       .localizations = localizations,
       .culture_definitions = culture_definitions});
}