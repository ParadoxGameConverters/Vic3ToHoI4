#ifndef SRC_HOI4WORLD_MAP_BUILDINGSCREATOR_H
#define SRC_HOI4WORLD_MAP_BUILDINGSCREATOR_H



#include <vector>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/hoi4_world/map/buildings.h"
#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/maps/map_data.h"



namespace hoi4
{

Buildings ImportBuildings(const std::vector<hoi4::State>& states,
    const hoi4::CoastalProvinces& coastal_provinces,
    const maps::MapData& map_data,
    const commonItems::ModFilesystem& mod_filesystem);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_BUILDINGSCREATOR_H