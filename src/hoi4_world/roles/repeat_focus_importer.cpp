#include "src/hoi4_world/roles/repeat_focus_importer.h"

#include <external/commonItems/ParserHelpers.h>

#include "requirements/always_trigger.h"


hoi4::RepeatFocusImporter::RepeatFocusImporter()
{
   parser_.registerKeyword("focus", [this](std::istream& input) {
      focuses_.emplace_back(focus_importer_.ImportFocus(input));
   });
   parser_.registerKeyword("requirement", [](std::istream& input) {
      commonItems::ignoreItem("", input);
   });
   parser_.IgnoreUnregisteredItems();
}


hoi4::RepeatFocus hoi4::RepeatFocusImporter::ImportRepeatFocus(std::istream& input)
{
   requirement_ = std::make_unique<AlwaysTrigger>(false);
   focuses_.clear();

   parser_.parseStream(input);

   return {std::move(requirement_), focuses_};
}