#include "src/hoi4_world/countries/hoi4_countries_converter.h"

#include <ranges>



std::map<std::string, hoi4::Country> hoi4::CountriesConverter::ConvertCountries(
    const std::map<int, vic3::Country>& source_countries,
    const std::map<int, std::set<std::string>>& source_technologies,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<mappers::TechMapping>& tech_mappings)
{
   std::map<std::string, Country> countries;

   for (const auto& [country_number, source_country]: source_countries)
   {
      std::set<std::string> source_country_technologies;
      if (const auto& source_technologies_itr = source_technologies.find(country_number);
          source_technologies_itr != source_technologies.end())
      {
         source_country_technologies = source_technologies_itr->second;
      }

      std::optional<Country> new_country = country_converter_.ConvertCountry(source_country,
          source_country_technologies,
          country_mapper,
          vic3_state_ids_to_hoi4_state_ids,
          tech_mappings);
      if (new_country.has_value())
      {
         countries.emplace(new_country->GetTag(), *new_country);
      }
   }

   return countries;
}