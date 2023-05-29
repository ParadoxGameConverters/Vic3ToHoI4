#include "src/vic3_world/buildings/buildings.h"

#include <numeric>
#include <ranges>



namespace
{

bool IsIndustrialBuilding(const vic3::Building& building)
{
   const std::string& type = building.GetType();

   return type == "building_food_industry" || type == "building_textile_mills" ||
          type == "building_furniture_manufacturies" || type == "building_glassworks" ||
          type == "building_tooling_workshops" || type == "building_paper_mills" ||
          type == "building_chemical_plants" || type == "building_synthetics_plants" ||
          type == "building_steel_mills" || type == "building_motor_industry" || type == "building_shipyards" ||
          type == "building_war_machine_industry" || type == "building_electrics_industry" ||
          type == "building_arms_industry" || type == "building_munition_plants" || type == "building_power_plant" ||
          type == "building_port" || type == "building_construction_sector";
}

}  // namespace



float vic3::Buildings::GetTotalGoodSalesValueInState(int state_number) const
{
   if (const auto buildings_in_state = buildings_.find(state_number); buildings_in_state != buildings_.end())
   {
      return std::accumulate(buildings_in_state->second.begin(),
          buildings_in_state->second.end(),
          0.0F,
          [](float total_value, const vic3::Building& building) {
             return total_value + building.GetGoodsSalesVales();
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
             return total_value + building.GetGoodsSalesVales();
          });
   }

   return total_value;
}


float vic3::Buildings::GetTotalIndustryGoodSalesValueInState(int state_number) const
{
   if (const auto buildings_in_state = buildings_.find(state_number); buildings_in_state != buildings_.end())
   {
      return std::accumulate(buildings_in_state->second.begin(),
          buildings_in_state->second.end(),
          0.0F,
          [](float total_value, const vic3::Building& building) {
             if (IsIndustrialBuilding(building))
             {
                return total_value + building.GetGoodsSalesVales();
             }
             return total_value;
          });
   }

   return 0.0F;
}



float vic3::Buildings::GetTotalIndustryGoodSalesValueInWorld() const
{
   float total_value = 0.0F;

   for (const std::vector<Building>& buildings_in_state: buildings_ | std::views::values)
   {
      total_value += std::accumulate(buildings_in_state.begin(),
          buildings_in_state.end(),
          0.0F,
          [](float total_value, const vic3::Building& building) {
             if (IsIndustrialBuilding(building))
             {
                return total_value + building.GetGoodsSalesVales();
             }
             return total_value;
          });
   }

   return total_value;
}