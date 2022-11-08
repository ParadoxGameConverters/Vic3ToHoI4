#include "src/out_hoi4/countries/out_country.h"

#include <fstream>
#include <string>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"



void out::OutputCommonCountriesFile(std::string_view output_name, const hoi4::Country& country)
{
   const std::string& tag = country.GetTag();

   const auto common_country_file_name = fmt::format("output/{}/common/countries/{}.txt", output_name, tag);
   std::ofstream common_country(common_country_file_name);
   if (!common_country.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", common_country_file_name));
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

   const auto country_history_file_name = fmt::format("output/{}/history/countries/{}.txt", output_name, tag);
   std::ofstream country_history(country_history_file_name);
   if (!country_history.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", country_history_file_name));
   }
   country_history.close();
}