#ifndef SRC_HOI4WORLD_MAP_BUILDING_H
#define SRC_HOI4WORLD_MAP_BUILDING_H



#include <optional>
#include <string>

#include "src/hoi4_world/map/building_position.h"



namespace hoi4
{

struct Building
{
   int state_id = 0;
   std::string type;
   BuildingPosition position;
   std::optional<int> connecting_sea_province;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_BUILDING_H
