#include "src/hoi4_world/roles/triggers/is_homeland_trigger.h"



namespace hoi4
{

bool IsHomelandTrigger::IsValid(const Context& context, [[maybe_unused]] const World& world) const
{
   const StateScope* maybe_state = std::get_if<StateScope>(&context.this_scope);
   if (!maybe_state)
   {
      return false;
   }

   return maybe_state->state.HasHomeland(culture_name_);
}


void PrintTo([[maybe_unused]] const IsHomelandTrigger& trigger, std::ostream* os)
{
   *os << "IsHomelandTrigger: " << trigger.culture_name_ << "\n";
}

}  // namespace hoi4