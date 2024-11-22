#include <sstream>

#include "external/fmt/include/fmt/format.h"
#include "src/mappers/culture/culture_graphics_mapper.h"



namespace mappers
{

void PrintTo(const GraphicsBlock& block, std::ostream* os)
{
   *os << "\n";
   *os << "PortraitPaths:\n";
   for (const auto& [portrait_type, portraits_names]: block.portrait_paths)
   {
      *os << fmt::format("\t{}: {{\n", portrait_type);
      for (const PortraitName& name: portraits_names)
      {
         *os << fmt::format("\t\t{}\n", name);
      }
      *os << "\t}\n";
   }
   *os << fmt::format("GraphicalCulture: {}\n", block.graphical_culture);
   *os << fmt::format("GraphicalCulture2d: {}\n", block.graphical_culture_2d);
}

}  // namespace mappers