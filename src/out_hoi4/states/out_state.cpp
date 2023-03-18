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
   state_history << fmt::format("\tmanpower = {}\n", state.GetManpower());
   state_history << "\n";
   state_history << fmt::format("\tstate_category = {}\n", state.GetCategory());
   if (state.GetCategory() == "wasteland")
   {
      state_history << "\timpassable = yes\n";
   }
   state_history << "\n";
   state_history << "\thistory = {\n";
   if (const auto& owner = state.GetOwner(); owner.has_value())
   {
      state_history << fmt::format("\t\towner = {}\n", *owner);
   }
   for (const auto& victory_point: state.GetVictoryPoints())
   {
      state_history << "\t\tvictory_points = {\n";
      state_history << fmt::format("\t\t\t{} {}\n", victory_point.first, victory_point.second);
      state_history << "\t\t}\n";
   }
   state_history << "\t\tbuildings = {\n";
   state_history << fmt::format("\t\t\tindustrial_complex = {}\n", state.GetCivilianFactories());
   state_history << fmt::format("\t\t\tarms_factory = {}\n", state.GetMilitaryFactories());
   if (state.GetDockyards() > 0)
   {
      state_history << fmt::format("\t\t\tdockyard = {}\n", state.GetDockyards());
   }
   if (state.GetNavalBaseLocation() && state.GetNavalBaseLevel())
   {
      state_history << fmt::format("\t\t\t{} = {{\n", *state.GetNavalBaseLocation());
      state_history << fmt::format("\t\t\t\tnaval_base = {}\n", *state.GetNavalBaseLevel());
      state_history << "\t\t\t}\n";
   }
   state_history << "\t\t}\n";
   state_history << "\t}\n";
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