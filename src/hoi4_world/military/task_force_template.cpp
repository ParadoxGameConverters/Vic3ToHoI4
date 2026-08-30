#include "src/hoi4_world/military/task_force_template.h"

#include <external/fmt/include/fmt/format.h>

#include <algorithm>



bool hoi4::TaskForceTemplate::AllVariantsActive(const std::set<std::string>& active_variants) const
{
   return std::ranges::all_of(ships_, [&active_variants](const hoi4::Ship& ship) {
      return active_variants.contains(ship.equipment) || active_variants.contains(ship.legacy_equipment) ||
             active_variants.contains(ship.version);
   });
}


void hoi4::TaskForceTemplate::AddShipsIfPossible(std::vector<hoi4::Ship>& ships,
    std::map<std::string, int>& ship_counts,
    std::map<std::string, float>& points) const
{
   int multiple = 1000;
   for (const auto& [pm, amount]: cost_)
   {
      const auto ratio = points[pm] / amount;
      if (ratio < 1)
      {
         return;
      }
      multiple = std::min(multiple, (int)floor(ratio));
   }

   for (const auto& [pm, amount]: cost_)
   {
      points[pm] -= amount * static_cast<float>(multiple);
   }
   for (int i = 0; i < multiple; ++i)
   {
      for (const auto& ship: ships_)
      {
         ship_counts[ship.name]++;
         const std::string ship_name = fmt::format("{} {}", ship.name, ship_counts[ship.name]);
         ships.emplace_back(Ship{
             .name = ship_name,
             .definition = ship.definition,
             .equipment = ship.equipment,
             .legacy_equipment = ship.legacy_equipment,
             .version = ship.version,
         });
      }
   }
}
