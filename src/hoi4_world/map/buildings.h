#ifndef SRC_HOI4WORLD_MAP_BUILDINGS_H
#define SRC_HOI4WORLD_MAP_BUILDINGS_H



#include <map>
#include <vector>

#include "src/hoi4_world/map/hoi4_building.h"



namespace hoi4
{

struct BuildingsOptions
{
   std::vector<Building> buildings;
   std::map<int, int> airport_locations;
};


class Buildings
{
  public:
   Buildings() = default;
   explicit Buildings(BuildingsOptions options): buildings_(std::move(options.buildings)) {}

   [[nodiscard]] const std::vector<Building>& GetBuildings() const { return buildings_; }

  private:
   std::vector<Building> buildings_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_BUILDINGS_H