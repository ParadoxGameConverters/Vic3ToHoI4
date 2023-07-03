#ifndef SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H
#define SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H



#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/interest_groups/interest_group.h"


namespace hoi4
{

std::optional<Character> ConvertCharacter(const int character_id,
    vic3::Character& source_character,
    const std::map<int, vic3::InterestGroup>& igs);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H