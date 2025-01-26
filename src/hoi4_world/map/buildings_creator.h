#ifndef SRC_HOI4WORLD_MAP_BUILDINGSCREATOR_H
#define SRC_HOI4WORLD_MAP_BUILDINGSCREATOR_H



#include <external/commonItems/ModLoader/ModFilesystem.h>

#include <vector>

#include "src/hoi4_world/map/buildings.h"
#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/states/hoi4_states.h"
#include "src/maps/map_data.h"



namespace hoi4
{

Buildings ImportBuildings(const States& states,
    const CoastalProvinces& coastal_provinces,
    const maps::MapData& map_data,
    const commonItems::ModFilesystem& mod_filesystem);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_BUILDINGSCREATOR_H