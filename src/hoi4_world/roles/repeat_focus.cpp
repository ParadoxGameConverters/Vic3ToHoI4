#include "src/hoi4_world/roles/repeat_focus.h"

#include "src/hoi4_world/roles/requirements/trigger_base.h"



namespace hoi4
{

void PrintTo(const RepeatFocus& repeat_focus, std::ostream* os)
{
   *os << "\n";
   *os << "Requirement: ";
   if (repeat_focus.requirement_)
   {
      PrintTo(*repeat_focus.requirement_, os);
   }
   else
   {
      *os << "nullptr\n";
   }
   *os << "{\n";
   for (const Focus& focus: repeat_focus.focuses_)
   {
      *os << "\tFocus: ";
      PrintTo(focus, os);
      *os << "\n";
   }
   *os << "}\n\n";
}

}  // namespace hoi4