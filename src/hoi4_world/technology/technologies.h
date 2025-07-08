#ifndef SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIES_H
#define SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIES_H



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
   Technologies() {}

   explicit Technologies(std::map<std::optional<std::string>, std::set<std::string>> technologies_by_limits):
       technologies_by_limits_(std::move(technologies_by_limits))
   {
   }

   [[nodiscard]] bool HasTechnology(const std::string& technology) const;

   [[nodiscard]] const std::map<std::optional<std::string>, std::set<std::string>>& GetTechnologies() const
   {
      return technologies_by_limits_;
   }

   std::partial_ordering operator<=>(const Technologies&) const = default;

  private:
   // technologies are sorted by limits that can be applied to them
   std::map<std::optional<std::string>, std::set<std::string>> technologies_by_limits_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_TECHNOLOGY_TECHNOLOGIES_H