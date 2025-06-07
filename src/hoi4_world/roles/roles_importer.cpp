#include "src/hoi4_world/roles/roles_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>

#include "src/hoi4_world/roles/role_importer.h"



using std::filesystem::path;



std::map<std::string, hoi4::Role> hoi4::ImportRoles()
{
   std::map<std::string, hoi4::Role> roles;

   RoleImporter role_importer;
   commonItems::parser roles_parser;
   roles_parser.registerRegex(commonItems::catchallRegex,
       [&roles, &role_importer](const std::string name, std::istream& input) {
          roles.emplace(name, role_importer.ImportRole(name, input));
       });

   for (const path& roles_file: commonItems::GetAllFilesInFolderRecursive(path("configurables/stories")))
   {
      roles_parser.parseFile("configurables/stories" / roles_file);
   }

   Log(LogLevel::Info) << fmt::format("\tImported {} roles.", roles.size());
   return roles;
}