#ifndef SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPERIMPORTER_H
#define SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPERIMPORTER_H



#include <filesystem>

#include "src/mappers/ideology/ideology_mapper.h"



namespace mappers
{

IdeologyMapper ImportIdeologyMapper(const std::filesystem::path& mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPERIMPORTER_H