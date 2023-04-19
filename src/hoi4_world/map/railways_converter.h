#ifndef SRC_HOI4WORLD_MAP_RAILWAYS_CONVERTER_H
#define SRC_HOI4WORLD_MAP_RAILWAYS_CONVERTER_H



#include <map>
#include <vector>

#include "src/hoi4_world/map/railways.h"
#include "src/hoi4_world/states/hoi4_states.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/maps/map_data.h"
#include "src/vic3_world/states/state_region.h"



namespace hoi4
{

Railways ConvertRailways(const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const mappers::ProvinceMapper& province_mapper,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const States& hoi4_states);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_RAILWAYS_CONVERTER_H