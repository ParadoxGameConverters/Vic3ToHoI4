#include "src/mappers/unit/unit_mapper.h"

#include <algorithm>
#include <ranges>

#include "external/fmt/include/fmt/format.h"



namespace
{

mappers::UnitPointsMap CalculateUnitPoints(const mappers::PmToPointsMap& methods, const mappers::UnitPointsMap& pm_counts)
{
   mappers::UnitPointsMap point_totals;

   for (const auto& [pm, count]: pm_counts)
   {
      const auto& pm_itr = methods.find(pm);
      if (pm_itr == methods.end())
      {
         Log(LogLevel::Warning) << fmt::format("Missing unit mapping rule for {}", pm);
         continue;
      }

      for (const auto& [unit, points]: pm_itr->second)
      {
         auto value = points * count;
         auto [itr, success] = point_totals.emplace(unit, value);
         if (!success)
         {
            itr->second += value;
         }
      }
   }

   return point_totals;
}

}  // namespace



mappers::UnitPointsMap mappers::UnitMapper::CalculateUnitPoints(
    const mappers::UnitPointsMap& pm_counts) const
{
   return ::CalculateUnitPoints(methods_, pm_counts);
}

