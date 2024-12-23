#include "src/hoi4_world/military/task_force.h"

#include <external/fmt/include/fmt/format.h>

#include <sstream>

#include "src/hoi4_world/military/ship.h"

namespace hoi4
{

void PrintTo(const TaskForce& force, std::ostream* os)
{
   *os << "Location = " << force.location << "\n" << fmt::format("{} ships:\n", force.ships.size());
   for (const auto& ship: force.ships)
   {
      *os << fmt::format("{} {} {} {} {} \n",
          ship.GetName(),
          ship.GetDefinition(),
          ship.GetEquipment(),
          ship.GetLegacyEquipment(),
          ship.GetVersion());
   }
}


}  // namespace hoi4
