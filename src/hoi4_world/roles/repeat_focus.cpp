#include "src/hoi4_world/roles/repeat_focus.h"

#include "src/hoi4_world/roles/requirements/trigger_base.h"



namespace hoi4
{

std::strong_ordering RepeatFocus::operator<=>(const RepeatFocus& other) const
{
   if (!requirement_ && other.requirement_)
   {
      return std::strong_ordering::less;
   }
   if (requirement_ && !other.requirement_)
   {
      return std::strong_ordering::greater;
   }
   if (*requirement_ != *other.requirement_)
   {
      if (*requirement_ < *other.requirement_)
      {
         return std::strong_ordering::less;
      }
      return std::strong_ordering::greater;
   }

   return focuses_ <=> other.focuses_;
}


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