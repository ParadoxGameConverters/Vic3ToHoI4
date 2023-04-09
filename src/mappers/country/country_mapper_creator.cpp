#include "src/mappers/country/country_mapper_creator.h"

#include <ranges>

#include "external/fmt/include/fmt/format.h"



mappers::CountryMapper mappers::CreateCountryMappings(const std::map<int, vic3::Country>& countries)
{
   std::map<int, std::string> country_mappings;

   char tag_prefix = 'Z';
   int tag_suffix = 0;

   for (const vic3::Country& country: countries | std::views::values)
   {
      const auto& tag = country.GetTag();
      country_mappings.emplace(country.GetNumber(), fmt::format("{}{:0>2}", tag_prefix, tag_suffix));

      ++tag_suffix;
      if (tag_suffix > 99)
      {
         tag_suffix = 0;
         --tag_prefix;
      }
   }

   return CountryMapper(country_mappings);
}