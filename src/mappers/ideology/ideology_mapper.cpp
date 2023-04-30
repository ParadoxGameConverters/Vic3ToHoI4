#include "src/mappers/ideology/ideology_mapper.h"

#include <algorithm>
#include <ranges>



namespace
{

mappers::ideology_points_map CalculateIdeologyPoints(const std::map<std::string, mappers::ideology_points_map>& rules,
    const std::set<std::string>& current_laws)
{
   mappers::ideology_points_map point_totals;

   for (const std::string& current_law: current_laws)
   {
      const auto& rule_itr = rules.find(current_law);
      if (rule_itr == rules.end())
      {
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



std::string mappers::IdeologyMapper::GetRulingIdeology(const std::set<std::string>& current_laws) const
{
   ideology_points_map point_totals = CalculateIdeologyPoints(rules_, current_laws);
   if (point_totals.empty())
   {
      return "neutrality";
   }

   const auto& winning_ideology = std::ranges::max_element(point_totals, [](const auto& a, const auto& b) {
      return a.second < b.second;
   });

   return winning_ideology->first;
}