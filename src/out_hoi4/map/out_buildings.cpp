#include "src/out_hoi4/map/out_buildings.h"

#include <fstream>
#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/map/out_building.h"



void out::OutputBuildings(std::string_view output_name, const hoi4::Buildings& buildings)
{
   const std::string buildings_path = fmt::format("output/{}/map/buildings.txt", output_name);
   std::ofstream buildings_file(buildings_path);
   if (!buildings_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", buildings_path));
   }
   for (const auto& building: buildings.GetBuildings())
   {
      buildings_file << building;
   }
   buildings_file.close();
}
