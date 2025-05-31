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

}  // namespace hoi4