#include "src/vic3_world/technology/vic3_technology_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>

#include <sstream>



std::map<int, std::set<std::string>> vic3::ImportAcquiredTechnologies(std::istream& input_stream)
{
   std::map<int, std::set<std::string>> all_acquired_technologies;

   std::optional<int> country_number;
   std::set<std::string> acquired_technologies;

   commonItems::parser entry_parser;
   entry_parser.registerKeyword("country", [&country_number](std::istream& input_stream) {
      country_number = commonItems::getInt(input_stream);
   });
   entry_parser.registerKeyword("acquired_technologies", [&acquired_technologies](std::istream& input_stream) {
      for (const auto& acquired_technology: commonItems::getStrings(input_stream))
      {
         acquired_technologies.insert(acquired_technology);
      }
   });
   entry_parser.IgnoreUnregisteredItems();

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&entry_parser, &country_number, &acquired_technologies, &all_acquired_technologies](const std::string& unused,
           std::istream& input_stream) {
          const auto entry_string = commonItems::stringOfItem(input_stream).getString();
          if (entry_string.find("{") == std::string::npos)
          {
             return;
          }
          std::istringstream entry_stream(entry_string);

          country_number.reset();
          acquired_technologies.clear();

          entry_parser.parseStream(entry_stream);

          if (country_number)
          {
             all_acquired_technologies.emplace(*country_number, acquired_technologies);
          }
       });

   commonItems::parser technology_parser;
   technology_parser.registerKeyword("database", [&database_parser](std::istream& input_stream) {
      database_parser.parseStream(input_stream);
   });
   technology_parser.IgnoreUnregisteredItems();

   technology_parser.parseStream(input_stream);

   return all_acquired_technologies;
}