#ifndef SRC_VIC3WORLD_STATES_STATEREGIONS_H
#define SRC_VIC3WORLD_STATES_STATEREGIONS_H



#include <string>
#include <vector>

#include "src/vic3_world/states/state_region.h"



namespace vic3
{

struct StateRegions
{
   std::map<std::string, StateRegion> name_to_region_map;
   std::vector<std::string> regions_in_order;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_STATEREGIONS_H