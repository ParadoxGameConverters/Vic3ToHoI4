#ifndef SRC_HOI4WORLD_EVENT_OPTION_H
#define SRC_HOI4WORLD_EVENT_OPTION_H


#include <external/commonItems/Parser.h>

#include <string>
#include <vector>



namespace hoi4
{

struct EventOption
{
   std::string name;
   std::string trigger;
   std::string ai_chance;
   std::vector<std::string> script_blocks;
   std::string hidden_effect;

   std::strong_ordering operator<=>(const EventOption&) const = default;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_EVENT_OPTION_H