#include "src/hoi4_world/world/hoi4_world_converter.h"

#include <ranges>

#include "external/commonItems/Log.h"
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