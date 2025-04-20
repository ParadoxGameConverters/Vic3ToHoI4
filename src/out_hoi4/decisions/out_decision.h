#ifndef SRC_OUTHOI4_OUTDECISION_H
#define SRC_OUTHOI4_OUTDECISION_H



#include <ostream>

#include "src/hoi4_world/decisions/decision.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::Decision& decision);

}  // namespace out



#endif  // SRC_OUTHOI4_OUTDECISION_H