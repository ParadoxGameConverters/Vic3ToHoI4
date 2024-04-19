#include "src/hoi4_world/roles/stories_creator.h"

#include <ranges>

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
      // scan for 'tag=TAG' constructs
      std::regex tag_match_regex(R"([\s\S]+tag=(.+)[\s\S]+)");
      std::smatch match;
      if (!std::regex_match(role.GetRequirements(), match, tag_match_regex))
      {
         continue;
      }
      std::string required_tag = match[1];

      // even though a direct lookup would suffice for this simple condition, loop over all countries because we'll
      // eventually have more complex conditions that aren't so easily checked
      for (const std::string& country_tag: countries | std::views::keys)
      {
         if (required_tag == country_tag)
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