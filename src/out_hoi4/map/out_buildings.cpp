#include "src/out_hoi4/map/out_buildings.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>
#include <ranges>

#include "src/out_hoi4/map/out_building.h"



using std::filesystem::path;



void out::OutputBuildings(const path& output_name, const hoi4::Buildings& buildings)
{
   const path buildings_path = "output" / output_name / "map/buildings.txt";
   std::ofstream buildings_file(buildings_path);
   if (!buildings_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", buildings_path.string()));
   }
   for (const auto& building: buildings.GetBuildings())
   {
      buildings_file << building;
   }
   buildings_file.close();
}
