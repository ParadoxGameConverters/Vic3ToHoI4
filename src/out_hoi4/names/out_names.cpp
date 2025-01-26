#include "src/out_hoi4/names/out_names.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>
#include <ranges>

#include "src/out_hoi4/names/out_name_list.h"



using std::filesystem::path;



void out::OutputNames(const path& output_name, const std::map<std::string, hoi4::Country>& countries)
{
   const path file_path = "output" / output_name / "common/names/converter_names.txt";
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path.string()));
   }

   for (const auto& country: countries | std::views::values)
   {
      out << country.GetTag() << " = {\n";
      out << country.GetNameList();
      out << "}\n";
   }

   out.close();
}
