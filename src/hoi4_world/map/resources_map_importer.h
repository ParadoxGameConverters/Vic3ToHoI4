#ifndef SRC_HOI4WORLD_RESOURCESMAPIMPORTER_H
#define SRC_HOI4WORLD_RESOURCESMAPIMPORTER_H



#include <filesystem>

#include "src/hoi4_world/map/resources_map.h"



namespace hoi4
{

ResourcesMap ImportResources(const std::filesystem::path& resources_file);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_RESOURCESMAPIMPORTER_H