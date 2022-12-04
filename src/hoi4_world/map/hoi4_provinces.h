#ifndef SRC_HOI4WORLD_MAP_HOI4PROVINCES_H
#define SRC_HOI4WORLD_MAP_HOI4PROVINCES_H



#include <map>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/hoi4_world/map/hoi4_province.h"



namespace hoi4
{

[[nodiscard]] std::map<int, Province> ImportProvinces(const commonItems::ModFilesystem& mod_filesystem);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_HOI4PROVINCES_H