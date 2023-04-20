#include "src/out_hoi4/map/out_supply_nodes.h"

#include <fstream>

#include "external/fmt/include/fmt/format.h"



void out::OutputSupplyNodes(std::string_view output_name, const std::set<int>& supply_nodes)
{
   const auto file_path = fmt::format("output/{}/map/supply_nodes.txt", output_name);
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path));
   }

   for (const int supply_node: supply_nodes)
   {
      out << "1 " << supply_node << "\n";
   }

   out.close();
}