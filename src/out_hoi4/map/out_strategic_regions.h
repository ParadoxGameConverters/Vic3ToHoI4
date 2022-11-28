#ifndef SRC_OUTHOI4_MAP_OUTSTRATEGICREGIONS_H
#define SRC_OUTHOI4_MAP_OUTSTRATEGICREGIONS_H



#include <string_view>

#include "src/hoi4_world/map/strategic_regions.h"



namespace out
{

void OutputStrategicRegions(std::string_view output_name, const hoi4::StrategicRegions& strategic_regions);

}



#endif  // SRC_OUTHOI4_MAP_OUTSTRATEGICREGIONS_H