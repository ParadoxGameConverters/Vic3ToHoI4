#include "src/hoi4_world/roles/requirements/tag_trigger.h"



namespace hoi4
{

bool TagTrigger::IsValid(const Scope& scope, [[maybe_unused]] const World& world) const
{
   const CountryScope* maybe_country = std::get_if<CountryScope>(&scope);
   if (!maybe_country)
   {
      return false;
   }

   return maybe_country->country.GetTag() == tag_;
}


std::unique_ptr<Trigger> TagTrigger::Copy() const
{
   return std::make_unique<TagTrigger>(tag_);
}

}  // namespace hoi4