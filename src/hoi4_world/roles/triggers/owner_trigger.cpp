#include "src/hoi4_world/roles/triggers/owner_trigger.h"



namespace hoi4
{

bool OwnerTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const StateScope* maybe_state = std::get_if<StateScope>(&context.this_scope);
   if (!maybe_state)
   {
      return false;
   }

   if (owner_ == "ROOT")
   {
      const CountryScope* maybe_root = std::get_if<CountryScope>(&context.root);
      if (!maybe_root)
      {
         return false;
      }
      return maybe_root->country.GetTag() == maybe_state->state.GetOwner();
   }

   return maybe_state->state.GetOwner() == owner_;
}


void PrintTo([[maybe_unused]] const OwnerTrigger& trigger, std::ostream* os)
{
   *os << "OwnerTrigger: " << trigger.owner_ << "\n";
}

}  // namespace hoi4