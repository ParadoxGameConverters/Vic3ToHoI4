#include "src/out_hoi4/map/out_strategic_regions.h"

#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/fmt/include/fmt/format.h>

#include <ranges>

#include "src/out_hoi4/map/out_strategic_region.h"



using std::filesystem::path;



void out::OutputStrategicRegions(const path& output_name, const hoi4::StrategicRegions& strategic_regions)
{
   if (const path strategic_path = "output" / output_name / "map/strategicregions";
       !commonItems::DoesFolderExist(strategic_path) && !create_directories(strategic_path))
   {
      throw std::runtime_error(fmt::format("Could not create output/{}/map/strategicregions", output_name.string()));
   }
   for (const auto& strategic_region: strategic_regions.GetStrategicRegions() | std::views::values)
   {
      OutputStrategicRegion(output_name, strategic_region);
   }
}