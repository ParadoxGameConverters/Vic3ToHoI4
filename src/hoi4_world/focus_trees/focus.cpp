#include "src/hoi4_world/focus_trees/focus.h"

#include "external/fmt/include/fmt/format.h"
#include "external/fmt/include/fmt/ranges.h"



namespace hoi4
{

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