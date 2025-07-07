#ifndef SRC_HOI4WORLD_MAP_HOI4PROVINCEDEFINITIONIMPORTER_H
#define SRC_HOI4WORLD_MAP_HOI4PROVINCEDEFINITIONIMPORTER_H



#include <external/commonItems/ModLoader/ModFilesystem.h>

#include "src/maps/province_definitions.h"



namespace hoi4
{

[[nodiscard]] maps::ProvinceDefinitionsOptions ImportProvinceDefinitions(
    const commonItems::ModFilesystem& mod_filesystem);

}



#endif  // SRC_HOI4WORLD_MAP_HOI4PROVINCEDEFINITIONIMPORTER_H