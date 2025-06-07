#include "src/hoi4_world/roles/requirements/or_trigger.h"

#include <algorithm>



namespace hoi4
{

bool OrTrigger::IsValid(const Scope& scope, const World& world) const
{
   return std::ranges::any_of(children_.begin(), children_.end(), [scope, world](const std::unique_ptr<Trigger>& a) {
      return a->IsValid(scope, world);
   });
}


bool OrTrigger::operator==(const OrTrigger& rhs) const
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


bool OrTrigger::operator<(const OrTrigger& rhs) const
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

}  // namespace hoi4