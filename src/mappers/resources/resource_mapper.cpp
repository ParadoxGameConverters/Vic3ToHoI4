#include "src/mappers/resources/resource_mapper.h"

#include <set>

#include "external/commonItems/Log.h"

namespace
{

std::set<std::string> seen;

void warn(const std::string& hoi_resource)
{
   if (seen.contains(hoi_resource))
   {
      return;
   }
   Log(LogLevel::Warning) << "Configuration for " << hoi_resource << " is missing.";
   seen.insert(hoi_resource);
}

}  // namespace

float mappers::ResourceMapper::CalculateScore(const std::string& hoi_resource,
    const std::vector<vic3::Building>& buildings) const
{
   if (!scores_.contains(hoi_resource))
   {
      warn(hoi_resource);
      return 0.0F;
   }
   float base = 0.0F;
   float bonus = 0.0F;
   const auto& score = scores_.at(hoi_resource);
   for (const auto& building: buildings)
   {
      if (score.base.contains(building.GetType()))
      {
         base += building.GetGoodsSalesValues() * score.base.at(building.GetType());
      }
      if (score.bonus_buildings.contains(building.GetType()))
      {
         bonus += building.GetGoodsSalesValues();
      }
   }

   if (base == 0.0F)
   {
      return base;
   }
   if (bonus > base)
   {
      bonus = base;
   }
   bonus /= base;
   bonus *= score.bonus;
   return base * (1.0F + bonus);
}

float mappers::ResourceMapper::WorldTotal(const std::string& hoi_resource) const
{
   if (!scores_.contains(hoi_resource))
   {
      warn(hoi_resource);
      return 0.0F;
   }

   return scores_.at(hoi_resource).total;
}
