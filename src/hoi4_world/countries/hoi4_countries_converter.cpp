#include "src/hoi4_world/countries/hoi4_countries_converter.h"

#include <ranges>



std::map<std::string, hoi4::Country> hoi4::CountriesConverter::ConvertCountries(
    const std::map<int, vic3::Country>& source_countries,
    const mappers::CountryMapper& country_mapper)
{
   std::map<std::string, Country> countries;

   for (const auto& source_country: source_countries | std::views::values)
   {
      Country new_country = country_converter_.ConvertCountry(source_country, country_mapper);
      countries.emplace(new_country.GetTag(), new_country);
   }

   return countries;
}