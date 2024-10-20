#ifndef SRC_HOI4WORLD_ROLES_REPEATFOCUSIMPORTER_H
#define SRC_HOI4WORLD_ROLES_REPEATFOCUSIMPORTER_H



#include "external/commonItems/Parser.h"
#include "src/hoi4_world/focus_trees/focus_importer.h"
#include "src/hoi4_world/roles/repeat_focus.h"



namespace hoi4
{

class RepeatFocusImporter
{
  public:
   RepeatFocusImporter();

   RepeatFocus ImportRepeatFocus(std::istream& input);

  private:
   commonItems::parser parser_;
   FocusImporter focus_importer_;

   std::function<bool(const Country&, const World&)> requirement_;
   std::vector<Focus> focuses_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_REPEATFOCUSIMPORTER_H