#include "src/hoi4_world/military/task_force_template.h"

#include <external/fmt/include/fmt/format.h>

bool hoi4::TaskForceTemplate::AllVariantsActive(const std::set<std::string>& active_variants) const
{
   for (const auto& ship: ships_)
   {
      if (!active_variants.contains(ship.GetEquipment()))
      {
         return false;
      }
      if (!active_variants.contains(ship.GetLegacyEquipment()))
      {
         return false;
      }
      if (!active_variants.contains(ship.GetVersion()))
      {
         return false;
      }
   }
   return true;
}

void hoi4::TaskForceTemplate::AddShipsIfPossible(std::vector<hoi4::Ship>& ships,
    std::map<std::string, int>& ship_counts,
    std::map<std::string, float>& points) const
{
   int multiple = 1000;
   for (const auto& [pm, amount]: cost_)
   {
      auto ratio = points[pm] / amount;
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
         ship_counts[ship.GetName()]++;
         std::string ship_name = fmt::format("{} {}", ship.GetName().c_str(), ship_counts[ship.GetName()]);
         ships.emplace_back(ship_name,
             ship.GetDefinition(),
             ship.GetEquipment(),
             ship.GetLegacyEquipment(),
             ship.GetVersion());
      }
   }
}
