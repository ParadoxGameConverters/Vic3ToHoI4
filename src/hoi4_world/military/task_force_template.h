#ifndef SRC_HOI4WORLD_MILITARY_TASKFORCETEMPLATE_H
#define SRC_HOI4WORLD_MILITARY_TASKFORCETEMPLATE_H

#include <map>
#include <set>
#include <string>

#include "src/hoi4_world/military/ship.h"


namespace hoi4
{

class TaskForceTemplate
{
  public:
   TaskForceTemplate(std::map<std::string, float> cost, std::vector<Ship> ships): cost_(cost), ships_(ships) {}

   bool AllVariantsActive(const std::set<std::string>& active_variants) const;
   void AddShipsIfPossible(std::vector<hoi4::Ship>& ships,
       std::map<std::string, int>& shipCounts,
       std::map<std::string, float>& points) const;

   std::partial_ordering operator<=>(const TaskForceTemplate&) const = default;

  private:
   std::map<std::string, float> cost_;
   std::vector<Ship> ships_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MILITARY_TASKFORCETEMPLATE_H
