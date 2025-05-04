#ifndef SRC_HOI4WORLD_EVENT_H
#define SRC_HOI4WORLD_EVENT_H



#include <optional>
#include <string>
#include <vector>

#include "src/hoi4_world/events/event_option.h"



namespace hoi4
{

struct Event
{
   std::string type;
   std::string id;
   std::optional<std::string> title;
   std::vector<std::string> descriptions;
   std::optional<std::string> picture;
   bool major_event = false;
   bool triggered_only = false;
   std::optional<bool> hidden;
   std::string trigger;
   std::optional<bool> fire_only_once;
   std::string mean_time_to_happen;
   std::string immediate;
   std::vector<EventOption> options;

   std::strong_ordering operator<=>(const Event&) const = default;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_EVENT_H