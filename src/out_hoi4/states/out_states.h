#ifndef SRC_OUTHOI4_STATES_OUTSTATES_H
#define SRC_OUTHOI4_STATES_OUTSTATES_H



#include <string_view>
#include <vector>

#include "src/hoi4_world/states/hoi4_state.h"



namespace out
{

void OutputStates(std::string_view output_name, const std::vector<hoi4::State>& states);

}  // namespace out



#endif  // SRC_OUTHOI4_STATES_OUTSTATES_H