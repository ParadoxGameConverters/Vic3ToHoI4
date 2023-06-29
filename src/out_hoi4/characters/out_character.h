#ifndef OUTHOI4_CHARACTERS_OUTCHARACTERS_H
#define OUTHOI4_CHARACTERS_OUTCHARACTERS_H



#include <vector>

#include "src/hoi4_world/characters/hoi4_character.h"



namespace out
{

void OutputCharacter(std::ostream& out, const std::string& tag, const hoi4::Character& character);
void OutputSpy(std::ostream& out, const hoi4::Character& character);

}  // namespace out



#endif  // OUTHOI4_CHARACTERS_OUTCHARACTERS_H