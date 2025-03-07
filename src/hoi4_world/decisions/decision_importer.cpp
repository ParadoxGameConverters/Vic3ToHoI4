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
      const commonItems::stringOfItem theAllowed(the_stream);
      decision_.allowed = theAllowed.getString();
   });
   parser_.registerRegex("highlight_state_targets|highlight_states",
       [this]([[maybe_unused]] const std::string& unused, std::istream& the_stream) {
          const commonItems::stringOfItem theHighlightStates(the_stream);
          decision_.highlight_state_targets = theHighlightStates.getString();
       });
   parser_.registerKeyword("available", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theAvailable(the_stream);
      decision_.available = theAvailable.getString();
   });
   parser_.registerKeyword("days_mission_timeout", [this](std::istream& the_stream) {
      decision_.days_mission_timeout = commonItems::getInt(the_stream);
   });
   parser_.registerKeyword("activation", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theActivation(the_stream);
      decision_.activation = theActivation.getString();
   });
   parser_.registerKeyword("targets", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theTargets(the_stream);
      decision_.targets = theTargets.getString();
   });
   parser_.registerKeyword("target_array", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theTargetArray(the_stream);
      decision_.target_array = theTargetArray.getString();
   });
   parser_.registerKeyword("target_root_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theTargetRootTrigger(the_stream);
      decision_.target_root_trigger = theTargetRootTrigger.getString();
   });
   parser_.registerKeyword("state_target", [this](std::istream& the_stream) {
      decision_.state_target = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("target_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theTargetTrigger(the_stream);
      decision_.target_trigger = theTargetTrigger.getString();
   });
   parser_.registerKeyword("target_non_existing", [this](std::istream& the_stream) {
      decision_.target_non_existing = commonItems::getString(the_stream) == "yes";
   });
   parser_.registerKeyword("visible", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theVisible(the_stream);
      decision_.visible = theVisible.getString();
   });
   parser_.registerKeyword("cancel_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theCancelTrigger(the_stream);
      decision_.cancel_trigger = theCancelTrigger.getString();
   });
   parser_.registerKeyword("targeted_modifier", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theTargetedModifier(the_stream);
      decision_.targeted_modifier = theTargetedModifier.getString();
   });
   parser_.registerKeyword("remove_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theRemoveTrigger(the_stream);
      decision_.remove_trigger = theRemoveTrigger.getString();
   });
   parser_.registerKeyword("custom_cost_trigger", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theCustomCostTrigger(the_stream);
      decision_.custom_cost_trigger = theCustomCostTrigger.getString();
   });
   parser_.registerKeyword("custom_cost_text", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theCustomCostText(the_stream);
      decision_.custom_cost_text = theCustomCostText.getString();
   });
   parser_.registerKeyword("on_map_mode", [this](std::istream& the_stream) {
      decision_.on_map_mode = commonItems::getString(the_stream);
   });
   parser_.registerKeyword("complete_effect", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theCompleteEffect(the_stream);
      decision_.complete_effect = theCompleteEffect.getString();
   });
   parser_.registerKeyword("remove_effect", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theRemoveEffect(the_stream);
      decision_.remove_effect = theRemoveEffect.getString();
   });
   parser_.registerKeyword("timeout_effect", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theTimeoutEffect(the_stream);
      decision_.timeout_effect = theTimeoutEffect.getString();
   });
   parser_.registerKeyword("ai_will_do", [this](std::istream& the_stream) {
      const commonItems::stringOfItem theAiWillDo(the_stream);
      decision_.ai_will_do = theAiWillDo.getString();
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
      const commonItems::stringOfItem theModifier(the_stream);
      decision_.modifier = theModifier.getString();
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