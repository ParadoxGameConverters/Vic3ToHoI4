#ifndef SRC_VIC3WORLD_BUILDINGS_BUILDINGS_H
#define SRC_VIC3WORLD_BUILDINGS_BUILDINGS_H



#include <map>
#include <vector>

#include "src/vic3_world/buildings/building.h"



namespace vic3
{

class Buildings
{
  public:
   Buildings() = default;

   explicit Buildings(std::map<int, std::vector<Building>> buildings): buildings_(std::move(buildings)) {}

   [[nodiscard]] float GetTotalGoodSalesValueInState(int state_number) const;
   [[nodiscard]] float GetTotalGoodSalesValueInWorld() const;

  private:
   std::map<int, std::vector<Building>> buildings_;
};


}  // namespace vic3



#endif  // SRC_VIC3WORLD_BUILDINGS_BUILDINGS_H