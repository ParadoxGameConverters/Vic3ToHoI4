#include "src/hoi4_world/military/ship_converter.hpp"

#include <external/fmt/include/fmt/format.h>



namespace hoi4
{

std::vector<hoi4::Ship> ShipConverter::ConvertShips(const std::vector<std::string>& vic3_ships,
    std::map<std::string, int>& ship_counts) const
{
   std::vector<Ship> ships;

   for (const std::string vic3_ship: vic3_ships)
   {
      const auto mapping = ship_mappings_.find(vic3_ship);
      if (mapping == ship_mappings_.end())
      {
         continue;
      }

      ship_counts[mapping->second.name]++;
      const std::string ship_name = fmt::format("{} {}", mapping->second.name, ship_counts[mapping->second.name]);
      ships.emplace_back(Ship{
          .name = ship_name,
          .definition = mapping->second.definition,
          .equipment = mapping->second.equipment,
          .legacy_equipment = mapping->second.legacy_equipment,
          .version = mapping->second.version,
      });
   }

   return ships;
}

}  // namespace hoi4