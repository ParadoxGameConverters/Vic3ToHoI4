#include "src/hoi4_world/technology/technologies_converter.h"



hoi4::Technologies hoi4::ConvertTechnologies(const std::set<std::string>& vic3_technologies,
    const std::vector<mappers::TechMapping>& tech_mappings)
{
   std::map<std::optional<std::string>, std::set<std::string>> technologies_by_limits;

   for (const auto& tech_mapping: tech_mappings)
   {
      if (std::ranges::any_of(tech_mapping.GetVic3Requirements(), [vic3_technologies](const std::string& requirement) {
             return !vic3_technologies.contains(requirement);
          }))
      {
         continue;
      }

      const auto& limit = tech_mapping.GetLimit();
      for (const auto& technology: tech_mapping.GetTechs())
      {
         auto [itr, inserted] = technologies_by_limits.insert(std::make_pair(limit, std::set{technology}));
         if (!inserted)
         {
            itr->second.insert(technology);
         }
      }
   }

   /*for (const auto& bonusMapping: researchBonusMapper.getResearchBonusMappings())
   {
           bool requirementViolated = false;
           for (const auto& requirement: bonusMapping.getVic2Requirements())
           {
                   if (!oldTechnologiesAndInventions.contains(requirement))
                   {
                           requirementViolated = true;
                           break;
                   }
           }
           if (requirementViolated)
           {
                   continue;
           }

           for (const auto& bonus: bonusMapping.getResearchBonuses())
           {
                   setResearchBonus(bonus.first, bonus.second);
           }
   }*/
   return {technologies_by_limits, {}};
}