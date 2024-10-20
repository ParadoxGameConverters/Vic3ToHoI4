#include "src/hoi4_world/focus_trees/focus.h"

#include "external/fmt/include/fmt/format.h"
#include "external/fmt/include/fmt/ranges.h"



namespace hoi4
{


void Focus::apply_replacement(std::string_view to_replace, std::string_view replace_with)
{
   while (id.find(to_replace) != std::string::npos)
   {
      id.replace(id.find(to_replace), to_replace.size(), replace_with);
   }
   while (icon.find(to_replace) != std::string::npos)
   {
      icon.replace(icon.find(to_replace), to_replace.size(), replace_with);
   }
   if (text.has_value())
   {
      while (text->find(to_replace) != std::string::npos)
      {
         text->replace(text->find(to_replace), to_replace.size(), replace_with);
      }
   }
   for (std::string& prerequisite: prerequisites)
   {
      while (prerequisite.find(to_replace) != std::string::npos)
      {
         prerequisite.replace(prerequisite.find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (mutually_exclusive.has_value())
   {
      while (mutually_exclusive->find(to_replace) != std::string::npos)
      {
         mutually_exclusive->replace(mutually_exclusive->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (bypass.has_value())
   {
      while (bypass->find(to_replace) != std::string::npos)
      {
         bypass->replace(bypass->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (relative_position_id.has_value())
   {
      while (relative_position_id->find(to_replace) != std::string::npos)
      {
         relative_position_id->replace(relative_position_id->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (available.has_value())
   {
      while (available->find(to_replace) != std::string::npos)
      {
         available->replace(available->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (cancel_if_invalid.has_value())
   {
      while (cancel_if_invalid->find(to_replace) != std::string::npos)
      {
         cancel_if_invalid->replace(cancel_if_invalid->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (continue_if_invalid.has_value())
   {
      while (continue_if_invalid->find(to_replace) != std::string::npos)
      {
         continue_if_invalid->replace(continue_if_invalid->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (will_lead_to_war_with.has_value())
   {
      while (will_lead_to_war_with->find(to_replace) != std::string::npos)
      {
         will_lead_to_war_with->replace(will_lead_to_war_with->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (select_effect.has_value())
   {
      while (select_effect->find(to_replace) != std::string::npos)
      {
         select_effect->replace(select_effect->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (complete_tooltip.has_value())
   {
      while (complete_tooltip->find(to_replace) != std::string::npos)
      {
         complete_tooltip->replace(complete_tooltip->find(to_replace), to_replace.size(), replace_with);
      }
   }
   while (completion_reward.find(to_replace) != std::string::npos)
   {
      completion_reward.replace(completion_reward.find(to_replace), to_replace.size(), replace_with);
   }
   if (ai_will_do.has_value())
   {
      while (ai_will_do->find(to_replace) != std::string::npos)
      {
         ai_will_do->replace(ai_will_do->find(to_replace), to_replace.size(), replace_with);
      }
   }
   if (allow_branch.has_value())
   {
      while (allow_branch->find(to_replace) != std::string::npos)
      {
         allow_branch->replace(allow_branch->find(to_replace), to_replace.size(), replace_with);
      }
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