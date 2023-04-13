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



hoi4::World hoi4::ConvertWorld(const commonItems::ModFilesystem& hoi4_mod_filesystem,
    const vic3::World& source_world,
    const mappers::CountryMapper& country_mapper,
    const mappers::ProvinceMapper& province_mapper)
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
       province_mapper.GetHoi4ToVic3ProvinceMappings(),
       map_data,
       province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories({{6, "city"},
           {0, "enclave"},
           {8, "large_city"},
           {5, "large_town"},
           {12, "megalopolis"},
           {10, "metropolis"},
           {1, "pastoral"},
           {2, "rural"},
           {4, "town"}}),
       default_states,
       source_world.GetStateRegions(),
       coastal_provinces,
       resources_map);

   strategic_regions.UpdateToMatchNewStates(states.states);

   Buildings buildings = ImportBuildings(states, coastal_provinces, map_data, hoi4_mod_filesystem);

   Railways railways = ConvertRailways(source_world.GetStateRegions(), province_mapper, map_data, province_definitions);

   Log(LogLevel::Info) << "\tConverting countries";
   Log(LogLevel::Progress) << "55%";

   const std::vector<mappers::TechMapping> tech_mappings = mappers::ImportTechMappings();

   countries = ConvertCountries(source_world.GetCountries(),
       source_world.GetAcquiredTechnologies(),
       country_mapper,
       states.vic3_state_ids_to_hoi4_state_ids,
       states.states,
       tech_mappings);

   Localizations localizations = ConvertLocalizations(source_world.GetLocalizations(),
       country_mapper.GetCountryMappings(),
       states.hoi4_state_names_to_vic3_state_names,
       source_world.GetStateRegions(),
       province_mapper,
       source_world.GetCountries());

   return World(WorldOptions{.countries = countries,
       .states = states,
       .strategic_regions = strategic_regions,
       .buildings = buildings,
       .railways = railways,
       .localizations = localizations});
}