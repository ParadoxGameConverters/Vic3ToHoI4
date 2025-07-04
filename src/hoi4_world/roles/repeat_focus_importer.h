#ifndef SRC_HOI4WORLD_ROLES_REPEATFOCUSIMPORTER_H
#define SRC_HOI4WORLD_ROLES_REPEATFOCUSIMPORTER_H



#include <external/commonItems/Parser.h>

#include "src/hoi4_world/focus_trees/focus_importer.h"
#include "src/hoi4_world/roles/repeat_focus.h"
#include "src/hoi4_world/roles/requirements/trigger_importer.h"



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
   TriggerImporter trigger_importer_;

   std::unique_ptr<Trigger> requirement_;
   std::vector<Focus> focuses_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_REPEATFOCUSIMPORTER_H