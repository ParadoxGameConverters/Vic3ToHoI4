#ifndef SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPER_H
#define SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPER_H



#include <map>
#include <set>
#include <string>



namespace mappers
{

using IdeologyPointsMap = std::map<std::string, int>;
using ItemToPointsMap = std::map<std::string, IdeologyPointsMap>;


class IdeologyMapper
{
  public:
   explicit IdeologyMapper(ItemToPointsMap rules, std::map<std::string, ItemToPointsMap> sub_ideology_rules):
       rules_(std::move(rules)),
       sub_ideology_rules_(std::move(sub_ideology_rules))
   {
   }

   [[nodiscard]] mappers::IdeologyPointsMap CalculateIdeologyPoints(const std::set<std::string>& current_laws) const;
   [[nodiscard]] std::string GetRulingIdeology(const std::set<std::string>& current_laws) const;
   [[nodiscard]] std::string GetSubIdeology(const std::string& ideology,
       const std::set<std::string>& current_laws) const;

  private:
   ItemToPointsMap rules_;
   std::map<std::string, ItemToPointsMap> sub_ideology_rules_;
};

}  // namespace mappers



#endif  // SRC_MAPPERS_IDEOLOGY_IDEOLOGYMAPPER_H