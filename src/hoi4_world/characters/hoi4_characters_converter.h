#ifndef SRC_HOI4WORLD_CHARACTERS_CHARACTERSCONVERTER_H
#define SRC_HOI4WORLD_CHARACTERS_CHARACTERSCONVERTER_H



#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/interest_groups/interest_group.h"


namespace hoi4
{

std::map<int, Character> ConvertCharacters(const std::map<int, vic3::Character>& source_characters,
    const std::map<int, vic3::InterestGroup>& igs);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_CHARACTERS_CHARACTERSCONVERTER_H