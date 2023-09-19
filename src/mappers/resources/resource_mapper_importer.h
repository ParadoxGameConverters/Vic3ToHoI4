#ifndef SRC_MAPPERS_RESOURCES_RESOURCEMAPPERIMPORTER_H
#define SRC_MAPPERS_RESOURCES_RESOURCEMAPPERIMPORTER_H


#include "src/mappers/resources/resource_mapper.h"


namespace mappers
{

ResourceMapper ImportResourceMapper(std::string_view mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_RESOURCES_RESOURCEMAPPERIMPORTER_H
