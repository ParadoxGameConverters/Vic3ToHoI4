#include "src/hoi4_world/roles/roles_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/ParserHelpers.h"
#include "src/hoi4_world/roles/role_importer.h"



std::vector<hoi4::Role> hoi4::ImportRoles()
{
   std::vector<hoi4::Role> roles;

   RoleImporter role_importer;
   commonItems::parser roles_parser;
   roles_parser.registerRegex(commonItems::catchallRegex,
       [&roles, &role_importer](const std::string name, std::istream& input) {
          roles.emplace_back(role_importer.ImportRole(name, input));
       });

   for (const std::string& roles_file: commonItems::GetAllFilesInFolderRecursive("configurables/stories"))
   {
      roles_parser.parseFile("configurables/stories/" + roles_file);
   }

   return roles;
}
