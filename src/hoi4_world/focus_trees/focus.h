#ifndef SRC_HOI4WORLD_FOCUSTREES_FOCUS_H
#define SRC_HOI4WORLD_FOCUSTREES_FOCUS_H



#include <optional>
#include <string>
#include <vector>



namespace hoi4
{

struct Focus
{
   std::string id;
   std::string icon;
   std::optional<std::string> text;
   std::vector<std::string> prerequisites;
   std::optional<std::string> mutually_exclusive;
   std::optional<std::string> bypass;
   int x_position = 0;
   int y_position = 0;
   std::optional<std::string> relative_position_id;
   int cost = 0;
   bool available_if_capitulated = false;
   std::optional<std::string> available;
   std::optional<std::string> cancel_if_invalid;
   std::optional<std::string> continue_if_invalid;
   std::optional<std::string> select_effect;
   std::optional<std::string> complete_tooltip;
   std::string completion_reward;
   std::optional<std::string> ai_will_do;
   std::optional<std::string> allow_branch;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_FOCUSTREES_FOCUS_H