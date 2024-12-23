#ifndef HOI4_STATES_DEFAULT_STATES_IMPORTER_H
#define HOI4_STATES_DEFAULT_STATES_IMPORTER_H



#include <external/commonItems/ModLoader/ModFilesystem.h>

#include <filesystem>
#include <map>

#include "src/hoi4_world/states/default_state.h"



namespace hoi4
{

std::map<int, DefaultState> ImportDefaultStates(const commonItems::ModFilesystem& mod_filesystem);

}  // namespace hoi4



#endif  // HOI4_STATES_DEFAULT_STATES_IMPORTER_H