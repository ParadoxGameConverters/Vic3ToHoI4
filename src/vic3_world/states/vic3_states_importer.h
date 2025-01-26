#ifndef SRC_VIC3WORLD_STATES_VIC3STATESIMPORTER_H
#define SRC_VIC3WORLD_STATES_VIC3STATESIMPORTER_H



#include <external/commonItems/Parser.h>

#include <istream>
#include <map>

#include "src/vic3_world/states/vic3_state.h"
#include "src/vic3_world/states/vic3_state_importer.h"



namespace vic3
{

[[nodiscard]] std::map<int, State> ImportStates(std::istream& input_stream);

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_VIC3STATESIMPORTER_H