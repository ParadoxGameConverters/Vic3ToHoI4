#ifndef SRC_OUTHOI4_MAP_OUTBUILDINGS_H
#define SRC_OUTHOI4_MAP_OUTBUILDINGS_H



#include <filesystem>

#include "src/hoi4_world/map/buildings.h"



namespace out
{

void OutputBuildings(const std::filesystem::path& output_name, const hoi4::Buildings& buildings);

}  // namespace out



#endif  // SRC_OUTHOI4_MAP_OUTBUILDINGS_H