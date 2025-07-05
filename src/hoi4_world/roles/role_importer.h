#ifndef SRC_HOI4WORLD_ROLES_ROLEIMPORTER_H
#define SRC_HOI4WORLD_ROLES_ROLEIMPORTER_H



#include <external/commonItems/Parser.h>

#include "src/hoi4_world/decisions/decision_importer.h"
#include "src/hoi4_world/decisions/decisions_category_importer.h"
#include "src/hoi4_world/events/event_importer.h"
#include "src/hoi4_world/focus_trees/focus_importer.h"
#include "src/hoi4_world/roles/repeat_focus_importer.h"
#include "src/hoi4_world/roles/role.h"
#include "src/hoi4_world/roles/triggers/trigger_importer.h"



namespace hoi4
{


class RoleImporter
{
  public:
   RoleImporter();

   Role ImportRole(std::string_view name, std::istream& input);

  private:
   RoleOptions role_options_;

   commonItems::parser role_parser_;
   TriggerImporter trigger_importer_;
   FocusImporter focus_importer_;
   RepeatFocusImporter repeat_focus_importer_;
   DecisionsCategoryImporter decisions_category_importer_;
   commonItems::parser decisions_categories_parser_;
   DecisionImporter decision_importer_;
   commonItems::parser decisions_category_parser_;
   commonItems::parser decisions_parser_;
   std::string current_decision_category_;
   commonItems::parser events_parser_;
   EventImporter event_importer_;
};

};  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_ROLEIMPORTER_H