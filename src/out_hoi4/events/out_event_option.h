#ifndef SRC_OUTHOI4_EVENTS_OUTEVENTOPTION_H
#define SRC_OUTHOI4_EVENTS_OUTEVENTOPTION_H



#include <ostream>

#include "src/hoi4_world/events/event_option.h"



namespace hoi4
{

std::ostream& operator<<(std::ostream& out, const EventOption& option);

}



#endif  // SRC_OUTHOI4_EVENTS_OUTEVENTOPTION_H