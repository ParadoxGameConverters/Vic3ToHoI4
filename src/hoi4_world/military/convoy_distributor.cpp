#include "src/hoi4_world/military/convoy_distributor.h"

namespace hoi4
{

void ConvoyDistributor::CalculateStateWeights(const vic3::World& source_world)
{
   const auto& states = source_world.GetStates();
   const auto& buildings = source_world.GetBuildings();

   vic3_total_ = 0;
   vic3_map_ = {};
   for (const auto [id, state]: states)
   {
      const auto building = buildings.GetBuildingInState(id, vic3::BuildingType::Port);
      if (!building)
      {
         continue;
      }
      const auto& methods = building->GetProductionMethods();
      float pmWeight = 0;
      for (const auto pm: methods)
      {
         if (!pm_weights_.contains(pm))
         {
            continue;
         }
         pmWeight = pm_weights_[pm];
         break;
      }
      auto stateWeight = building->GetStaffingLevel() * pmWeight;
      vic3_total_ += stateWeight;
      vic3_map_[id] = stateWeight;
   }
}

int ConvoyDistributor::ConvoysFromState(int id) const
{
   if (!vic3_map_.contains(id))
   {
      return 0;
   }
   if (vic3_total_ == 0)
   {
      return 0;
   }
   auto weight = vic3_map_.at(id);
   weight *= hoi4_total_;
   weight /= vic3_total_;
   return static_cast<int>(std::round(weight));
}


}  // namespace hoi4
