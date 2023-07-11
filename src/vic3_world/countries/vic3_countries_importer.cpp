#include "src/vic3_world/countries/vic3_countries_importer.h"

#include <sstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



std::map<int, vic3::Country> vic3::ImportCountries(const std::map<std::string, commonItems::Color>& color_definitions,
    std::istream& input_stream)
{
   CountryImporter country_importer_;

   std::map<int, Country> countries_;

   commonItems::parser database_parser_;
   database_parser_.registerRegex(commonItems::integerRegex,
       [color_definitions, &country_importer_, &countries_](const std::string& number_string,
           std::istream& input_stream) {
          const int country_number = std::stoi(number_string);
          const auto country_string = commonItems::stringOfItem(input_stream).getString();
          if (country_string.find("{") == std::string::npos)
          {
             return;
          }
          std::istringstream country_stream(country_string);

          std::optional<Country> new_country =
              country_importer_.ImportCountry(country_number, country_stream, color_definitions);
          if (new_country)
          {
             countries_.emplace(country_number, *new_country);
          }
       });

   commonItems::parser countries_parser_;
   countries_parser_.registerKeyword("database", [&database_parser_](std::istream& input_stream) {
      database_parser_.parseStream(input_stream);
   });
   countries_parser_.IgnoreUnregisteredItems();


   countries_.clear();
   countries_parser_.parseStream(input_stream);
   return countries_;
}