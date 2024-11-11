#ifndef SRC_HOI4WORLD_FOCUSTREES_FOCUS_H
#define SRC_HOI4WORLD_FOCUSTREES_FOCUS_H



#include <optional>
#include <ostream>
#include <string>
#include <vector>



namespace hoi4
{

class Focus
{
  public:
   std::string id;
   std::string icon;
   std::optional<std::string> text;
   std::vector<std::string> prerequisites;
   std::optional<std::string> mutually_exclusive;
   std::optional<std::string> bypass;
   bool tree_starter = false;
   int x_position = 0;
   int y_position = 0;
   std::optional<std::string> relative_position_id;
   int cost = 0;
   bool available_if_capitulated = false;
   std::optional<std::string> available;
   std::optional<std::string> cancel_if_invalid;
   std::optional<std::string> continue_if_invalid;
   std::optional<std::string> will_lead_to_war_with;
   std::optional<std::string> select_effect;
   std::optional<std::string> complete_tooltip;
   std::string completion_reward;
   std::optional<std::string> ai_will_do;
   std::optional<std::string> allow_branch;

   std::strong_ordering operator<=>(const Focus&) const = default;

   // replaces text in all applicable focus items
   void apply_replacement(std::string_view to_replace, std::string_view replace_with);

   // This allows the Google test framework to print human-readable focuses if a test fails.
   friend void PrintTo(const Focus& focus, std::ostream* os);
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_FOCUSTREES_FOCUS_H