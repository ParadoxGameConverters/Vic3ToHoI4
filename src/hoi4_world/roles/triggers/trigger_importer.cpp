#include "src/hoi4_world/roles/triggers/trigger_importer.h"

#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/and_trigger.h"
#include "src/hoi4_world/roles/triggers/is_capital_trigger.h"
#include "src/hoi4_world/roles/triggers/not_trigger.h"
#include "src/hoi4_world/roles/triggers/or_trigger.h"
#include "src/hoi4_world/roles/triggers/tag_trigger.h"



namespace hoi4
{

TriggerImporter::TriggerImporter()
{
   trigger_parser_.registerKeyword("always", [this](std::istream& input) {
      const std::optional<std::string> equals = trigger_parser_.getNextTokenWithoutMatching(input);
      const std::optional<std::string> value_string = trigger_parser_.getNextTokenWithoutMatching(input);
      const bool value = value_string.value_or("no") == "yes";
      triggers_.push_back(std::make_unique<AlwaysTrigger>(value));
   });
   trigger_parser_.registerKeyword("tag", [this](std::istream& input) {
      const std::optional<std::string> equals = trigger_parser_.getNextTokenWithoutMatching(input);
      if (const std::optional<std::string> tag_string = trigger_parser_.getNextTokenWithoutMatching(input); tag_string)
      {
         triggers_.push_back(std::make_unique<TagTrigger>(tag_string.value()));
      }
   });
   trigger_parser_.registerKeyword("AND", [this](std::istream& input) {
      std::vector<std::unique_ptr<Trigger>> triggers = TriggerImporter{}.ImportTriggers(input);
      triggers_.push_back(std::make_unique<AndTrigger>(std::move(triggers)));
   });
   trigger_parser_.registerKeyword("OR", [this](std::istream& input) {
      std::vector<std::unique_ptr<Trigger>> triggers = TriggerImporter{}.ImportTriggers(input);
      triggers_.push_back(std::make_unique<OrTrigger>(std::move(triggers)));
   });
   trigger_parser_.registerKeyword("NOT", [this](std::istream& input) {
      std::vector<std::unique_ptr<Trigger>> triggers = TriggerImporter{}.ImportTriggers(input);
      triggers_.push_back(std::make_unique<NotTrigger>(std::move(triggers)));
   });
   trigger_parser_.registerKeyword("is_capital", [this]([[maybe_unused]] std::istream& input) {
      const std::optional<std::string> equals = trigger_parser_.getNextTokenWithoutMatching(input);
      const std::optional<std::string> value_string = trigger_parser_.getNextTokenWithoutMatching(input);
      const bool value = value_string.value_or("no") == "yes";
      triggers_.push_back(std::make_unique<IsCapitalTrigger>(value));
   });
   trigger_parser_.IgnoreAndLogUnregisteredItems();
}


std::unique_ptr<Trigger> TriggerImporter::ImportTrigger(std::istream& input)
{
   std::vector<std::unique_ptr<Trigger>> triggers = ImportTriggers(input);

   if (triggers.empty())
   {
      return std::make_unique<AlwaysTrigger>(true);
   }
   if (triggers.size() == 1)
   {
      return std::move(triggers[0]);
   }
   else
   {
      return std::make_unique<AndTrigger>(std::move(triggers));
   }
}


std::vector<std::unique_ptr<Trigger>> TriggerImporter::ImportTriggers(std::istream& input)
{
   trigger_parser_.parseStream(input);

   return std::move(triggers_);
}

}  // namespace hoi4
