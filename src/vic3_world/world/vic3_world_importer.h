#ifndef VIC3WORLD_WORLD_VIC3WORLDIMPORTER_H
#define VIC3WORLD_WORLD_VIC3WORLDIMPORTER_H



#include <string_view>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/vic3_world/world/vic3_world.h"
#include "src/configuration/configuration.h"



namespace vic3
{


World ImportWorld(const configuration::Configuration& configuration);


}  // namespace vic3



#endif  // VIC3WORLD_WORLD_VIC3WORLDIMPORTER_H