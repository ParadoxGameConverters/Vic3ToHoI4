#ifndef SRC_VIC3WORLD_CULTURES_CULTUREDEFINITIONSIMPORTER_H
#define SRC_VIC3WORLD_CULTURES_CULTUREDEFINITIONSIMPORTER_H



#include <external/commonItems/ModLoader/ModFilesystem.h>

#include <map>
#include <string>

#include "culture_definition.h"



namespace vic3
{

std::map<std::string, CultureDefinition> ImportCultureDefinitions(const commonItems::ModFilesystem& mod_filesystem);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CULTURES_CULTUREDEFINITIONSIMPORTER_H