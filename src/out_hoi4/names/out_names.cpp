#include "src/out_hoi4/names/out_names.h"

#include <fstream>
#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/names/out_name_list.h"



void out::OutputNames(std::string_view output_name, const std::map<std::string, hoi4::Country>& countries)
{
   const auto file_path = fmt::format("output/{}/common/names/converter_names.txt", output_name);
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path));
   }

   for (const auto& country: countries | std::views::values)
   {
      out << country.GetTag() << " = {\n ";
      out << country.GetNameList();
      out << "}";
   }

   out.close();
}
