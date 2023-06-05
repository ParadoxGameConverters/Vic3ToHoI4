#ifndef SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPERIMPORTER_H
#define SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPERIMPORTER_H



#include "src/mappers/culture/culture_graphics_mapper.h"



namespace mappers
{

CultureGraphicsMapper ImportCultureGraphicsMapper(std::string_view mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_CULTURE_CULTUREGRAPHICSMAPPERIMPORTER_H