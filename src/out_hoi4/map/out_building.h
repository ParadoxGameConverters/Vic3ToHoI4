#ifndef SRC_OUTHOI4_MAP_OUTBUILDING_H
#define SRC_OUTHOI4_MAP_OUTBUILDING_H


#include <ostream>

#include "src/hoi4_world/map/hoi4_building.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::Building& building);

}



#endif  // SRC_OUTHOI4_MAP_OUTBUILDING_H