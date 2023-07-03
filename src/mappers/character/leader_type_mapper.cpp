#include "src/mappers/character/leader_type_mapper.h"

#include <ranges>

mappers::LeaderType mappers::LeaderTypeMapper::GetCountryLeaderType(const std::set<std::string>& current_laws) const
{
   // If no mapping is found, head of state is assumed
   for (const std::string& governance_law: rules_ | std::views::keys)
   {
      if (current_laws.contains(governance_law))
      {
         for (const auto& [power_law, leader_type]: rules_.at(governance_law))
         {
            if (current_laws.contains(power_law))
            {
               return leader_type;
            }
         }
         return kHeadOfState;
      }
   }
   return kHeadOfState;
}
