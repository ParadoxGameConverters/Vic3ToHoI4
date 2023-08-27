#include "src/mappers/unit/unit_mapper.h"

#include <algorithm>
#include <ranges>

#include "external/fmt/include/fmt/format.h"

std::set<std::string> mappers::UnitMapper::warned_;

std::vector<hoi4::Battalion> mappers::UnitMapper::MakeBattalions(const std::vector<std::string> methods,
    int scale) const
{
   int equip = 0;
   BattalionMap current;
   for (const auto& pm: methods)
   {
      const auto& itr = templates_.find(pm);
      if (itr == templates_.end())
      {
         if (warned_.find(pm) == warned_.end())
         {
            Log(LogLevel::Warning) << fmt::format("Missing unit mapping rule for {}", pm);
         }
         warned_.insert(pm);
         continue;
      }

      equip += itr->second.equipment;
      for (const auto& [ut, str]: itr->second.units)
      {
         current[ut] += str * scale;
      }
   }

   std::vector<hoi4::Battalion> units;
   for (const auto& [ut, str]: current)
   {
      units.emplace_back(hoi4::Battalion(ut, equip, str));
   }
   return units;
}
