#include "src/hoi4_world/roles/repeat_focus_importer.h"

#include "external/commonItems/ParserHelpers.h"



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
   requirement_ = [](const Country&, const World&) {
      static int num_matched = 0;
      ++num_matched;
      return num_matched <= 4;
   };
   focuses_.clear();

   parser_.parseStream(input);

   return {requirement_, focuses_};
}