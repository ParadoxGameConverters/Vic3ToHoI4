#ifndef SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPERIMPORTER_H
#define SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPERIMPORTER_H



#include "src/mappers/character/leader_type_mapper.h"



namespace mappers
{

LeaderTypeMapper ImportLeaderTypeMapper(std::string_view mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_CHARACTER_LEADERTYPEMAPPERIMPORTER_H