#ifndef SRC_OUTHOI4_EVENTS_OUTEVENTOPTION_H
#define SRC_OUTHOI4_EVENTS_OUTEVENTOPTION_H



#include <ostream>

#include "src/hoi4_world/events/event_option.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::EventOption& option);

}



#endif  // SRC_OUTHOI4_EVENTS_OUTEVENTOPTION_H