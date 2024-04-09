#include "src/hoi4_world/roles/stories_creator.h"

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/roles/roles_importer.h"



void hoi4::CreateStories()
{
   Log(LogLevel::Info) << "Writing stories";
   std::map<std::string, Role> roles = ImportRoles();
   Log(LogLevel::Info) << fmt::format("\tImported {} roles.", roles.size());
}