#ifndef SRC_OUTHOI4_MAP_OUTBUILDINGS_H
#define SRC_OUTHOI4_MAP_OUTBUILDINGS_H



#include "src/hoi4_world/map/buildings.h"



namespace out
{

void OutputBuildings(std::string_view output_name, const hoi4::Buildings& buildings);

}



#endif  // SRC_OUTHOI4_MAP_OUTBUILDINGS_H