#ifndef SRC_OUTHOI4_STATES_OUTSTATE_H
#define SRC_OUTHOI4_STATES_OUTSTATE_H



#include <string_view>

#include "src/hoi4_world/states/hoi4_state.h"



namespace out
{

void OutputState(std::string_view output_name, const hoi4::State& state);

}  // namespace out



#endif  // SRC_OUTHOI4_STATES_OUTSTATE_H