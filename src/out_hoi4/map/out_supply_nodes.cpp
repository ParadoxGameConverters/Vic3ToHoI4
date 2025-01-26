#include "src/out_hoi4/map/out_supply_nodes.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>



using std::filesystem::path;



void out::OutputSupplyNodes(const path& output_name, const std::set<int>& supply_nodes)
{
   const path file_path = "output" / output_name / "map/supply_nodes.txt";
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path.string()));
   }

   for (const int supply_node: supply_nodes)
   {
      out << "1 " << supply_node << "\n";
   }

   out.close();
}