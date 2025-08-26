#include "src/hoi4_world/roles/triggers/and_trigger.h"

#include <algorithm>



namespace hoi4
{

bool AndTrigger::IsValid(const Context& context, const World& world) const
{
   return std::ranges::all_of(children_.begin(), children_.end(), [context, &world](const std::unique_ptr<Trigger>& a) {
      return a->IsValid(context, world);
   });
}


bool AndTrigger::operator==(const AndTrigger& rhs) const
{
   if (children_.size() != rhs.children_.size())
   {
      return false;
   }

   for (unsigned i = 0; i < children_.size(); ++i)
   {
      if (*children_[i] != *rhs.children_[i])
      {
         return false;
      }
   }

   return true;
}


bool AndTrigger::operator<(const AndTrigger& rhs) const
{
   if (children_.size() < rhs.children_.size())
   {
      return true;
   }
   if (children_.size() > rhs.children_.size())
   {
      return false;
   }

   for (unsigned i = 0; i < children_.size(); ++i)
   {
      if (*children_[i] < *rhs.children_[i])
      {
         return true;
      }
      if (*children_[i] != *rhs.children_[i])
      {
         return false;
      }
   }

   return true;
}


std::unique_ptr<Trigger> AndTrigger::Copy() const
{
   std::vector<std::unique_ptr<Trigger>> children;
   for (const auto& child: children_)
   {
      children.push_back(child->Copy());
   }

   return std::make_unique<AndTrigger>(std::move(children));
}


void PrintTo(const AndTrigger& trigger, std::ostream* os)
{
   *os << "AndTrigger: {\n";
   for (const std::unique_ptr<Trigger>& child: trigger.children_)
   {
      *os << child;
   }
   *os << "}\n";
}

}  // namespace hoi4