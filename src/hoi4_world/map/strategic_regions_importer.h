#ifndef SRC_HOI4WORLD_MAP_STRATEGICREGIONSIMPORTER_H
#define SRC_HOI4WORLD_MAP_STRATEGICREGIONSIMPORTER_H



#include <external/commonItems/ModLoader/ModFilesystem.h>

#include <map>
#include <optional>

#include "src/hoi4_world/map/strategic_regions.h"



namespace hoi4
{

StrategicRegions ImportStrategicRegions(const commonItems::ModFilesystem& hoi4_mod_filesystem);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_STRATEGICREGIONSIMPORTER_H