#include "src/hoi4_world/roles/repeat_focus.h"

#include "src/hoi4_world/roles/triggers/trigger_base.h"



namespace hoi4
{

RepeatFocus& RepeatFocus::operator=(const RepeatFocus& other)
{
   trigger_ = other.trigger_->Copy();
   focuses_ = other.focuses_;
   return *this;
}


RepeatFocus& RepeatFocus::operator=(RepeatFocus&& other)
{
   trigger_ = std::move(other.trigger_);
   focuses_ = std::move(other.focuses_);

   return *this;
}


std::strong_ordering RepeatFocus::operator<=>(const RepeatFocus& other) const
{
   if (!trigger_ && other.trigger_)
   {
      return std::strong_ordering::less;
   }
   if (trigger_ && !other.trigger_)
   {
      return std::strong_ordering::greater;
   }
   if (*trigger_ != *other.trigger_)
   {
      if (*trigger_ < *other.trigger_)
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
   if (repeat_focus.trigger_)
   {
      PrintTo(*repeat_focus.trigger_, os);
   }
   else
   {
      *os << "no requirement\n";
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