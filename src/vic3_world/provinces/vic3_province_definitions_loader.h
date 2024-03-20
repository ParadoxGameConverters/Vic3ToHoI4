#ifndef SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONSLOADER_H
#define SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONSLOADER_H



#include <string>
#include <vector>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/Parser.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"



namespace vic3
{

[[nodiscard]] ProvinceDefinitions LoadProvinceDefinitions();

}  // namespace vic3



#endif  // SRC_VIC3WORLD_PROVINCES_VIC3PROVINCEDEFINITIONSLOADER_H