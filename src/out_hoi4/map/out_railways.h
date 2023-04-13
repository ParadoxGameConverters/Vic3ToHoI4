#ifndef OUTHOI4_MAP_OUTRAILWAYS_H
#define OUTHOI4_MAP_OUTRAILWAYS_H



#include <vector>

#include "src/hoi4_world/map/railway.h"



namespace out
{

void OutputRailways(std::string_view output_name, const std::vector<hoi4::Railway>& railways);

}  // namespace out



#endif  // OUTHOI4_MAP_OUTRAILWAYS_H