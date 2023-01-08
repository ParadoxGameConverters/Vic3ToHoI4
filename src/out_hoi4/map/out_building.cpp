#include "src/out_hoi4/map/out_building.h"

#include <iomanip>

#include "external/fmt/include/fmt/format.h"



std::ostream& out::operator<<(std::ostream& out, const hoi4::Building& building)
{
   out << fmt::format("{};{};{:.2f};{:.2f};{:.2f};{:.2f};{}\n",
       building.state_id,
       building.type,
       building.position.x_coordinate,
       building.position.y_coordinate,
       building.position.z_coordinate,
       building.position.rotation,
       building.connecting_sea_province.has_value() ? building.connecting_sea_province.value() : 0);

   return out;
}