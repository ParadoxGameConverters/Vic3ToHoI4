#ifndef SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTERSIMPORTER_H
#define SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTERSIMPORTER_H



#include <istream>

#include "src/vic3_world/characters/vic3_character.h"


namespace vic3
{

std::map<int, Character> ImportCharacters(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTERSIMPORTER_H