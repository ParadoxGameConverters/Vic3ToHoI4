#include "src/hoi4_world/map/hoi4_building.h"

#include <external/fmt/include/fmt/format.h>

#include <sstream>



namespace hoi4
{

void PrintTo(const Building& building, std::ostream* os)
{
   *os << "\n"
       << fmt::format("state id: {}\n", building.GetStateId()) << fmt::format("type: {}\n", building.GetType())
       << fmt::format("position: ({}, {}, {})\n",
              building.GetPosition().x_coordinate,
              building.GetPosition().y_coordinate,
              building.GetPosition().z_coordinate)
       << fmt::format("rotation: {}\n", building.GetPosition().rotation);
   if (building.GetConnectingSeaProvince().has_value())
   {
      *os << fmt::format("connecting sea province: {}", building.GetConnectingSeaProvince().value());
   }
}

}  // namespace hoi4