#include "src/hoi4_world/roles/stories_creator.h"

#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/roles/roles_importer.h"



namespace
{

using Tag = std::string;
using CombinationName = std::string;


bool IsRoleValidForCountry(const hoi4::Role& role, const std::string_view country_tag, const hoi4::Country& country)
{
   // scan for 'always=yes' constructs
   const std::regex always_match_regex(R"([\s\S]*always[\s\S]?=[\s\S]?yes[\s\S]*)");
   std::smatch always_match;
   if (std::regex_match(role.GetRequirements(), always_match, always_match_regex))
   {
      return true;
   }

   // scan for 'tag=TAG' constructs
   const std::regex tag_match_regex(R"([\s\S]*tag[\s\S]?=[\s\S]?(\w{3})[\s\S]*)");
   std::smatch tag_match;
   if (std::regex_match(role.GetRequirements(), tag_match, tag_match_regex))
   {
      return tag_match[1] == std::string(country_tag);
   }

   // scan for 'has_culture=culture' constructs
   const std::regex culture_match_regex(R"([\s\S]*has_culture[\s\S]?=[\s\S]?(.+)[\s\S]*)");
   std::smatch culture_match;
   if (std::regex_match(role.GetRequirements(), culture_match, culture_match_regex))
   {
      return country.GetPrimaryCultures().contains(tag_match[1]);
   }

   return false;
}


std::vector<std::pair<Tag, CombinationName>> MakeCombinations(const std::map<std::string, hoi4::Role>& roles,
    const std::map<std::string, hoi4::Country>& countries)
{
   std::vector<std::pair<Tag, CombinationName>> combinations;

   for (const auto& [role_name, role]: roles)
   {
      for (const auto& [country_tag, country]: countries)
      {
         if (IsRoleValidForCountry(role, country_tag, country))
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