#include "src/mappers/character/leader_type_mapper.h"



mappers::LeaderType mappers::LeaderTypeMapper::GetCountryLeaderType(const std::set<std::string>& current_laws) const
{
   // If no mapping is found, head of state is assumed
   for (const auto& [governance_law, power_leader_map]: rules_)
   {
      if (current_laws.contains(governance_law))
      {
         for (const auto& [power_law, leader_type]: power_leader_map)
         {
            if (current_laws.contains(power_law))
            {
               return leader_type;
            }
         }
         return "head_of_state";
      }
   }
   return "head_of_state";
}
