#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H



#include <map>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

std::map<std::string, Country> ConvertCountries(const std::map<int, vic3::Country>& source_countries,
    const std::map<int, std::set<std::string>>& source_technologies,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<mappers::TechMapping>& tech_mappings);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H