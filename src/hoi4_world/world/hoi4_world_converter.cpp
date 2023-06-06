#include "src/hoi4_world/world/hoi4_world_converter.h"

#include <ranges>

#include "external/commonItems/Log.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/countries/hoi4_countries_converter.h"
#include "src/hoi4_world/localizations/localizations_converter.h"
#include "src/hoi4_world/map/buildings_creator.h"
#include "src/hoi4_world/map/coastal_provinces_creator.h"
#include "src/hoi4_world/map/hoi4_province_definition_importer.h"
#include "src/hoi4_world/map/railways_converter.h"
#include "src/hoi4_world/map/resources_map_importer.h"
#include "src/hoi4_world/map/strategic_regions_importer.h"
#include "src/hoi4_world/states/default_states_importer.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"
#include "src/mappers/technology/tech_mappings_importer.h"
#include "src/maps/map_data.h"
#include "src/maps/map_data_importer.h"



namespace
{

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
          GetCapitalStateNumber(vic3_country_number, country_mapper, countries, states.size());
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

}  // namespace



hoi4::World hoi4::ConvertWorld(const commonItems::ModFilesystem& hoi4_mod_filesystem,
    const vic3::World& source_world,
    const mappers::CountryMapper& country_mapper,
    const mappers::ProvinceMapper& province_mapper,
    bool debug)
{
   std::map<std::string, Country> countries;

   Log(LogLevel::Info) << "Creating Hoi4 world";
   Log(LogLevel::Progress) << "50%";

   StrategicRegions strategic_regions = ImportStrategicRegions(hoi4_mod_filesystem);

   Log(LogLevel::Info) << "\tConverting states";
   const auto province_definitions = ImportProvinceDefinitions(hoi4_mod_filesystem);
   const maps::MapData map_data = maps::MapDataImporter(province_definitions).ImportMapData(hoi4_mod_filesystem);
   const std::map<int, DefaultState> default_states = hoi4::ImportDefaultStates(hoi4_mod_filesystem);
   CoastalProvinces coastal_provinces = CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());
   ResourcesMap resources_map = ImportResources("configurables/resources.txt");

   States states = ConvertStates(source_world.GetStates(),
       source_world.GetProvinceDefinitions(),
       source_world.GetBuildings(),
       province_mapper,
       map_data,
       province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories({
           {1, "pastoral"},
           {2, "rural"},
           {4, "town"},
           {5, "large_town"},
           {6, "city"},
           {8, "large_city"},
           {10, "metropolis"},
           {12, "megalopolis"},
       }),
       default_states,
       source_world.GetStateRegions(),
       coastal_provinces,
       resources_map,
       debug);

   strategic_regions.UpdateToMatchNewStates(states.states);

   Buildings buildings = ImportBuildings(states, coastal_provinces, map_data, hoi4_mod_filesystem);

   Railways railways =
       ConvertRailways(source_world.GetStateRegions(), province_mapper, map_data, province_definitions, states);

   Log(LogLevel::Info) << "\tConverting countries";
   Log(LogLevel::Progress) << "55%";

   const std::vector<mappers::TechMapping> tech_mappings = mappers::ImportTechMappings();

   const std::map<int, vic3::Country>& source_countries = source_world.GetCountries();
   countries = ConvertCountries(source_countries,
       source_world.GetAcquiredTechnologies(),
       country_mapper,
       states.vic3_state_ids_to_hoi4_state_ids,
       states.states,
       tech_mappings);

   std::set<std::string> great_powers = MapPowers(source_world.GetCountryRankings().GetGreatPowers(), country_mapper);
   std::set<std::string> major_powers = MapPowers(source_world.GetCountryRankings().GetMajorPowers(), country_mapper);
   IncreaseVictoryPointsInCapitals(states.states, source_world.GetCountryRankings(), country_mapper, countries);
   LogVictoryPointData(states.states);

   Localizations localizations = ConvertLocalizations(source_world.GetLocalizations(),
       country_mapper.GetCountryMappings(),
       states.hoi4_state_names_to_vic3_state_names,
       source_world.GetStateRegions(),
       province_mapper,
       source_world.GetCountries());

   return World(WorldOptions{.countries = countries,
       .great_powers = great_powers,
       .major_powers = major_powers,
       .states = states,
       .strategic_regions = strategic_regions,
       .buildings = buildings,
       .railways = railways,
       .localizations = localizations});
}