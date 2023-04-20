#ifndef SRC_HOI4WORLD_MAP_RAILWAYS_H
#define SRC_HOI4WORLD_MAP_RAILWAYS_H



#include <map>
#include <vector>

#include "src/hoi4_world/map/railway.h"



namespace hoi4
{

struct Railways
{
   std::vector<Railway> railways;
   std::set<int> railway_endpoints;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_RAILWAYS_H