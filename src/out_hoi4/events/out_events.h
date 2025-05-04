#ifndef SRC_OUTHOI4_EVENTS_OUTEVENTS_H
#define SRC_OUTHOI4_EVENTS_OUTEVENTS_H



#include "src/hoi4_world/events/event.h"



namespace out
{

void OutputEvents(const std::filesystem::path& output_name,
    const std::map<std::string, std::vector<hoi4::Event>>& country_events);

}  // namespace out



#endif  // SRC_OUTHOI4_EVENTS_OUTEVENTS_H