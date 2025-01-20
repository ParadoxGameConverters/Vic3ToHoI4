#ifndef SRC_OUTHOI4_COUNTRIES_OUTCOUNTRY_H
#define SRC_OUTHOI4_COUNTRIES_OUTCOUNTRY_H



#include <filesystem>
#include <string_view>

#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputCommonCountriesFile(const std::filesystem::path& output_name, const hoi4::Country& country);

void OutputCommonCountryTag(const hoi4::Country& country, std::ofstream& tags_file);

void OutputCommonCharactersFile(const std::filesystem::path& output_name,
    const hoi4::Country& country,
    const std::map<int, hoi4::Character>& characters);

void OutputCountryHistory(const std::filesystem::path& output_name,
    const hoi4::Country& country,
    const std::map<int, hoi4::Character>& characters);

void OutputCountryNavy(const std::filesystem::path& output_name, const hoi4::Country& country);

void OutputCountryUnits(const std::filesystem::path& oob_file, const hoi4::Country& country);



}  // namespace out



#endif  // SRC_OUTHOI4_COUNTRIES_OUTCOUNTRY_H
