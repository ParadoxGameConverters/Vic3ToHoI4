#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDINGS_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDINGS_H



#include <map>
#include <optional>
#include <vector>

#include "src/vic3_world/buildings/building.h"



namespace vic3
{

/// <summary>
/// Map from state to buildings
/// </summary>
class Buildings
{
  public:
   Buildings() = default;

   explicit Buildings(std::map<int, std::vector<Building>> buildings): buildings_(std::move(buildings)) {}

   [[nodiscard]] float GetTotalGoodSalesValueInState(int state_number) const;
   [[nodiscard]] float GetTotalGoodSalesValueInWorld() const;
   [[nodiscard]] std::map<int, std::vector<Building>> GetStorage() const { return buildings_; }
   [[nodiscard]] std::vector<Building> GetBuildingsInState(int state_number) const
   {
      if (buildings_.contains(state_number))
      {
         return buildings_.at(state_number);
      }
      return {};
   }


  private:
   std::map<int, std::vector<Building>> buildings_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_BUILDINGS_BUILDINGS_H
