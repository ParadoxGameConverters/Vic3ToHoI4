#ifndef SRC_OUTHOI4_MAP_OUTSTRATEGICREGION_H
#define SRC_OUTHOI4_MAP_OUTSTRATEGICREGION_H



#include <string_view>

#include "src/hoi4_world/map/strategic_region.h"



namespace out
{

void OutputStrategicRegion(const std::filesystem::path& output_name, const hoi4::StrategicRegion& strategic_region);

}  // namespace out



#endif  // SRC_OUTHOI4_MAP_OUTSTRATEGICREGION_H