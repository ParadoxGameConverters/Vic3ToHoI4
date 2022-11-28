#include "src/out_hoi4/map/out_strategic_region.h"

#include <fstream>

#include "external/fmt/include/fmt/format.h"



void out::OutputStrategicRegion(std::string_view output_name, const hoi4::StrategicRegion& strategic_region)
{
   const auto strategic_region_file_name =
       fmt::format("output/{}/map/strategicregions/{}", output_name, strategic_region.GetFilename());
   std::ofstream strategic_region_file(strategic_region_file_name);
   if (!strategic_region_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", strategic_region_file_name));
   }

   strategic_region_file << "\n";
   strategic_region_file << "strategic_region={\n";
   strategic_region_file << "\tid=" << strategic_region.GetID() << "\n";
   strategic_region_file << "\tname=\"" << strategic_region.GetName() << "\"\n";
   strategic_region_file << "\tprovinces={\n";
   strategic_region_file << "\t\t";
   for (const auto& province: strategic_region.GetNewProvinces())
   {
      strategic_region_file << province << " ";
   }
   strategic_region_file << "\n";
   strategic_region_file << "\t}\n";
   if (strategic_region.hasStaticModifiers())
   {
      strategic_region_file << "\tstatic_modifiers={\n";
      for (const auto& modifier: strategic_region.GetStaticModifiers())
      {
         strategic_region_file << "\t\t" << modifier.first << "=" << modifier.second << "\n";
      }
      strategic_region_file << "\n";
      strategic_region_file << "\t}\n";
   }
   auto navalTerrain = strategic_region.GetNavalTerrain();
   if (navalTerrain)
   {
      strategic_region_file << "\tnaval_terrain=" << *navalTerrain << "\n";
   }
   strategic_region_file << "\tweather" << strategic_region.GetWeather() << "\n";
   strategic_region_file << "}";

   strategic_region_file.close();
}