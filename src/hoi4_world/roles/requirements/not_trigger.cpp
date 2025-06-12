#include "src/hoi4_world/roles/requirements/not_trigger.h"

#include <algorithm>



namespace hoi4
{

bool NotTrigger::IsValid(const Scope& scope, const World& world) const
{
   return std::ranges::none_of(children_.begin(), children_.end(), [scope, world](const std::unique_ptr<Trigger>& a) {
      return a->IsValid(scope, world);
   });
}


bool NotTrigger::operator==(const NotTrigger& rhs) const
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


bool NotTrigger::operator<(const NotTrigger& rhs) const
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


std::unique_ptr<Trigger> NotTrigger::Copy() const
{
   std::vector<std::unique_ptr<Trigger>> children;
   for (const auto& child: children_)
   {
      children.push_back(child->Copy());
   }

   std::unique_ptr<Trigger> copy = std::make_unique<NotTrigger>(std::move(children));
   return std::move(copy);
}

}  // namespace hoi4