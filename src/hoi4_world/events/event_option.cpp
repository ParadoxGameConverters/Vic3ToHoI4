#include "src/hoi4_world/events/event_option.h"



namespace hoi4
{

void PrintTo(const EventOption& option, std::ostream* os)
{
   *os << "Name: " << option.name << "\n";
   *os << "Trigger: " << option.trigger << "\n";
   *os << "AI chance: " << option.ai_chance << "\n";
   for (std::string_view script_block: option.script_blocks)
   {
      *os << "Script block: " << script_block << "\n";
   }
   *os << "Hidden effect: " << option.hidden_effect << "\n";
}

}  // namespace hoi4