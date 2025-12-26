#include "src/hoi4_world/world/hoi4_world_converter.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>

#include <ranges>

#include "hoi4_world.h"
#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/characters/hoi4_characters_converter.h"
#include "src/hoi4_world/countries/hoi4_countries_converter.h"
#include "src/hoi4_world/diplomacy/hoi4_war_converter.h"
#include "src/hoi4_world/focus_trees/focus_tree_assembler.h"
#include "src/hoi4_world/localizations/localizations_converter.h"
#include "src/hoi4_world/map/buildings_creator.h"
#include "src/hoi4_world/map/coastal_provinces_creator.h"
#include "src/hoi4_world/map/hoi4_province_definition_importer.h"
#include "src/hoi4_world/map/railways_converter.h"
#include "src/hoi4_world/map/resources_map_importer.h"
#include "src/hoi4_world/map/strategic_regions_importer.h"
#include "src/hoi4_world/roles/roles_importer.h"
#include "src/hoi4_world/roles/stories_creator.h"
#include "src/hoi4_world/states/default_states_importer.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"
#include "src/hoi4_world/world/hoi4_world_framework_builder.h"
#include "src/mappers/culture/culture_graphics_mapper_importer.h"
#include "src/mappers/technology/tech_mappings_importer.h"
#include "src/maps/map_data.h"
#include "src/maps/map_data_importer.h"
#include "src/support/progress_manager.h"
#include "src/vic3_world/wars/war.h"



namespace
{

std::map<std::string, vic3::ProvinceType> GatherVic3SignificantProvinces(
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions)
{
   std::map<std::string, vic3::ProvinceType> vic3_significant_provinces;

   for (const vic3::StateRegion& vic3_state_region: vic3_state_regions | std::views::values)
   {
      const std::map<vic3::ProvinceId, vic3::ProvinceType>& significant_provinces =
          vic3_state_region.GetSignificantProvinces();
      for (const auto& [province, type]: significant_provinces)
      {
         vic3_significant_provinces.emplace(province, type);
      }
   }

   return vic3_significant_provinces;
}


std::set<std::string> MapPowers(const std::set<int>& source_powers, const mappers::CountryMapper& country_mapper)
{
   std::set<std::string> powers;

   for (const int great_power_number: source_powers)
   {
      if (std::optional<std::string> hoi4_tag = country_mapper.GetHoiTag(great_power_number); hoi4_tag)
      {
         powers.insert(*hoi4_tag);
      }
   }

   return powers;
}


std::optional<int> GetCapitalStateNumber(int vic3_country_number,
    const mappers::CountryMapper& country_mapper,
    const std::map<std::string, hoi4::Country>& hoi4_countries,
    int num_states)
{
   const std::optional<std::string> possible_tag = country_mapper.GetHoiTag(vic3_country_number);
   if (!possible_tag)
   {
      return std::nullopt;
   }
   const auto& country_itr = hoi4_countries.find(*possible_tag);
   if (country_itr == hoi4_countries.end())
   {
      return std::nullopt;
   }

   const std::optional<int> possible_capital_state_num = country_itr->second.GetCapitalState();
   if (!possible_capital_state_num || num_states < *possible_capital_state_num)
   {
      return std::nullopt;
   }

   return possible_capital_state_num;
}


void MarkCapitalsAsCapitals(const std::map<std::string, hoi4::Country>& countries, std::vector<hoi4::State>& states)
{
   for (const hoi4::Country& country: countries | std::views::values)
   {
      const std::optional<int> possible_capital = country.GetCapitalState();
      if (!possible_capital)
      {
         continue;
      }
      if (*possible_capital - 1 > states.size())
      {
         continue;
      }

      hoi4::State& capital_state = states.at(*possible_capital - 1);
      capital_state.SetIsCapital(true);
   }
}


void IncreaseAirBasesInCapitals(const std::map<std::string, hoi4::Country>& countries, std::vector<hoi4::State>& states)
{
   for (const hoi4::Country& country: countries | std::views::values)
   {
      const std::optional<int> possible_capital = country.GetCapitalState();
      if (!possible_capital)
      {
         continue;
      }
      if (*possible_capital - 1 > states.size())
      {
         continue;
      }

      hoi4::State& capital_state = states.at(*possible_capital - 1);
      capital_state.IncreaseAirBaseLevel(5);
   }
}


void IncreaseVictoryPointsInCapitals(std::vector<hoi4::State>& states,
    const vic3::CountryRankings& country_rankings,
    const mappers::CountryMapper& country_mapper,
    const std::map<std::string, hoi4::Country>& countries)
{
   const int num_great_powers = static_cast<int>(country_rankings.GetGreatPowers().size());
   int num_handled_great_powers = 0;
   const int num_major_powers = static_cast<int>(country_rankings.GetMajorPowers().size());
   int num_handled_major_powers = 0;
   int num_handled_others = 0;
   for (int vic3_country_number: country_rankings.GetScoredCountries() | std::views::values)
   {
      std::optional<int> possible_capital_state_number =
          GetCapitalStateNumber(vic3_country_number, country_mapper, countries, static_cast<int>(states.size()));
      if (!possible_capital_state_number)
      {
         continue;
      }
      hoi4::State& capital_state = states[*possible_capital_state_number - 1];

      if (country_rankings.GetGreatPowers().contains(vic3_country_number))
      {
         if (num_handled_great_powers < num_great_powers / 2)
         {
            capital_state.SetHighestVictoryPointValue(50);
         }
         else
         {
            capital_state.SetHighestVictoryPointValue(40);
         }
         ++num_handled_great_powers;
      }
      else if (country_rankings.GetMajorPowers().contains(vic3_country_number))
      {
         if (num_handled_major_powers < num_major_powers / 2)
         {
            capital_state.SetHighestVictoryPointValue(30);
         }
         else
         {
            capital_state.SetHighestVictoryPointValue(25);
         }
         ++num_handled_major_powers;
      }
      else
      {
         if (num_handled_others < 30)
         {
            capital_state.SetHighestVictoryPointValue(20);
         }
         else if (num_handled_others < 60)
         {
            capital_state.SetHighestVictoryPointValue(15);
         }
         else
         {
            capital_state.SetHighestVictoryPointValue(10);
         }
         ++num_handled_others;
      }
   }
}


void LogVictoryPointData(const std::vector<hoi4::State>& states)
{
   std::map<int, int> victory_point_values;
   for (const hoi4::State& state: states)
   {
      for (int victory_point_value: state.GetVictoryPoints() | std::views::values)
      {
         const auto& [itr, success] = victory_point_values.emplace(victory_point_value, 1);
         if (!success)
         {
            itr->second++;
         }
      }
   }

   Log(LogLevel::Info) << "Victory point data:";
   for (const auto& [value, num_instances]: victory_point_values)
   {
      Log(LogLevel::Info) << fmt::format("\t{} victory points of value {}", num_instances, value);
   }
}


void ConvertWars(const std::vector<vic3::War>& source_wars,
    const mappers::CountryMapper& country_mapper,
    std::map<std::string, hoi4::Country>& countries)
{
   std::set<std::string> independent_countries;
   std::ranges::copy(countries | std::views::keys, std::inserter(independent_countries, independent_countries.begin()));

   for (const vic3::War& source_war: source_wars)
   {
      std::optional<hoi4::War> war = hoi4::ConvertWar(source_war, independent_countries, country_mapper);
      if (!war.has_value())
      {
         continue;
      }

      if (auto country_itr = countries.find(war->original_attacker); country_itr != countries.end())
      {
         country_itr->second.AddWar(*war);
      }
   }
}

}  // namespace

hoi4::World hoi4::ConvertWorld(const commonItems::ModFilesystem& hoi4_mod_filesystem,
    const vic3::World& source_world,
    const mappers::WorldMapper& world_mapper,
    std::future<hoi4::WorldFramework> world_framework_future,
    const configuration::Configuration& config)
{
   Log(LogLevel::Info) << "Creating Hoi4 world";

   std::map<std::string, vic3::ProvinceType> vic3_significant_provinces =
       GatherVic3SignificantProvinces(source_world.GetStateRegions());
   hoi4::WorldFramework world_framework = world_framework_future.get();

   ProgressManager::SetProgress(50);
   Log(LogLevel::Info) << "\tConverting states";

   hoi4::States states = ConvertStates(source_world,
       world_mapper,
       world_framework,
       vic3_significant_provinces,
       world_framework.map_data,
       config);

   world_framework.strategic_regions.UpdateToMatchNewStates(states.states, world_framework.map_data);


   std::future<hoi4::Buildings> buildings_future =
       std::async(std::launch::async, [&states, &world_framework, &hoi4_mod_filesystem]() {
          auto result =
              ImportBuildings(states, world_framework.coastal_provinces, world_framework.map_data, hoi4_mod_filesystem);
          ProgressManager::AddProgress(5);
          return result;
       });

   std::future<hoi4::Railways> railways_future =
       std::async(std::launch::async, [&vic3_significant_provinces, &world_mapper, &world_framework, &states]() {
          // convertRailways logs progress internally
          return ConvertRailways(vic3_significant_provinces,
              world_mapper.province_mapper,
              world_framework.map_data,
              world_framework.province_definitions,
              states);
       });

   Log(LogLevel::Info) << "\tConverting countries";
   ProgressManager::AddProgress(10);

   std::map<int, hoi4::Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   std::map<std::string, hoi4::Country> countries = ConvertCountries(source_world,
       world_mapper,
       source_world.GetLocalizations(),
       states,
       characters,
       culture_queues,
       config.debug);

   Log(LogLevel::Info) << "\tAssigning portraits to characters";
   ProgressManager::AddProgress(5);
   AssignPortraits(culture_queues,
       world_mapper.culture_graphics_mapper,
       source_world.GetCultureDefinitions(),
       source_world.GetPlaythroughId(),
       characters);

   std::set<std::string> great_powers =
       MapPowers(source_world.GetCountryRankings().GetGreatPowers(), world_mapper.country_mapper);
   std::set<std::string> major_powers =
       MapPowers(source_world.GetCountryRankings().GetMajorPowers(), world_mapper.country_mapper);
   MarkCapitalsAsCapitals(countries, states.states);
   IncreaseAirBasesInCapitals(countries, states.states);
   IncreaseVictoryPointsInCapitals(states.states,
       source_world.GetCountryRankings(),
       world_mapper.country_mapper,
       countries);
   LogVictoryPointData(states.states);
   ProgressManager::AddProgress(5);

   ConvertWars(source_world.GetWars(), world_mapper.country_mapper, countries);
   ProgressManager::AddProgress(1);

   hoi4::Localizations localizations = ConvertLocalizations(source_world.GetLocalizations(),
       world_mapper.country_mapper.GetCountryMappings(),
       states.hoi4_state_names_to_vic3_state_names,
       source_world.GetStateRegions(),
       world_mapper.province_mapper,
       source_world.GetCountries(),
       source_world.GetCharacters(),
       countries,
       characters);

   hoi4::Railways railways = railways_future.get();
   hoi4::Buildings buildings = buildings_future.get();
   ProgressManager::AddProgress(5);

   hoi4::World world(hoi4::WorldOptions{
       .countries = countries,
       .great_powers = great_powers,
       .major_powers = major_powers,
       .states = states,
       .strategic_regions = world_framework.strategic_regions,
       .buildings = buildings,
       .railways = railways,
       .localizations = localizations,
       .characters = characters,
       .culture_definitions = source_world.GetCultureDefinitions(),
   });

   std::set<DecisionsCategory> decisions_categories;
   std::map<std::string, std::vector<Decision>> decisions_in_categories;
   std::map<std::string, std::vector<Event>> country_events;

   const std::map<std::string, Role> roles = ImportRoles();
   std::map<std::string, Country>& modifiable_countries = world.GetModifiableCountries();

   std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories(roles, world, modifiable_countries);

   // Create tag aliases
   std::map<std::string, std::string> aliases;

   char alias_char = 'A';
   int alias_number = 0;
   for (auto& [tag, country_roles]: stories)
   {
      for (const Role& country_role: country_roles)
      {
         for (const auto& alias_definition: country_role.GetAliasDefinitions())
         {
            std::string alias_tag = fmt::format("{}{:0>2}", alias_char, alias_number);
            alias_number++;
            if (alias_number > 99)
            {
               alias_number = 0;
               alias_char++;
            }

            std::string flag = alias_definition.country_flag;
            while (flag.find("$TAG$") != std::string::npos)
            {
               flag.replace(flag.find("$TAG$"), 5, tag);
            }

            world.AddTagAlias(TagAlias{
                .alias = alias_tag,
                .original_tag = tag,
                .flag = flag,
            });

            std::string identifier = alias_definition.alias_identifier;
            while (identifier.find("$TAG$") != std::string::npos)
            {
               identifier.replace(identifier.find("$TAG$"), 5, tag);
            }
            aliases.emplace(identifier, alias_tag);
         }
      }
   }

   // Apply stories
   for (const auto& [tag, country_roles]: stories)
   {
      auto country_itr = modifiable_countries.find(tag);
      if (country_itr == modifiable_countries.end())
      {
         Log(LogLevel::Warning) << fmt::format("Country {} in story could not be found.", tag);
         continue;
      }

      std::set<std::string> role_names;
      std::ranges::for_each(country_roles, [&role_names](const Role& role) {
         role_names.insert(role.GetName());
      });
      country_itr->second.SetRoles(role_names);

      for (const Role& country_role: country_roles)
      {
         for (const DecisionsCategory& role_category: country_role.GetDecisionsCategories())
         {
            decisions_categories.insert(role_category);
         }
         for (const auto& [category, decisions]: country_role.GetDecisionsInCategories())
         {
            decisions_in_categories.emplace(category, decisions);
         }
         for (const Event& event: country_role.GetEvents())
         {
            if (auto [itr, success] = country_events.emplace(tag, std::vector{event}); !success)
            {
               itr->second.push_back(event);
            }
         }
      }

      const FocusTree tree = AssembleTree(country_roles, tag, aliases, world);
      country_itr->second.SetFocusTree(tree);
   }

   world.SetDecisionsCategories(decisions_categories);
   world.SetDecisions(decisions_in_categories);
   world.SetCountryEvents(country_events);

   return world;
}
