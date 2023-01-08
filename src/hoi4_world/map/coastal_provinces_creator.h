#ifndef SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H
#define SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H



#include <map>
#include <vector>

#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/hoi4_world/map/hoi4_province.h"
#include "src/maps/map_data.h"



namespace hoi4
{

CoastalProvinces CreateCoastalProvinces(const maps::MapData& map_data, const std::map<int, Province>& provinces);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_COASTALPROVINCESCREATOR_H