#ifndef SRC_VIC3WORLD_STATES_STATEREGIONSIMPORTER_H
#define SRC_VIC3WORLD_STATES_STATEREGIONSIMPORTER_H



#include <map>
#include <string>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/vic3_world/states/state_regions.h"



namespace vic3
{

StateRegions ImportStateRegions(const commonItems::ModFilesystem& filesystem);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_STATEREGIONSIMPORTER_H