#include "src/out_hoi4/map/out_strategic_regions.h"

#include <ranges>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/map/out_strategic_region.h"



void out::OutputStrategicRegions(std::string_view output_name, const hoi4::StrategicRegions& strategic_regions)
{
   if (!commonItems::TryCreateFolder(fmt::format("output/{}/map/strategicregions", output_name)))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/map/strategicregions", output_name));
   }
   for (const auto& strategic_region: strategic_regions.GetStrategicRegions() | std::views::values)
   {
      OutputStrategicRegion(output_name, strategic_region);
   }
}