#ifndef SRC_OUTHOI4_EVENTS_OUTEVENT_H
#define SRC_OUTHOI4_EVENTS_OUTEVENT_H



#include <ostream>

#include "src/hoi4_world/events/event.h"



namespace hoi4
{

std::ostream& operator<<(std::ostream& out, const Event& event);

}  // namespace hoi4



#endif  // SRC_OUTHOI4_EVENTS_OUTEVENT_H