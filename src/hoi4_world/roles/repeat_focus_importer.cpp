#include "src/hoi4_world/roles/repeat_focus_importer.h"

#include <external/commonItems/ParserHelpers.h>

#include "src/hoi4_world/roles/triggers/always_trigger.h"



hoi4::RepeatFocusImporter::RepeatFocusImporter()
{
   parser_.registerKeyword("focus", [this](std::istream& input) {
      focuses_.emplace_back(focus_importer_.ImportFocus(input));
   });
   parser_.registerKeyword("trigger", [this](std::istream& input) {
      trigger_ = trigger_importer_.ImportTrigger(input);
   });
   parser_.IgnoreUnregisteredItems();
}


hoi4::RepeatFocus hoi4::RepeatFocusImporter::ImportRepeatFocus(std::istream& input)
{
   trigger_ = std::make_unique<AlwaysTrigger>(false);
   focuses_.clear();

   parser_.parseStream(input);

   return {std::move(trigger_), focuses_};
}