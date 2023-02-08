#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H



#include <optional>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

class CountryConverter
{
  public:
   std::optional<Country> ConvertCountry(const vic3::Country& source_country,
       const mappers::CountryMapper& country_mapper, const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids);
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H