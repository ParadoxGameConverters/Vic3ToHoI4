#include "src/out_hoi4/countries/out_country.h"

#include <fstream>
#include <string>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"



void out::OutputCommonCountriesFile(std::string_view output_name, const hoi4::Country& country)
{
   const std::string& tag = country.GetTag();

   std::ofstream common_country(fmt::format("output/{}/common/countries/{}.txt", output_name, tag));
   if (!common_country.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/common/countries/{}.txt", output_name, tag));
   }
   common_country.close();
}


void out::OutputCommonCountryTag(const hoi4::Country& country, std::ofstream& tags_file)
{
   tags_file << fmt::format("{0} = \"countries/{0}.txt\"\n", country.GetTag());
}


void out::OutputCountryHistory(std::string_view output_name, const hoi4::Country& country)
{
   const std::string& tag = country.GetTag();

   std::ofstream country_history(fmt::format("output/{}/history/countries/{}.txt", output_name, tag));
   if (!country_history.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/history/countries/{}.txt", output_name, tag));
   }
   country_history.close();
}