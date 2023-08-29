#include "src/vic3_world/buildings/buildings.h"

#include <numeric>
#include <ranges>



float vic3::Buildings::GetTotalGoodSalesValueInState(int state_number) const
{
   if (const auto buildings_in_state = buildings_.find(state_number); buildings_in_state != buildings_.end())
   {
      return std::accumulate(buildings_in_state->second.begin(),
          buildings_in_state->second.end(),
          0.0F,
          [](float total_value, const vic3::Building& building) {
             return total_value + building.GetGoodsSalesValues();
          });
   }

   return 0.0F;
}



float vic3::Buildings::GetTotalGoodSalesValueInWorld() const
{
   float total_value = 0.0F;

   for (const std::vector<Building>& buildings_in_state: buildings_ | std::views::values)
   {
      total_value += std::accumulate(buildings_in_state.begin(),
          buildings_in_state.end(),
          0.0F,
          [](float total_value, const vic3::Building& building) {
             return total_value + building.GetGoodsSalesValues();
          });
   }

   return total_value;
}


const std::optional<vic3::Building> vic3::Buildings::GetBuildingInState(int state_number,
    const std::string& building_type) const
{
   const auto itr = buildings_.find(state_number);
   if (itr == buildings_.end())
   {
      return {};
   }
   auto buildings = itr->second;
   auto match = std::find_if(buildings.begin(), buildings.end(), [building_type](const vic3::Building& b) {
      return b.GetType() == building_type;
   });
   if (match != buildings.end())
   {
      return *match;
   }
   return {};
}
