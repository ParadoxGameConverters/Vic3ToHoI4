#include "src/hoi4_world/roles/triggers/has_role_trigger.h"



namespace hoi4
{

bool HasRoleTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const CountryScope* maybe_country = std::get_if<CountryScope>(&context.this_scope);
   if (!maybe_country)
   {
      return false;
   }

   return maybe_country->country.HasRole(role_);
}


std::unique_ptr<Trigger> HasRoleTrigger::Copy() const
{
   return std::make_unique<HasRoleTrigger>(role_);
}


void PrintTo(const HasRoleTrigger& trigger, std::ostream* os)
{
   *os << "HasRoleTrigger: " << trigger.role_ << "\n";
}

}  // namespace hoi4