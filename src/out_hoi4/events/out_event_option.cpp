#include "src/out_hoi4/events/out_event_option.h"



namespace out
{

std::ostream& operator<<(std::ostream& out, const hoi4::EventOption& option)
{
   out << "\toption = {\n";
   if (!option.name.empty())
   {
      out << "\t\tname = " << option.name << "\n";
   }
   if (!option.trigger.empty())
   {
      out << "\t\ttrigger " << option.trigger << "\n";
   }
   if (!option.ai_chance.empty())
   {
      out << "\t\tai_chance " << option.ai_chance << "\n";
   }
   for (const auto& scriptBlock: option.script_blocks)
   {
      out << "\t\t" << scriptBlock << "\n";
   }
   if (!option.hidden_effect.empty())
   {
      out << "\t\thidden_effect " << option.hidden_effect << "\n";
   }
   out << "\t}";

   return out;
}

}  // namespace out