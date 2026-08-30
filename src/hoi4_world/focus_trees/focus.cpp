#include "src/hoi4_world/focus_trees/focus.h"

#include <external/fmt/include/fmt/format.h>
#include <external/fmt/include/fmt/ranges.h>

#include <string>
#include <string_view>



namespace
{

void ReplaceText(std::string_view to_replace, std::string_view replace_with, std::string& to_update)
{
   while (to_update.find(to_replace) != std::string::npos)
   {
      to_update.replace(to_update.find(to_replace), to_replace.size(), replace_with);
   }
}

}  // namespace



namespace hoi4
{


void Focus::ApplyReplacement(std::string_view to_replace, std::string_view replace_with)
{
   ReplaceText(to_replace, replace_with, id);
   ReplaceText(to_replace, replace_with, icon);

   if (text.has_value())
   {
      ReplaceText(to_replace, replace_with, *text);
   }
   for (std::string& prerequisite: prerequisites)
   {
      ReplaceText(to_replace, replace_with, prerequisite);
   }
   if (mutually_exclusive.has_value())
   {
      ReplaceText(to_replace, replace_with, *mutually_exclusive);
   }
   if (bypass.has_value())
   {
      ReplaceText(to_replace, replace_with, *bypass);
   }
   if (relative_position_id.has_value())
   {
      ReplaceText(to_replace, replace_with, *relative_position_id);
   }
   if (available.has_value())
   {
      ReplaceText(to_replace, replace_with, *available);
   }
   if (cancel_if_invalid.has_value())
   {
      ReplaceText(to_replace, replace_with, *cancel_if_invalid);
   }
   if (continue_if_invalid.has_value())
   {
      ReplaceText(to_replace, replace_with, *continue_if_invalid);
   }
   if (will_lead_to_war_with.has_value())
   {
      ReplaceText(to_replace, replace_with, *will_lead_to_war_with);
   }
   if (select_effect.has_value())
   {
      ReplaceText(to_replace, replace_with, *select_effect);
   }
   if (complete_tooltip.has_value())
   {
      ReplaceText(to_replace, replace_with, *complete_tooltip);
   }
   ReplaceText(to_replace, replace_with, completion_reward);

   if (ai_will_do.has_value())
   {
      ReplaceText(to_replace, replace_with, *ai_will_do);
   }
   if (allow_branch.has_value())
   {
      ReplaceText(to_replace, replace_with, *allow_branch);
   }
}


void PrintTo(const Focus& focus, std::ostream* os)
{
   *os << "\n"
       << "id = " << focus.id << "\n"
       << "icon = " << focus.icon << "\n"
       << "text = " << focus.text.value_or("nullopt") << "\n"
       << fmt::format("prerequisites = {{ {} }}\n", fmt::join(focus.prerequisites, " "))
       << "mutually_exclusive = " << focus.mutually_exclusive.value_or("nullopt") << "\n"
       << "bypass = " << focus.bypass.value_or("nullopt") << "\n"
       << "tree_starter = " << (focus.tree_starter ? "true" : "false") << "\n"
       << "x_position = " << focus.x_position << "\n"
       << "y_position = " << focus.y_position << "\n"
       << "relative_position_id = " << focus.relative_position_id.value_or("nullopt") << "\n"
       << "cost = " << focus.cost << "\n"
       << "available_if_capitulated = " << focus.available_if_capitulated << "\n"
       << "available = " << focus.available.value_or("nullopt") << "\n"
       << "cancel_if_invalid = " << focus.cancel_if_invalid.value_or("nullopt") << "\n"
       << "continue_if_invalid = " << focus.continue_if_invalid.value_or("nullopt") << "\n"
       << "select_effect = " << focus.select_effect.value_or("nullopt") << "\n"
       << "complete_tooltip = " << focus.complete_tooltip.value_or("nullopt") << "\n"
       << "completion_reward = " << focus.completion_reward << "\n"
       << "ai_will_do = " << focus.ai_will_do.value_or("nullopt") << "\n"
       << "allow_branch = " << focus.allow_branch.value_or("nullopt") << "\n";
}

}  // namespace hoi4