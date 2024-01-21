#ifndef SRC_HOI4WORLD_MILITARY_TASK_FORCE_H
#define SRC_HOI4WORLD_MILITARY_TASK_FORCE_H

#include <sstream>
#include <vector>

#include "src/hoi4_world/military/ship.h"

namespace hoi4
{

struct TaskForce
{
   std::string name;
   std::vector<Ship> ships;
   int location;

   std::partial_ordering operator<=>(const TaskForce&) const = default;
   friend void PrintTo(const TaskForce& force, std::ostream* os);
};

}  // namespace hoi4

#endif  // SRC_HOI4WORLD_MILITARY_TASK_FORCE_H
