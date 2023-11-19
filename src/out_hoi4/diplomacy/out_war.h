#ifndef OUTHOI4_DIPLOMACY_OUTWAR_H
#define OUTHOI4_DIPLOMACY_OUTWAR_H


#include <ostream>

#include "src/hoi4_world/diplomacy/hoi4_war.h"


namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::War& war);

}  // namespace out



#endif  // OUTHOI4_DIPLOMACY_OUTWAR_H