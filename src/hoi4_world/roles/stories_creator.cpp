#include "src/hoi4_world/roles/stories_creator.h"

#include <ranges>
#include <set>

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
   const std::regex culture_match_regex(R"([\s\S]*country_has_primary_culture[\s\S]?=[\s\S]?(.+)[\s\S]*)");
   std::smatch culture_match;
   if (std::regex_match(role.GetRequirements(), culture_match, culture_match_regex))
   {
      return country.GetPrimaryCultures().contains(culture_match[1]);
   }

   return false;
}


std::optional<std::vector<std::pair<Tag, CombinationName>>> MakeCombinations(
    const std::map<std::string, hoi4::Role>& roles,
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

   Log(LogLevel::Info) << fmt::format("\tCreated {} role combinations.", combinations.size());
   return combinations;
}


std::optional<std::vector<std::pair<Tag, CombinationName>>> SortCombinations(
    std::vector<std::pair<Tag, CombinationName>> combinations,
    const std::map<std::string, hoi4::Role>& roles)
{
   std::ranges::sort(combinations, [roles](const auto& a, const auto& b) {
      int a_score = 0;
      if (const auto role = roles.find(a.first); role != roles.end())
      {
         a_score = role->second.GetScore();
      }
      int b_score = 0;
      if (const auto role = roles.find(b.first); role != roles.end())
      {
         b_score = role->second.GetScore();
      }

      if (a_score != b_score)
      {
         return a_score < b_score;
      }
      if (a.first != b.first)
      {
         return a.first < b.first;
      }
      return a.second < b.second;
   });

   return combinations;
}


std::optional<std::vector<std::pair<Tag, CombinationName>>> FilterCombinations(
    std::vector<std::pair<Tag, CombinationName>> combinations,
    const std::map<std::string, hoi4::Role>& roles)
{
   std::set<std::pair<Tag, std::string>> blocked_combinations;

   const auto [first, last] = std::ranges::remove_if(combinations,
       [&blocked_combinations, roles](const std::pair<Tag, CombinationName>& combination) {
          const auto& role = roles.find(combination.second);
          if (role == roles.end())
          {
             return true;
          }

          if (blocked_combinations.contains(combination))
          {
             return true;
          }
          if (blocked_combinations.contains(std::make_pair(combination.first, role->second.GetCategory())))
          {
             return true;
          }

          for (const std::string blocker: role->second.GetBlockers())
          {
             blocked_combinations.emplace(std::make_pair(combination.first, blocker));
          }

          return false;
       });
   combinations.erase(first, last);

   Log(LogLevel::Info) << fmt::format("\tFiltered to {} role combinations.", combinations.size());
   return combinations;
}


std::optional<std::vector<std::pair<Tag, hoi4::Role>>> ExpandCombinations(
    const std::vector<std::pair<Tag, CombinationName>>& combinations,
    const std::map<std::string, hoi4::Role>& roles)
{
   std::vector<std::pair<Tag, hoi4::Role>> expanded_combinations;

   for (const auto& [tag, role_name]: combinations)
   {
      const auto& role = roles.find(role_name);
      if (role == roles.end())
      {
         return std::nullopt;
      }

      expanded_combinations.emplace_back(tag, role->second);
   }

   return expanded_combinations;
}

}  // namespace



std::vector<std::pair<Tag, hoi4::Role>> hoi4::CreateStories(const std::map<std::string, hoi4::Role>& roles,
    const std::map<std::string, hoi4::Country>& countries)
{
   Log(LogLevel::Info) << "Writing stories";
   const std::vector<std::pair<Tag, Role>> role_combinations =
       MakeCombinations(roles, countries)
           .and_then([roles](std::vector<std::pair<Tag, CombinationName>> combinations) {
              return SortCombinations(combinations, roles);
           })
           .and_then([roles](std::vector<std::pair<Tag, CombinationName>> combinations) {
              return FilterCombinations(combinations, roles);
           })
           .and_then([roles](std::vector<std::pair<Tag, CombinationName>> combinations) {
              return ExpandCombinations(combinations, roles);
           })
           .value_or(std::vector<std::pair<Tag, Role>>{});

   // remove this once tests are written and just return the results above
   Log(LogLevel::Info) << "\tCombinations:";
   for (const auto& combination: role_combinations)
   {
      Log(LogLevel::Info) << fmt::format("\t\t{} - {}", combination.first, combination.second.GetName());
   }

   return role_combinations;
}