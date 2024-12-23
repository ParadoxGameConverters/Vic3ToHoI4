#include "src/out_hoi4/map/out_building.h"

#include <external/fmt/include/fmt/format.h>

#include <iomanip>



std::ostream& out::operator<<(std::ostream& out, const hoi4::Building& building)
{
   out << fmt::format("{};{};{:.2f};{:.2f};{:.2f};{:.2f};{}\n",
       building.GetStateId(),
       building.GetType(),
       building.GetPosition().x_coordinate,
       building.GetPosition().y_coordinate,
       building.GetPosition().z_coordinate,
       building.GetPosition().rotation,
       building.GetConnectingSeaProvince().has_value() ? building.GetConnectingSeaProvince().value() : 0);

   return out;
}