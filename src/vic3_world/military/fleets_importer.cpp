#include "src/vic3_world/military/fleets_importer.hpp"

#include <external/commonItems/ParserHelpers.h>

#include <istream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "src/vic3_world/database/database_parser.h"



namespace vic3
{

std::map<int64_t, std::vector<int64_t>> ImportShips(std::istream& input_stream)
{
   std::map<int64_t, std::vector<int64_t>> ships;

   std::optional<int64_t> fleet;
   std::optional<int64_t> version;

   commonItems::parser ship_parser;
   ship_parser.registerKeyword("fleet", [&fleet](std::istream& input) {
      fleet = commonItems::getLlong(input);
   });
   ship_parser.registerKeyword("version", [&version](std::istream& input) {
      version = commonItems::getLlong(input);
   });
   ship_parser.IgnoreAndLogUnregisteredItems();

   const auto& parser_function = [&ship_parser, &fleet, &version, &ships]([[maybe_unused]] const std::string& unused,
                                     std::istream& input_stream) {
      fleet.reset();
      version.reset();
      ship_parser.parseStream(input_stream);
      if (!fleet || !version)
      {
         return;
      }

      if (auto [itr, success] = ships.emplace(*fleet, std::vector{*version}); !success)
      {
         itr->second.push_back(*version);
      }
   };
   DatabaseParser database_parser(parser_function);
   database_parser.IgnoreAndLogUnregisteredItems();

   database_parser.parseStream(input_stream);

   return ships;
}


std::map<int64_t, std::vector<std::string>> AddShipTypes(const std::map<int64_t, std::string>& ship_versions,
    const std::map<int64_t, std::vector<int64_t>>& ships)
{
   std::map<int64_t, std::vector<std::string>> new_ships;
   for (const auto [fleet_id, ships_in_fleet]: ships)
   {
      std::vector<std::string> ship_types;
      for (int64_t version: ships_in_fleet)
      {
         if (auto itr = ship_versions.find(version); itr != ship_versions.end())
         {
            ship_types.push_back(itr->second);
         }
      }
      new_ships.emplace(fleet_id, ship_types);
   }

   return new_ships;
}

}  // namespace vic3