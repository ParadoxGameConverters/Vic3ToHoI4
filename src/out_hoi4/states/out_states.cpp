#include "src/out_hoi4/states/out_states.h"

#include "src/out_hoi4/states/out_state.h"



void out::OutputStates(const std::filesystem::path& output_name, const std::vector<hoi4::State>& states)
{
   for (const auto& state: states)
   {
      OutputState(output_name, state);
   }
}