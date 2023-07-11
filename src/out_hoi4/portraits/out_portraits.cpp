#include "src/out_hoi4/portraits/out_portraits.h"

#include <fstream>
#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/portraits/out_portrait_paths.h"

void out::OutputPortraits(std::string_view output_name, const std::map<std::string, hoi4::Country>& countries)
{
   const auto file_path = fmt::format("output/{}/portraits/converter_portraits.txt", output_name);
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path));
   }

   for (const auto& country: countries | std::views::values)
   {
      out << country.GetTag() << " = {\n";
      mappers::OutPortraitPaths(out, country.GetGraphicsBlock().portrait_paths);
      out << "}\n";
   }

   out.close();
}
