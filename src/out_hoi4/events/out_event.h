#ifndef SRC_OUTHOI4_EVENTS_OUTEVENT_H
#define SRC_OUTHOI4_EVENTS_OUTEVENT_H



#include <ostream>

#include "src/hoi4_world/events/event.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::Event& event);

}  // namespace out



#endif  // SRC_OUTHOI4_EVENTS_OUTEVENT_H