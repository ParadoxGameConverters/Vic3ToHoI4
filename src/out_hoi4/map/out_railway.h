#ifndef OUTHOI4_MAP_OUTRAILWAY_H
#define OUTHOI4_MAP_OUTRAILWAY_H



#include <ostream>

#include "src/hoi4_world/map/railway.h"



namespace hoi4
{

std::ostream& operator<<(std::ostream& out, const Railway& railway);

}  // namespace hoi4



#endif  // OUTHOI4_MAP_OUTRAILWAY_H