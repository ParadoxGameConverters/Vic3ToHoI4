#include "src/mappers/ideology/ideology_mapper.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>

#include <algorithm>
#include <ranges>



namespace
{

mappers::IdeologyPointsMap CalculateIdeologyPoints(const std::map<std::string, mappers::IdeologyPointsMap>& rules,
    const std::set<std::string>& current_laws)
{
   mappers::IdeologyPointsMap point_totals;

   for (const std::string& current_law: current_laws)
   {
      const auto& rule_itr = rules.find(current_law);
      if (rule_itr == rules.end())
      {
         Log(LogLevel::Warning) << fmt::format("Missing ideology mapping rule or sub-ideology mapping rule for {}",
             current_law);
         continue;
      }

      for (const auto& [ideology, points]: rule_itr->second)
      {
         auto [itr, success] = point_totals.emplace(ideology, points);
         if (!success)
         {
            itr->second += points;
         }
      }
   }

   return point_totals;
}

}  // namespace



mappers::IdeologyPointsMap mappers::IdeologyMapper::CalculateIdeologyPoints(
    const std::set<std::string>& current_laws) const
{
   return ::CalculateIdeologyPoints(rules_, current_laws);
}



std::string mappers::IdeologyMapper::GetRulingIdeology(const std::set<std::string>& current_laws) const
{
   IdeologyPointsMap point_totals = ::CalculateIdeologyPoints(rules_, current_laws);
   if (point_totals.empty())
   {
      return "neutrality";
   }

   const auto& winning_ideology = std::ranges::max_element(point_totals, [](const auto& a, const auto& b) {
      return a.second < b.second;
   });

   return winning_ideology->first;
}


std::string mappers::IdeologyMapper::GetSubIdeology(const std::string& ideology,
    const std::set<std::string>& current_laws) const
{
   const auto rules = sub_ideology_rules_.find(ideology);
   if (rules == sub_ideology_rules_.end())
   {
      Log(LogLevel::Warning) << fmt::format("No sub-ideology mapping rules for {}", ideology);
      return "despotism";
   }

   IdeologyPointsMap point_totals = ::CalculateIdeologyPoints(rules->second, current_laws);
   if (point_totals.empty())
   {
      return "despotism";
   }

   const auto& winning_sub_ideology = std::ranges::max_element(point_totals, [](const auto& a, const auto& b) {
      return a.second < b.second;
   });

   return winning_sub_ideology->first;
}