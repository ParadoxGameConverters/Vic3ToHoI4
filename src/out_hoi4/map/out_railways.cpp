#include "src/out_hoi4/map/out_railways.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>

#include "src/out_hoi4/map/out_railway.h"



void out::OutputRailways(std::string_view output_name, const std::vector<hoi4::Railway>& railways)
{
   const auto file_path = fmt::format("output/{}/map/railways.txt", output_name);
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path));
   }

   for (const auto& railway: railways)
   {
      if (railway.GetLevel() > 0)
      {
         out << railway;
      }
   }

   out.close();
}