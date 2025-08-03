#include "src/vic3_world/countries/vic3_countries_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/ParserHelpers.h>

#include <sstream>



std::map<int, vic3::Country> vic3::ImportCountries(const std::map<std::string, commonItems::Color>& color_definitions,
    std::istream& input_stream)
{
   CountryImporter country_importer;

   std::map<int, Country> countries;

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [color_definitions, &country_importer, &countries](const std::string& number_string,
           std::istream& input_stream) {
          const int country_number = static_cast<int>(std::stoul(number_string));
          const auto country_string = commonItems::stringOfItem(input_stream).getString();
          if (country_string.find("{") == std::string::npos)
          {
             return;
          }
          std::istringstream country_stream(country_string);

          std::optional<Country> new_country =
              country_importer.ImportCountry(country_number, country_stream, color_definitions);
          if (new_country)
          {
             countries.emplace(country_number, *new_country);
          }
       });

   commonItems::parser countries_parser;
   countries_parser.registerKeyword("database", [&database_parser](std::istream& input_stream) {
      database_parser.parseStream(input_stream);
   });
   countries_parser.IgnoreUnregisteredItems();


   countries.clear();
   countries_parser.parseStream(input_stream);
   return countries;
}