#include "src/out_hoi4/decisions/out_decision.h"



std::ostream& out::operator<<(std::ostream& out, const hoi4::Decision& decision)
{
   out << "\n\t" << decision.name << " = {\n";
   if (!decision.icon.empty())
   {
      out << "\n\t\ticon = " << decision.icon << "\n";
   }
   if (!decision.name_field.empty())
   {
      out << "\n\t\tname = " << decision.name_field << "\n";
   }
   if (!decision.is_good.empty())
   {
      out << "\n\t\tis_good = " << decision.is_good << "\n";
   }
   if (!decision.allowed.empty())
   {
      out << "\n\t\tallowed " << decision.allowed << "\n";
   }
   if (!decision.highlight_state_targets.empty())
   {
      out << "\n\t\thighlight_states " << decision.highlight_state_targets << "\n";
   }
   if (!decision.available.empty())
   {
      out << "\n\t\tavailable " << decision.available << "\n";
   }
   if (decision.days_mission_timeout)
   {
      out << "\n\t\tdays_mission_timeout = " << *decision.days_mission_timeout << "\n";
   }
   if (!decision.activation.empty())
   {
      out << "\n\t\tactivation " << decision.activation << "\n";
   }
   if (!decision.targets.empty())
   {
      out << "\n\t\ttargets " << decision.targets << "\n";
   }
   if (decision.target_non_existing)
   {
      out << "\n\t\ttarget_non_existing = yes\n";
   }
   if (!decision.target_array.empty())
   {
      out << "\n\t\ttarget_array " << decision.target_array << "\n";
   }
   if (!decision.target_root_trigger.empty())
   {
      out << "\n\t\ttarget_root_trigger " << decision.target_root_trigger << "\n";
   }
   if (!decision.state_target.empty())
   {
      out << "\n\t\tstate_target = " << decision.state_target << "\n";
   }
   if (!decision.target_trigger.empty())
   {
      out << "\n\t\ttarget_trigger " << decision.target_trigger << "\n";
   }
   if (!decision.visible.empty())
   {
      out << "\n\t\tvisible " << decision.visible << "\n";
   }
   if (!decision.cancel_trigger.empty())
   {
      out << "\n\t\tcancel_trigger " << decision.cancel_trigger << "\n";
   }
   if (!decision.remove_trigger.empty())
   {
      out << "\n\t\tremove_trigger " << decision.remove_trigger << "\n";
   }
   if (!decision.custom_cost_trigger.empty())
   {
      out << "\n\t\tcustom_cost_trigger " << decision.custom_cost_trigger << "\n";
   }
   if (!decision.custom_cost_text.empty())
   {
      out << "\n\t\tcustom_cost_text " << decision.custom_cost_text << "\n";
   }
   if (!decision.fire_only_once.empty())
   {
      out << "\n\t\tfire_only_once = " << decision.fire_only_once << "\n";
   }
   if (!decision.modifier.empty())
   {
      out << "\n\t\tmodifier " << decision.modifier << "\n";
   }
   if (decision.days_remove)
   {
      out << "\n\t\tdays_remove = " << *decision.days_remove << "\n";
   }
   if (decision.days_reenable)
   {
      out << "\t\tdays_re_enable = " << *decision.days_reenable << "\n";
   }
   if (decision.cost)
   {
      out << "\n\t\tcost = " << *decision.cost << "\n";
   }
   if (!decision.on_map_mode.empty())
   {
      out << "\n\t\ton_map_mode = " << decision.on_map_mode << "\n";
   }
   if (!decision.complete_effect.empty())
   {
      out << "\n\t\tcomplete_effect " << decision.complete_effect << "\n";
   }
   if (!decision.targeted_modifier.empty())
   {
      out << "\n\t\ttargeted_modifier " << decision.targeted_modifier << "\n";
   }
   if (!decision.remove_effect.empty())
   {
      out << "\n\t\tremove_effect " << decision.remove_effect << "\n";
   }
   if (!decision.timeout_effect.empty())
   {
      out << "\n\t\ttimeout_effect " << decision.timeout_effect << "\n";
   }
   if (!decision.ai_will_do.empty())
   {
      out << "\n\t\tai_will_do " << decision.ai_will_do << "\n";
   }

   out << "\t}";

   return out;
}