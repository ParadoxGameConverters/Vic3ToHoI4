#ifndef SRC_HOI4WORLD_RESOURCESMAPIMPORTER_H
#define SRC_HOI4WORLD_RESOURCESMAPIMPORTER_H



#include <string_view>

#include "src/hoi4_world/map/resources_map.h"



namespace hoi4
{

ResourcesMap ImportResources(std::string_view resources_file);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_RESOURCESMAPIMPORTER_H