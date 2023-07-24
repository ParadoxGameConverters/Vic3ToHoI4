#ifndef SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPERIMPORTER_H
#define SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPERIMPORTER_H



#include "src/mappers/character/character_trait_mapper.h"



namespace mappers
{

CharacterTraitMapper ImportCharacterTraitMapper(std::string_view mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPERIMPORTER_H