#ifndef SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H
#define SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H


#include <map>

#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/hoi4_world/states/hoi4_states.h"
#include "src/hoi4_world/world/hoi4_world_framework.h"
#include "src/mappers/world/world_mapper.h"
#include "src/maps/map_data.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/world/vic3_world.h"

namespace hoi4
{

std::map<std::string, int> MapVic3ProvincesToStates(const std::map<int, vic3::State>& states,
    const vic3::ProvinceDefinitions& vic3_province_definitions);


States ConvertStates(const vic3::World& source_world,
    const mappers::WorldMapper& world_mapper,
    const hoi4::WorldFramework& world_framework,
    const std::map<std::string, vic3::ProvinceType>& significant_vic3_provinces,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const CoastalProvinces& coastal_provinces,
    bool debug = false);

}  // namespace hoi4

#endif  // SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H