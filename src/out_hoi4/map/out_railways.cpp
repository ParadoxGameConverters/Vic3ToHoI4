#include "src/out_hoi4/map/out_railways.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>

#include "src/out_hoi4/map/out_railway.h"



using std::filesystem::path;



void out::OutputRailways(const path& output_name, const std::vector<hoi4::Railway>& railways)
{
   const path file_path = "output" / output_name / "map/railways.txt";
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path.string()));
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