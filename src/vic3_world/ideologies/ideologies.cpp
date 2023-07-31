#include "src/vic3_world/ideologies/ideologies.h"



std::map<vic3::Law, vic3::ApprovalAmount> vic3::Ideologies::CalculateLawApprovals(
    const std::set<std::string>& ideologies) const
{
   std::map<Law, ApprovalAmount> approvals;

   for (const auto& ideology_name: ideologies)
   {
      const auto ideology = ideologies_.find(ideology_name);
      if (ideology == ideologies_.end())
      {
         continue;
      }

      for (const auto& [law, approval]: ideology->second.GetLawApprovals())
      {
         if (auto [itr, success] = approvals.emplace(law, approval); !success)
         {
            itr->second += approval;
         }
      }
   }

   return approvals;
}