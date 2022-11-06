#ifndef SRC_OUTHOI4_COUNTRIES_OUTCOUNTRIES_H
#define SRC_OUTHOI4_COUNTRIES_OUTCOUNTRIES_H



#include <map>

#include "src/hoi4_world/countries/hoi4_country.h"



namespace out
{

void OutputCountries(std::string_view output_name, const std::map<std::string, hoi4::Country>& countries);

}  // namespace out



#endif  // SRC_OUTHOI4_COUNTRIES_OUTCOUNTRIES_H