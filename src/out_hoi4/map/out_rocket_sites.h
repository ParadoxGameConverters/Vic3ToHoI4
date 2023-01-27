#ifndef SRC_OUTHOI4_MAP_OUTROCKETSITES_H
#define SRC_OUTHOI4_MAP_OUTROCKETSITES_H



#include <string_view>
#include <vector>

#include "src/hoi4_world/states/hoi4_state.h"



namespace out
{

void OutputRocketSites(std::string_view output_name, const std::vector<hoi4::State>& states);

}



#endif  // SRC_OUTHOI4_MAP_OUTROCKETSITES_H