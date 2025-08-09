#include "src/hoi4_world/roles/triggers/is_capital_trigger.h"



namespace hoi4
{

bool IsCapitalTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const StateScope* maybe_state = std::get_if<StateScope>(&context.this_scope);
   if (!maybe_state)
   {
      return false;
   }

   return maybe_state->state.IsCapital() == value_;
}


void PrintTo([[maybe_unused]] const IsCapitalTrigger& trigger, std::ostream* os)
{
   *os << "IsCapitalTrigger: " << trigger.value_ << "\n";
}

}  // namespace hoi4