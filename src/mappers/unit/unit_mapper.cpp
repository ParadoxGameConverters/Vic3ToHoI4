#include "src/mappers/unit/unit_mapper.h"

#include <algorithm>
#include <ranges>

#include "external/fmt/include/fmt/format.h"

std::vector<hoi4::Battalion> mappers::UnitMapper::MakeBattalions(
    const std::vector<std::string> methods, int scale) const
{
   static std::set<std::string> warned;

   int equip = 0;
   BattalionMap current;
   for (const auto& pm: methods)
   {
      const auto& itr = templates_.find(pm);
      if (itr == templates_.end())
      {
         if (warned.find(pm) == warned.end())
         {
            warned.insert(pm);
            Log(LogLevel::Warning) << fmt::format("Missing unit mapping rule for {}", pm);
         }
         continue;
      }

      equip += itr->second.equipment;
      for (const auto& [ut, str]: itr->second.units) {
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

