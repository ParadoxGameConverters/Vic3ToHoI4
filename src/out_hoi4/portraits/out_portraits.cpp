#include "src/out_hoi4/portraits/out_portraits.h"

#include <fstream>

#include "external/fmt/include/fmt/format.h"

void out::OutputPortraits(std::string_view output_name, const std::vector<hoi4::Country>& countries)
{
   const auto file_path = fmt::format("output/{}/portraits/converter_portraits.txt", output_name);
   std::ofstream out(file_path);
   if (!out.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}", file_path));
   }

   for (const auto& country: countries)
   {
      const auto& graphics_block = country.GetGraphicsBlock();

      // out << fmt::format("{} = {\n", country.GetTag());
      out << "\tarmy = {\n";
      out << "\t\tmale = {\n";
      // out << graphics_block.portrait_paths.army;
      out << "\t\t}\n";
      out << "\t}\n";
      out << "}";
      // ...
   }

   out.close();
}
