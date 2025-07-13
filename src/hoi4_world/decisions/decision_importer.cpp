#include "src/hoi4_world/decisions/decision_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/ParserHelpers.h>



namespace hoi4
{

DecisionImporter::DecisionImporter()
{
   parser_.registerKeyword("name", [this](std::istream& the_stream) {
      decision_.name_field = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("icon", [this](std::istream& the_stream) {
      decision_.icon = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("is_good", [this](std::istream& the_stream) {
      decision_.is_good = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("allowed", [this](std::istream& the_stream) {
      const commonItems::stringOfItem allowed(the_stream);
      decision_.allowed = allowed.getString();
   });
   parser_.registerRegex("highlight_state_targets|highlight_states",
       [this]([[maybe_unused]] const std::string& unused, std::istream& the_stream) {
          const commonItems::stringOfItem highlight_states(the_stream);
          decision_.highlight_state_targets = highlight_states.getString();
       });
   parser_.registerKeyword("available", [this](std::istream& the_stream) {
      const commonItems::stringOfItem available(the_stream);
      decision_.available = available.getString();
   });
   parser_.registerKeyword("days_mission_timeout", [this](std::istream& the_stream) {
      decision_.days_mission_timeout = commonItems::getInt(the_stream);
   });
   parser_.registerKeyword("activation", [this](std::istream& the_stream) {
      const commonItems::stringOfItem activation(the_stream);
      decision_.activation = activation.getString();
   });
   parser_.registerKeyword("targets", [this](std::istream& the_stream) {
      const commonItems::stringOfItem targets(the_stream);
      decision_.targets = targets.getString();
   });
   parser_.registerKeyword("target_array", [this](std::istream& the_stream) {
      const commonItems::stringOfItem target_array(the_stream);
      decision_.target_array = target_array.getString();
   });
   parser_.registerKeyword("target_root_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem target_root_trigger(the_stream);
      decision_.target_root_trigger = target_root_trigger.getString();
   });
   parser_.registerKeyword("state_target", [this](std::istream& the_stream) {
      decision_.state_target = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("target_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem target_trigger(the_stream);
      decision_.target_trigger = target_trigger.getString();
   });
   parser_.registerKeyword("target_non_existing", [this](std::istream& the_stream) {
      decision_.target_non_existing = commonItems::getString(the_stream) == "yes";
   });
   parser_.registerKeyword("visible", [this](std::istream& the_stream) {
      const commonItems::stringOfItem visible(the_stream);
      decision_.visible = visible.getString();
   });
   parser_.registerKeyword("cancel_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem cancel_trigger(the_stream);
      decision_.cancel_trigger = cancel_trigger.getString();
   });
   parser_.registerKeyword("targeted_modifier", [this](std::istream& the_stream) {
      const commonItems::stringOfItem targeted_modifier(the_stream);
      decision_.targeted_modifier = targeted_modifier.getString();
   });
   parser_.registerKeyword("remove_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem remove_trigger(the_stream);
      decision_.remove_trigger = remove_trigger.getString();
   });
   parser_.registerKeyword("custom_cost_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem custom_cost_trigger(the_stream);
      decision_.custom_cost_trigger = custom_cost_trigger.getString();
   });
   parser_.registerKeyword("custom_cost_text", [this](std::istream& the_stream) {
      const commonItems::stringOfItem custom_cost_text(the_stream);
      decision_.custom_cost_text = custom_cost_text.getString();
   });
   parser_.registerKeyword("on_map_mode", [this](std::istream& the_stream) {
      decision_.on_map_mode = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("complete_effect", [this](std::istream& the_stream) {
      const commonItems::stringOfItem complete_effect(the_stream);
      decision_.complete_effect = complete_effect.getString();
   });
   parser_.registerKeyword("remove_effect", [this](std::istream& the_stream) {
      const commonItems::stringOfItem remove_effect(the_stream);
      decision_.remove_effect = remove_effect.getString();
   });
   parser_.registerKeyword("timeout_effect", [this](std::istream& the_stream) {
      const commonItems::stringOfItem timeout_effect(the_stream);
      decision_.timeout_effect = timeout_effect.getString();
   });
   parser_.registerKeyword("ai_will_do", [this](std::istream& the_stream) {
      const commonItems::stringOfItem ai_will_do(the_stream);
      decision_.ai_will_do = ai_will_do.getString();
   });
   parser_.registerKeyword("days_remove", [this](std::istream& the_stream) {
      decision_.days_remove = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("days_re_enable", [this](std::istream& the_stream) {
      decision_.days_reenable = commonItems::getInt(the_stream);
   });
   parser_.registerKeyword("cost", [this](std::istream& the_stream) {
      decision_.cost = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("fire_only_once", [this](std::istream& the_stream) {
      decision_.fire_only_once = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("modifier", [this](std::istream& the_stream) {
      const commonItems::stringOfItem modifier(the_stream);
      decision_.modifier = modifier.getString();
   });
   parser_.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}


Decision DecisionImporter::GetDecision(std::string_view name, std::istream& the_stream)
{
   decision_ = {};
   decision_.name = name;

   parser_.parseStream(the_stream);

   return decision_;
}

}  // namespace hoi4