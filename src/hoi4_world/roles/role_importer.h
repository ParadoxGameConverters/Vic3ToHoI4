#ifndef SRC_HOI4WORLD_ROLES_ROLEIMPORTER_H
#define SRC_HOI4WORLD_ROLES_ROLEIMPORTER_H



#include "external/commonItems/Parser.h"
#include "src/hoi4_world/focus_trees/focus_importer.h"
#include "src/hoi4_world/roles/role.h"



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
   FocusImporter focus_importer_;
};

};  // namespace hoi4



#endif  // SRC_HOI4WORLD_ROLES_ROLEIMPORTER_H