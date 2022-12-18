#ifndef SRC_VIC3WORLD_COUNTRIES_COUNTRYDEFINITIONSIMPORTER_H
#define SRC_VIC3WORLD_COUNTRIES_COUNTRYDEFINITIONSIMPORTER_H



#include <map>

#include "external/commonItems/Color.h"
#include "external/commonItems/ModLoader/ModFilesystem.h"



namespace vic3
{

std::map<std::string, commonItems::Color> ImportCountryColorDefinitions(
    const commonItems::ModFilesystem& mod_filesystem);

}



#endif  // SRC_VIC3WORLD_COUNTRIES_COUNTRYDEFINITIONSIMPORTER_H