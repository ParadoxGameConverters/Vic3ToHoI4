#include "src/vic3_world/countries/vic3_countries_importer.h"

#include <sstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



vic3::CountriesImporter::CountriesImporter(const std::map<std::string, commonItems::Color>& color_definitions)
{
   countries_parser_.registerKeyword("database", [this](std::istream& input_stream) {
      database_parser_.parseStream(input_stream);
   });
   countries_parser_.IgnoreUnregisteredItems();

   database_parser_.registerRegex(commonItems::integerRegex,
       [this, color_definitions](const std::string& number_string, std::istream& input_stream) {
          const int country_number = std::stoi(number_string);
          const auto country_string = commonItems::stringOfItem(input_stream).getString();
          if (country_string.find("{") == std::string::npos)
          {
             return;
          }
          std::istringstream country_stream(country_string);
          countries_.emplace(country_number, country_importer_.ImportCountry(country_stream, color_definitions));
       });
}


std::map<int, vic3::Country> vic3::CountriesImporter::ImportCountries(std::istream& input_stream)
{
   countries_.clear();
   countries_parser_.parseStream(input_stream);
   return countries_;
}