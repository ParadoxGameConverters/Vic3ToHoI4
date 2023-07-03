#ifndef SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H
#define SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H



#include <map>

#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/hoi4_world/map/resources_map.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/default_state.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/states/hoi4_states.h"
#include "src/hoi4_world/states/state_categories.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/maps/map_data.h"
#include "src/vic3_world/buildings/buildings.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/states/state_region.h"
#include "src/vic3_world/states/vic3_state.h"



namespace hoi4
{

States ConvertStates(const std::map<int, vic3::State>& states,
    const vic3::ProvinceDefinitions& vic3_province_definitions,
    const std::map<std::string, vic3::ProvinceType>& significant_vic3_provinces,
    const vic3::Buildings& vic3_buildings,
    const mappers::ProvinceMapper& province_mapper,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const hoi4::StrategicRegions& strategic_regions,
    const mappers::CountryMapper& country_mapper,
    const hoi4::StateCategories& state_categories,
    const std::map<int, DefaultState>& default_states,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const CoastalProvinces& coastal_provinces,
    const ResourcesMap& resources_map,
    bool debug);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H