#include "src/out_hoi4/countries/out_countries.h"

#include <fstream>
#include <ranges>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/countries/out_country.h"
#include "src/out_hoi4/focus_trees/out_focus_tree.h"



void out::OutputCountries(std::string_view output_name,
    const std::map<std::string, hoi4::Country>& countries,
    const std::map<int, hoi4::Character>& characters,
    configuration::UseStories use_stories)
{
   std::ofstream tags_file(fmt::format("output/{}/common/country_tags/00_countries.txt", output_name));
   if (!tags_file.is_open())
   {
      throw std::runtime_error(
          fmt::format("Could not open output/{}/common/country_tags/00_countries.txt", output_name));
   }

   for (const auto& [tag, country]: countries)
   {
      OutputCommonCountryTag(country, tags_file);
      OutputCommonCountriesFile(output_name, country);
      OutputCommonCharactersFile(output_name, country, characters);
      OutputCountryHistory(output_name, country, characters);
      auto oob_file = fmt::format("output/{}/history/units/{}_1936.txt", output_name, tag);
      commonItems::TryCopyFile("configurables/division_templates.txt", oob_file);
      OutputCountryUnits(oob_file, country);
      OutputCountryNavy(output_name, country);
      OutputFocusTree(output_name, tag, country.GetFocusTree(), use_stories);
   }

   tags_file.close();
}
