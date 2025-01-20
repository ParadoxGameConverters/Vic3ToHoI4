#include "src/out_hoi4/countries/out_countries.h"

#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/fmt/include/fmt/format.h>

#include <fstream>
#include <ranges>

#include "src/out_hoi4/countries/out_country.h"
#include "src/out_hoi4/focus_trees/out_focus_tree.h"



using std::filesystem::copy_options;
using std::filesystem::path;



void out::OutputCountries(const path& output_name,
    const std::map<std::string, hoi4::Country>& countries,
    const std::map<int, hoi4::Character>& characters,
    configuration::UseStories use_stories)
{
   std::ofstream tags_file("output" / output_name / "common/country_tags/00_countries.txt");
   if (!tags_file.is_open())
   {
      throw std::runtime_error(
          fmt::format("Could not open output/{}/common/country_tags/00_countries.txt", output_name.string()));
   }

   for (const auto& [tag, country]: countries)
   {
      OutputCommonCountryTag(country, tags_file);
      OutputCommonCountriesFile(output_name, country);
      OutputCommonCharactersFile(output_name, country, characters);
      OutputCountryHistory(output_name, country, characters);
      const path oob_file = "output" / output_name / fmt::format("history/units/{}_1936.txt", tag);
      if (!copy_file("configurables/division_templates.txt", oob_file, copy_options::overwrite_existing))
      {
         throw std::runtime_error("Could not copy to " + oob_file.string());
      }
      OutputCountryUnits(oob_file, country);
      OutputCountryNavy(output_name, country);
      OutputFocusTree(output_name, tag, country.GetFocusTree(), use_stories);
   }

   tags_file.close();
}
