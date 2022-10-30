#ifndef VIC3WORLD_WORLD_VIC3WORLDIMPORTER_H
#define VIC3WORLD_WORLD_VIC3WORLDIMPORTER_H



#include <string_view>
#include "src/vic3_world/world/vic3_world.h"



namespace vic3
{


World ImportWorld(std::string_view save_filename);


}  // namespace vic3



#endif  // VIC3WORLD_WORLD_VIC3WORLDIMPORTER_H