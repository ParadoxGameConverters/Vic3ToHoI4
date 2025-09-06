#include "src/mappers/unit/unit_mapper.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>

#include <algorithm>
#include <ranges>



namespace
{

void WarnForMissingMapping(const std::string& pm, std::set<std::string>& warned)
{
   if (warned.contains(pm))
   {
      Log(LogLevel::Warning) << fmt::format("Missing unit mapping rule for {}", pm);
   }
   warned.insert(pm);
}

}  // namespace


std::vector<hoi4::Battalion> mappers::UnitMapper::MakeBattalions(const std::vector<std::string>& methods,
    int scale) const
{
   int equip = 0;
   BattalionMap current;
   for (const auto& pm: methods)
   {
      const auto& itr = templates_.find(pm);
      if (itr == templates_.end())
      {
         WarnForMissingMapping(pm, warned_);
         continue;
      }

      equip += itr->second.equipment;
      for (const auto& [ut, str]: itr->second.units)
      {
         current[ut] += str * static_cast<float>(scale);
      }
   }

   std::vector<hoi4::Battalion> units;
   for (const auto& [ut, str]: current)
   {
      units.emplace_back(ut, equip, str);
   }
   return units;
}


std::vector<hoi4::Battalion> mappers::UnitMapper::MakeBattalions(const vic3::MilitaryFormation& formation) const
{
   int equip = 0;
   BattalionMap current;
   for (const auto& [unit_type, amount]: formation.units)
   {
      const auto& itr = templates_.find(unit_type);
      if (itr == templates_.end())
      {
         WarnForMissingMapping(unit_type, warned_);
         continue;
      }

      equip += itr->second.equipment;
      for (const auto& [ut, str]: itr->second.units)
      {
         current[ut] += str * static_cast<float>(amount);
      }
   }
   for (const vic3::CombatUnit& combat_unit: formation.combat_units)
   {
      if (!combat_unit.type)
      {
         continue;
      }
      const auto& itr = templates_.find(combat_unit.type.value());
      if (itr == templates_.end())
      {
         WarnForMissingMapping(combat_unit.type.value(), warned_);
         continue;
      }

      equip += itr->second.equipment;
      for (const auto& [ut, str]: itr->second.units)
      {
         current[ut] += str * static_cast<float>(combat_unit.current_manpower) / 1000.0F;
      }
   }


   std::vector<hoi4::Battalion> units;
   for (const auto& [ut, str]: current)
   {
      units.emplace_back(ut, equip, str);
   }
   return units;
}