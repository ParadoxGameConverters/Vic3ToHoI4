#include "src/hoi4_world/roles/triggers/has_homeland_trigger.h"



namespace hoi4
{

bool HasHomelandTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const StateScope* maybe_state = std::get_if<StateScope>(&context.this_scope);
   if (!maybe_state)
   {
      return false;
   }

   return maybe_state->state.HasHomeland(value_);
}


void PrintTo([[maybe_unused]] const HasHomelandTrigger& trigger, std::ostream* os)
{
   *os << "HasHomelandTrigger: " << trigger.value_ << "\n";
}

}  // namespace hoi4