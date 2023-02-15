#ifndef SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIES_H_
#define SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIES_H_



#include <map>
#include <set>
#include <string>
#include <vector>

#include "src/mappers/technology/tech_mapping.h"



namespace hoi4
{

class Technologies
{
  public:
   Technologies(std::map<std::optional<std::string>, std::set<std::string>> technologies_by_limits,
       std::map<std::string, float> research_bonuses):
       technologies_by_limits_(std::move(technologies_by_limits)),
       research_bonuses_(std::move(research_bonuses))
   {
   }

   [[nodiscard]] bool HasTechnology(const std::string& technology) const;

   [[nodiscard]] const std::map<std::optional<std::string>, std::set<std::string>>& GetTechnologies() const
   {
      return technologies_by_limits_;
   }
   [[nodiscard]] const std::map<std::string, float>& GetResearchBonuses() const { return research_bonuses_; }

  private:
   // technologies are sorted by limits that can be applied to them
   std::map<std::optional<std::string>, std::set<std::string>> technologies_by_limits_;
   std::map<std::string, float> research_bonuses_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIES_H_