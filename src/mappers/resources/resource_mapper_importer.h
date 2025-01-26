#ifndef SRC_MAPPERS_RESOURCES_RESOURCEMAPPERIMPORTER_H
#define SRC_MAPPERS_RESOURCES_RESOURCEMAPPERIMPORTER_H



#include <filesystem>

#include "src/mappers/resources/resource_mapper.h"



namespace mappers
{

ResourceMapper ImportResourceMapper(const std::filesystem::path& mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_RESOURCES_RESOURCEMAPPERIMPORTER_H
