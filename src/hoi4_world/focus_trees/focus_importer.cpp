#include "src/hoi4_world/focus_trees/focus_importer.h"

#include <external/commonItems/ParserHelpers.h>



namespace hoi4
{

FocusImporter::FocusImporter()
{
   focus_parser_.registerKeyword("id", [this](std::istream& input_stream) {
      id_ = commonItems::getString(input_stream);
   });
   focus_parser_.registerKeyword("icon", [this](std::istream& input_stream) {
      icon_ = commonItems::getString(input_stream);
   });
   focus_parser_.registerKeyword("text", [this](std::istream& input_stream) {
      text_ = commonItems::getString(input_stream);
   });
   focus_parser_.registerKeyword("mutually_exclusive", [this](std::istream& input_stream) {
      mutually_exclusive_ = commonItems::stringOfItem(input_stream).getString();
   });
   focus_parser_.registerKeyword("bypass", [this](std::istream& input_stream) {
      bypass_ = commonItems::stringOfItem(input_stream).getString();
   });
   focus_parser_.registerKeyword("tree_starter", [this](std::istream& input_stream) {
      tree_starter_ = (commonItems::getString(input_stream) == "yes");
   });
   focus_parser_.registerKeyword("x", [this](std::istream& input_stream) {
      x_position_ = commonItems::getInt(input_stream);
   });
   focus_parser_.registerKeyword("y", [this](std::istream& input_stream) {
      y_position_ = commonItems::getInt(input_stream);
   });
   focus_parser_.registerKeyword("relative_position_id", [this](std::istream& input_stream) {
      relative_position_id_ = commonItems::getString(input_stream);
   });
   focus_parser_.registerKeyword("cost", [this](std::istream& input_stream) {
      cost_ = commonItems::getInt(input_stream);
   });
   focus_parser_.registerKeyword("available_if_capitulated", [this](std::istream& input_stream) {
      available_if_capitulated_ = (commonItems::getString(input_stream) == "yes");
   });
   focus_parser_.registerKeyword("available", [this](std::istream& input_stream) {
      available_ = commonItems::stringOfItem(input_stream).getString();
   });
   focus_parser_.registerKeyword("cancel_if_invalid", [this](std::istream& input_stream) {
      cancel_if_invalid_ = commonItems::getString(input_stream);
   });
   focus_parser_.registerKeyword("continue_if_invalid", [this](std::istream& input_stream) {
      continue_if_invalid_ = commonItems::getString(input_stream);
   });
   focus_parser_.registerKeyword("will_lead_to_war_with", [this](std::istream& input_stream) {
      will_lead_to_war_with_ = commonItems::getString(input_stream);
   });
   focus_parser_.registerKeyword("select_effect", [this](std::istream& input_stream) {
      select_effect_ = commonItems::stringOfItem(input_stream).getString();
   });
   focus_parser_.registerKeyword("complete_tooltip", [this](std::istream& input_stream) {
      complete_tooltip_ = commonItems::getString(input_stream);
   });
   focus_parser_.registerKeyword("completion_reward", [this](std::istream& input_stream) {
      completion_reward_ = commonItems::stringOfItem(input_stream).getString();
   });
   focus_parser_.registerKeyword("ai_will_do", [this](std::istream& input_stream) {
      ai_will_do_ = commonItems::stringOfItem(input_stream).getString();
   });
   focus_parser_.registerKeyword("prerequisite", [this](std::istream& input_stream) {
      prerequisites_.push_back(commonItems::stringOfItem(input_stream).getString());
   });
   focus_parser_.registerKeyword("allow_branch", [this](std::istream& input_stream) {
      allow_branch_ = commonItems::stringOfItem(input_stream).getString();
   });
   focus_parser_.IgnoreAndLogUnregisteredItems();
}


Focus FocusImporter::ImportFocus(std::istream& input_stream)
{
   id_.clear();
   icon_.clear();
   text_.reset();
   prerequisites_.clear();
   mutually_exclusive_.reset();
   bypass_.reset();
   tree_starter_ = false;
   x_position_ = 0;
   y_position_ = 0;
   relative_position_id_.reset();
   cost_ = 0;
   available_if_capitulated_ = false;
   available_.reset();
   cancel_if_invalid_.reset();
   continue_if_invalid_.reset();
   will_lead_to_war_with_.reset();
   select_effect_.reset();
   complete_tooltip_.reset();
   completion_reward_.clear();
   ai_will_do_.reset();
   allow_branch_.reset();

   focus_parser_.parseStream(input_stream);

   return {
       .id = id_,
       .icon = icon_,
       .text = text_,
       .prerequisites = prerequisites_,
       .mutually_exclusive = mutually_exclusive_,
       .bypass = bypass_,
       .tree_starter = tree_starter_,
       .x_position = x_position_,
       .y_position = y_position_,
       .relative_position_id = relative_position_id_,
       .cost = cost_,
       .available_if_capitulated = available_if_capitulated_,
       .available = available_,
       .cancel_if_invalid = cancel_if_invalid_,
       .continue_if_invalid = continue_if_invalid_,
       .will_lead_to_war_with = will_lead_to_war_with_,
       .select_effect = select_effect_,
       .complete_tooltip = complete_tooltip_,
       .completion_reward = completion_reward_,
       .ai_will_do = ai_will_do_,
       .allow_branch = allow_branch_,
   };
}

}  // namespace hoi4
