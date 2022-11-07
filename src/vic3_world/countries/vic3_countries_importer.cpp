#include "src/vic3_world/countries/vic3_countries_importer.h"

#include <sstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



vic3::CountriesImporter::CountriesImporter(bool debug): country_importer_(debug)
{
   countries_parser_.registerKeyword("database", [this](std::istream& input_stream) {
      database_parser_.parseStream(input_stream);
   });
   if (debug)
   {
      countries_parser_.registerRegex(commonItems::catchallRegex, commonItems::ignoreAndLogItem);
   }
   else
   {
      countries_parser_.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
   }

   database_parser_.registerRegex(commonItems::integerRegex,
       [this](const std::string& number_string, std::istream& input_stream) {
          const int country_number = std::stoi(number_string);
          const auto country_string = commonItems::stringOfItem(input_stream).getString();
          if (country_string.find("none") != std::string::npos)
          {
             return;
          }
          std::istringstream country_stream(country_string);
          countries_.emplace(country_number, country_importer_.ImportCountry(country_stream));
       });
}


std::map<int, vic3::Country> vic3::CountriesImporter::ImportCountries(std::istream& input_stream)
{
   countries_.clear();
   countries_parser_.parseStream(input_stream);
   return countries_;
}