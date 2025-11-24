#include "src/out_hoi4/focus_trees/out_focus.h"



std::ostream& out::operator<<(std::ostream& output, const hoi4::Focus& focus)
{
   output << "\tfocus = {\n";
   output << "\t\tid = " << focus.id << "\n";
   output << "\t\ticon = " << focus.icon << "\n";
   if (focus.text.has_value())
   {
      output << "\t\ttext = " << *focus.text << "\n";
   }
   for (const std::string& prerequisite: focus.prerequisites)
   {
      output << "\t\tprerequisite " << prerequisite << "\n";
   }
   if (focus.mutually_exclusive.has_value())
   {
      output << "\t\tmutually_exclusive " << *focus.mutually_exclusive << "\n";
   }
   if (focus.bypass.has_value())
   {
      output << "\t\tbypass " << *focus.bypass << "\n";
   }
   if (focus.will_lead_to_war_with.has_value())
   {
      output << "\t\twill_lead_to_war_with = " << *focus.will_lead_to_war_with << "\n";
   }
   output << "\t\tx = " << focus.x_position << "\n";
   output << "\t\ty = " << focus.y_position << "\n";
   if (focus.relative_position_id.has_value())
   {
      output << "\t\trelative_position_id = " << *focus.relative_position_id << "\n";
   }
   output << "\t\tcost = " << focus.cost << "\n";
   if (focus.available_if_capitulated)
   {
      output << "\t\tavailable_if_capitulated = yes\n";
   }
   if (focus.available.has_value())
   {
      output << "\t\tavailable " << *focus.available << "\n";
   }
   if (focus.cancel_if_invalid.has_value())
   {
      output << "\t\tcancel_if_invalid = " << *focus.cancel_if_invalid << "\n";
   }
   if (focus.continue_if_invalid.has_value())
   {
      output << "\t\tcontinue_if_invalid = " << *focus.continue_if_invalid << "\n";
   }
   if (focus.select_effect.has_value())
   {
      output << "\t\tselect_effect " << *focus.select_effect << "\n";
   }
   if (focus.complete_tooltip.has_value())
   {
      output << "\t\tcomplete_tooltip " << *focus.complete_tooltip << "\n";
   }
   output << "\t\tcompletion_reward " << focus.completion_reward << "\n";
   if (focus.ai_will_do.has_value())
   {
      output << "\t\tai_will_do " << *focus.ai_will_do << "\n";
   }
   if (focus.allow_branch.has_value())
   {
      output << "\t\tallow_branch " << *focus.allow_branch << "\n";
   }

   output << "\t}\n";

   return output;
}
