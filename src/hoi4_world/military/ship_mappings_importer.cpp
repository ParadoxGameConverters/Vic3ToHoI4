#include "src/hoi4_world/military/ship_mappings_importer.hpp"

#include <istream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "src/hoi4_world/military/ship.h"



namespace hoi4
{

std::map<std::string, hoi4::Ship> ImportShipMappings(std::string_view input_file)
{
   std::map<std::string, hoi4::Ship> ship_mappings;
   hoi4::Ship ship;

   commonItems::parser ship_parser;
   ship_parser.registerKeyword("name", [&ship](std::istream& input) {
      ship.name = commonItems::getString(input);
   });
   ship_parser.registerKeyword("definition", [&ship](std::istream& input) {
      ship.definition = commonItems::getString(input);
   });
   ship_parser.registerKeyword("equipment", [&ship](std::istream& input) {
      ship.equipment = commonItems::getString(input);
   });
   ship_parser.registerKeyword("legacy_equipment", [&ship](std::istream& input) {
      ship.legacy_equipment = commonItems::getString(input);
   });
   ship_parser.registerKeyword("version", [&ship](std::istream& input) {
      ship.version = commonItems::getString(input);
   });

   commonItems::parser parser;
   parser.registerRegex(commonItems::catchallRegex,
       [&ship_mappings, &ship, &ship_parser](const std::string& vic3_ship, std::istream& input) {
          ship = {};
          ship_parser.parseStream(input);

          ship_mappings.emplace(vic3_ship, ship);
       });

   parser.parseFile(input_file);
   return ship_mappings;
}

}  // namespace hoi4