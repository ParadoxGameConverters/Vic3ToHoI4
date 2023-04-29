#include "src/mappers/ideology/ideology_mapper.h"



std::string mappers::IdeologyMapper::GetRulingIdeology(const std::set<std::string>& current_laws) const
{
   if (current_laws.contains("law_landed_voting") || current_laws.contains("law_wealth_voting") ||
       current_laws.contains("law_census_voting") || current_laws.contains("law_universal_suffrage") ||
       current_laws.contains("law_presidential_republic") || current_laws.contains("law_parliamentary_republic") ||
       current_laws.contains("law_council_republic"))
   {
      return "democratic";
   }

   return "neutrality";
}
