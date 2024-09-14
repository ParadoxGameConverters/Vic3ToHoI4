#ifndef SRC_VIC3WORLD_STATES_STATEREGIONS_H
#define SRC_VIC3WORLD_STATES_STATEREGIONS_H



#include <map>
#include <string>

#include "src/vic3_world/states/state_region.h"



namespace vic3
{

struct StateRegions
{
   std::map<std::string, StateRegion> name_to_region_map;
   std::map<std::string, int> region_indexes;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_STATEREGIONS_H