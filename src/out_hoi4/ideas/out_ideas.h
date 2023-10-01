#ifndef SRC_OUTHOI4_IDEAS_OUTIDEAS_H
#define SRC_OUTHOI4_IDEAS_OUTIDEAS_H



#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputMonarchIdeas(std::string_view output_name,
    const std::map<std::string, hoi4::Country>& countries,
    const std::map<int, hoi4::Character>& characters);

}



#endif  // SRC_OUTHOI4_IDEAS_OUTIDEAS_H