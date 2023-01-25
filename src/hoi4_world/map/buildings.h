#ifndef SRC_HOI4WORLD_MAP_BUILDINGS_H
#define SRC_HOI4WORLD_MAP_BUILDINGS_H



#include <map>
#include <vector>

#include "src/hoi4_world/map/hoi4_building.h"



namespace hoi4
{

class Buildings
{
  public:
   Buildings(std::vector<Building> buildings, std::map<int, int> airport_locations):
       buildings_(std::move(buildings)),
       airport_locations_(std::move(airport_locations))
   {
   }

   [[nodiscard]] const std::vector<Building>& GetBuildings() const { return buildings_; }
   [[nodiscard]] const std::map<int, int>& GetAirportLocations() const { return airport_locations_; }

  private:
   std::vector<Building> buildings_;
   std::map<int, int> airport_locations_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_BUILDINGS_H