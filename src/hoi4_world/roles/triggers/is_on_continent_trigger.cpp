#include "src/hoi4_world/roles/triggers/is_on_continent_trigger.h"



namespace hoi4
{

bool IsOnContinentTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const StateScope* maybe_state = std::get_if<StateScope>(&context.this_scope);
   if (!maybe_state)
   {
      return false;
   }

   return maybe_state->state.GetContinent() == value_;
}


void PrintTo([[maybe_unused]] const IsOnContinentTrigger& trigger, std::ostream* os)
{
   *os << "IsOnContinentTrigger: " << trigger.value_ << "\n";
}

}  // namespace hoi4