#include "src/mappers/resources/resource_mapper.h"

float mappers::ResourceMapper::CalculateScore(const std::string& hoi_resource,
    const std::vector<vic3::Building>& buildings) const
{
   if (!scores_.contains(hoi_resource))
   {
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

   if (bonus > base)
   {
      bonus = base;
   }
   bonus /= base;
   bonus *= score.bonus;
   return base * (1.0F + bonus);
}
