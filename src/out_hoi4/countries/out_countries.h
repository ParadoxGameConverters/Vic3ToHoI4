#ifndef SRC_OUTHOI4_COUNTRIES_OUTCOUNTRIES_H
#define SRC_OUTHOI4_COUNTRIES_OUTCOUNTRIES_H



#include <map>

#include "src/configuration/configuration.h"
#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputCountries(std::string_view output_name,
    const std::map<std::string, hoi4::Country>& countries,
    const std::map<int, hoi4::Character>& characters,
    configuration::UseStories use_stories);

}  // namespace out



#endif  // SRC_OUTHOI4_COUNTRIES_OUTCOUNTRIES_H