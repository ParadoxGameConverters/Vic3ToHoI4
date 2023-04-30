#ifndef SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPERIMPORTER_H
#define SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPERIMPORTER_H



#include "src/mappers/ideology/ideology_mapper.h"



namespace mappers
{

IdeologyMapper ImportIdeologyMapper(std::string_view mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPERIMPORTER_H