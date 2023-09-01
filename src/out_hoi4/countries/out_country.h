#ifndef SRC_OUTHOI4_COUNTRIES_OUTCOUNTRY_H
#define SRC_OUTHOI4_COUNTRIES_OUTCOUNTRY_H



#include <string_view>

#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputCommonCountriesFile(std::string_view output_name, const hoi4::Country& country);

void OutputCommonCountryTag(const hoi4::Country& country, std::ofstream& tags_file);

void OutputCommonCharactersFile(std::string_view output_name,
    const hoi4::Country& country,
    const std::map<int, hoi4::Character>& characters);

void OutputCountryHistory(std::string_view output_name,
    const hoi4::Country& country,
    const std::map<int, hoi4::Character>& characters);

void OutputCountryUnits(const std::string& oob_file, const hoi4::Country& country);

}  // namespace out



#endif  // SRC_OUTHOI4_COUNTRIES_OUTCOUNTRY_H
