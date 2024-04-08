#include "src/hoi4_world/roles/stories_creator.h"

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/roles/roles_importer.h"



namespace
{

using Tag = std::string;
using CombinationName = std::string;

std::vector<std::pair<Tag, CombinationName>> MakeCombinations(const std::map<std::string, hoi4::Role>& roles,
    const std::map<std::string, hoi4::Country>& countries)
{
   std::vector<std::pair<Tag, CombinationName>> combinations;

   for (const auto& [role_name, role]: roles)
   {
      for (const auto& [country_tag, country]: countries)
      {
         if (country_tag == "ITA")
         {
            combinations.emplace_back(country_tag, role_name);
         }
      }
   }

   return combinations;
}

}  // namespace



void hoi4::CreateStories(const std::map<std::string, hoi4::Country>& countries)
{
   Log(LogLevel::Info) << "Writing stories";

   const std::map<std::string, Role> roles = ImportRoles();
   Log(LogLevel::Info) << fmt::format("\tImported {} roles.", roles.size());

   const std::vector<std::pair<Tag, CombinationName>> role_combinations = MakeCombinations(roles, countries);
   Log(LogLevel::Info) << fmt::format("\tCreated {} role combinations.", role_combinations.size());
}