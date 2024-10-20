#ifndef SRC_HOI4WORLD_FOCUSTREES_FOCUSIMPORTER_H
#define SRC_HOI4WORLD_FOCUSTREES_FOCUSIMPORTER_H



#include <istream>
#include <optional>
#include <string>
#include <vector>

#include "external/commonItems/Parser.h"
#include "src/hoi4_world/focus_trees/focus.h"



namespace hoi4
{

class FocusImporter
{
  public:
   FocusImporter();

   [[nodiscard]] Focus ImportFocus(std::istream& input_stream);

  private:
   commonItems::parser focus_parser_;

   std::string id_;
   std::string icon_;
   std::optional<std::string> text_;
   std::vector<std::string> prerequisites_;
   std::optional<std::string> mutually_exclusive_;
   std::optional<std::string> bypass_;
   bool tree_starter_ = false;
   int x_position_ = 0;
   int y_position_ = 0;
   std::optional<std::string> relative_position_id_;
   int cost_ = 0;
   bool available_if_capitulated_ = false;
   std::optional<std::string> available_;
   std::optional<std::string> cancel_if_invalid_;
   std::optional<std::string> continue_if_invalid_;
   std::optional<std::string> will_lead_to_war_with_;
   std::optional<std::string> select_effect_;
   std::optional<std::string> complete_tooltip_;
   std::string completion_reward_;
   std::optional<std::string> ai_will_do_;
   std::optional<std::string> allow_branch_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_FOCUSTREES_FOCUSIMPORTER_H