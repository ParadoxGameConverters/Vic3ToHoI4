#include "src/vic3_world/military/ship_versions_importer.hpp"

#include <external/commonItems/Log.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/printf.h>

#include "src/vic3_world/database/database_parser.h"



namespace vic3
{

std::map<int64_t, std::string> ImportShipVersions(std::istream& input)
{
   std::map<int64_t, std::string> ship_versions;
   std::string type_string;


   commonItems::parser info_parser;
   info_parser.registerKeyword("type", [&type_string](std::istream& input_stream) {
      type_string = commonItems::getString(input_stream);
   });
   info_parser.IgnoreUnregisteredItems();

   commonItems::parser version_parser;
   version_parser.registerKeyword("info", [&info_parser](std::istream& input_stream) {
      info_parser.parseStream(input_stream);
   });
   version_parser.IgnoreUnregisteredItems();

   const auto& parser_function = [&ship_versions, &type_string, &version_parser](const std::string& number_string,
                                     std::istream& input_stream) {
      const int64_t version_number = std::stoll(number_string);
      type_string.clear();
      version_parser.parseStream(input_stream);
      ship_versions.emplace(version_number, type_string);
   };
   DatabaseParser versions_database_parser(parser_function);
   versions_database_parser.IgnoreUnregisteredItems();

   commonItems::parser manager_parser;
   manager_parser.registerKeyword("versions", [&versions_database_parser](std::istream& input_stream) {
      versions_database_parser.parseStream(input_stream);
   });
   manager_parser.IgnoreUnregisteredItems();

   manager_parser.parseStream(input);

   return ship_versions;
}

}  // namespace vic3