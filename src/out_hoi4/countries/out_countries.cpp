#include "src/out_hoi4/countries/out_countries.h"

#include <fstream>
#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/countries/out_country.h"



void out::OutputCountries(std::string_view output_name, const std::map<std::string, hoi4::Country>& countries)
{
   std::ofstream tags_file(fmt::format("output/{}/common/country_tags/00_countries.txt", output_name));
   if (!tags_file.is_open())
   {
      throw std::runtime_error(
          fmt::format("Could not open output/{}/common/country_tags/00_countries.txt", output_name));
   }

   for (const auto& country: countries | std::views::values)
   {
      OutputCommonCountryTag(country, tags_file);
      OutputCommonCountriesFile(output_name, country);
      OutputCountryHistory(output_name, country);
   }

   tags_file.close();
}