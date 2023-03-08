#ifndef SRC_VIC3WORLD_STATES_STATEREGIONSIMPORTER_H
#define SRC_VIC3WORLD_STATES_STATEREGIONSIMPORTER_H



#include <map>
#include <string>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/vic3_world/states/state_region.h"



namespace vic3
{

std::map<std::string, StateRegion> ImportStateRegions(const commonItems::ModFilesystem filesystem);

}



#endif  // SRC_VIC3WORLD_STATES_STATEREGIONSIMPORTER_H