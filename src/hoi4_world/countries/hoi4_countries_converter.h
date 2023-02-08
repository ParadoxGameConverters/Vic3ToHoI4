#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H



#include <map>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

class CountriesConverter
{
  public:
   std::map<std::string, Country> ConvertCountries(const std::map<int, vic3::Country>& source_countries,
       const mappers::CountryMapper& country_mapper,
       const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids);

  private:
   CountryConverter country_converter_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H