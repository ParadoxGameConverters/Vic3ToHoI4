#include "src/vic3_world/buildings/buildings_importer.h"

#include <map>
#include <vector>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "src/vic3_world/buildings/building_importer.h"



vic3::Buildings vic3::ImportBuildings(std::istream& input_stream)
{
   std::map<int, std::vector<Building>> buildings;

   BuildingImporter building_importer;

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&buildings, &building_importer](const std::string& unused, std::istream& input_stream) {
          const Building new_building = building_importer.ImportBuilding(input_stream);
          if (!new_building.GetStateNumber())
          {
             return;
          }

          const auto [iterator, success] = buildings.emplace(*new_building.GetStateNumber(), std::vector{new_building});
          if (!success)
          {
             iterator->second.push_back(new_building);
          }
       });

   commonItems::parser buildings_parser;
   buildings_parser.registerKeyword("database", [&buildings, &database_parser](std::istream& input_stream) {
      buildings.clear();
      database_parser.parseStream(input_stream);
   });

   buildings_parser.parseStream(input_stream);

   return Buildings{buildings};
}