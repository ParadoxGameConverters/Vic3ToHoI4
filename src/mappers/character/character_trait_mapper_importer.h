#ifndef SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPERIMPORTER_H
#define SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPERIMPORTER_H



#include <filesystem>

#include "src/mappers/character/character_trait_mapper.h"



namespace mappers
{

CharacterTraitMapper ImportCharacterTraitMapper(const std::filesystem::path& mapping_file);

}  // namespace mappers



#endif  // SRC_MAPPERS_CHARACTER_CHARACTERTRAITMAPPERIMPORTER_H