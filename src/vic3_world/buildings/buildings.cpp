#include "src/vic3_world/buildings/buildings.h"

#include <ranges>



float vic3::Buildings::GetTotalGoodSalesValueInState(int state_number) const
{
   float total_value = 0.0F;

   if (const auto buildings_in_state = buildings_.find(state_number); buildings_in_state != buildings_.end())
   {
      for (const Building& building: buildings_in_state->second)
      {
         total_value += building.GetGoodsSalesVales();
      }
   }

   return total_value;
}



float vic3::Buildings::GetTotalGoodSalesValueInWorld() const
{
   float total_value = 0.0F;

   for (const std::vector<Building>& buildings_in_state: buildings_ | std::views::values)
   {
      for (const Building& building: buildings_in_state)
      {
         total_value += building.GetGoodsSalesVales();
      }
   }

   return total_value;
}