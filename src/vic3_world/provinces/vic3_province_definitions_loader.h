#ifndef SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONSLOADER_H
#define SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONSLOADER_H



#include <external/commonItems/ModLoader/ModFilesystem.h>
#include <external/commonItems/Parser.h>

#include <string>
#include <vector>

#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/states/state_regions.h"



namespace vic3
{

[[nodiscard]] ProvinceDefinitions LoadProvinceDefinitions(const StateRegions& state_regions,
    const commonItems::ModFilesystem& filesystem);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONSLOADER_H