#include "src/out_hoi4/states/out_state.h"

#include <fstream>

#include "external/fmt/include/fmt/format.h"



void out::OutputState(std::string_view output_name, const hoi4::State& state)
{
   const int state_number = state.GetId();

   const auto state_history_file_name = fmt::format("output/{}/history/states/{}.txt", output_name, state_number);
   std::ofstream state_history(state_history_file_name);
   if (!state_history.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", state_history_file_name));
   }


   state_history << "state = {\n";
   state_history << fmt::format("\tid = {}\n", state_number);
   state_history << fmt::format("\tname = \"STATE_{}\"\n", state_number);
   state_history << "\tmanpower = 1000\n";
   state_history << "\n";
   state_history << "\tstate_category = rural\n";
   state_history << "\n";
   state_history << "\tprovinces = {\n";
   state_history << "\t\t";
   for (const auto& province: state.GetProvinces())
   {
      state_history << province << " ";
   }
   state_history << "\n";
   state_history << "\t}\n";
   state_history << "}\n";

   state_history.close();
}