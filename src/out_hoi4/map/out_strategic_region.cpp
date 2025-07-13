#include "src/out_hoi4/map/out_strategic_region.h"

#include <external/fmt/include/fmt/format.h>

#include <fstream>



using std::filesystem::path;



void out::OutputStrategicRegion(const path& output_name, const hoi4::StrategicRegion& strategic_region)
{
   const path strategic_region_file_name =
       "output" / output_name / "map/strategicregions" / strategic_region.GetFilename();
   std::ofstream strategic_region_file(strategic_region_file_name);
   if (!strategic_region_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not create {}", strategic_region_file_name.string()));
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
   if (strategic_region.HasStaticModifiers())
   {
      strategic_region_file << "\tstatic_modifiers={\n";
      for (const auto& modifier: strategic_region.GetStaticModifiers())
      {
         strategic_region_file << "\t\t" << modifier.first << "=" << modifier.second << "\n";
      }
      strategic_region_file << "\n";
      strategic_region_file << "\t}\n";
   }
   auto naval_terrain = strategic_region.GetNavalTerrain();
   if (naval_terrain)
   {
      strategic_region_file << "\tnaval_terrain=" << *naval_terrain << "\n";
   }
   strategic_region_file << "\tweather" << strategic_region.GetWeather() << "\n";
   strategic_region_file << "}";

   strategic_region_file.close();
}