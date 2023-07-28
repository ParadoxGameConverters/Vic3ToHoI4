#include "src/mappers/character/generic_advisor_mapper.h"
﻿
﻿
﻿#include <ranges>
﻿
﻿ namespace {
﻿   std::set<hoi4::Trait> GetConvertedAdvisorTraits(const std::vector<int>& character_ids, const std::map<int, hoi4::Character>& characters)
﻿   {
﻿     std::set<hoi4::Trait> occupied_traits;
﻿     for( const int id: character_ids)
﻿     {
﻿       const auto& character_itr = characters.find(id);
﻿       if(character_itr == characters.end())
﻿       {
﻿         continue;
﻿       }
﻿       const auto& character = character_itr->second;
﻿       
﻿       if (!character.GetAdvisorData()))
﻿       {
﻿         continue;
﻿       }
﻿       
﻿       for(const hoi4::Trait& trait:  character.GetAdvisorData().traits))
﻿       {
﻿          occupied_traits.emplace(trait);
﻿       }
﻿     }
﻿     return occupied_traits;
﻿   }
﻿ }
﻿
﻿AdvisorMap mappers::SelectGenericAdvisors(const std::vector<int>& character_ids, std::map<int, hoi4::Character>& characters)
﻿{
﻿  AdvisorMap generic_advisors;
﻿  std::set<hoi4::Trait> converted_advisor_traits = GetConvertedAdvisorTraits(character_ids, characters);
﻿  
﻿  std::ranges::copy_if(advisor_map_, std::inserter (generic_advisors, generic_advisors.begin(), [converted_advisor_traits](const std::pair<hoi4::Trait, hoi4::Slot>& advisor){
﻿     return converted_advisor_traits.contains(advisor.first);
﻿  });
﻿  
﻿  return generic_advisors;
﻿}