#include "src/hoi4_world/map/strategic_regions_importer.h"

#include <external/commonItems/Log.h>

#include "src/hoi4_world/map/strategic_region_importer.h"



hoi4::StrategicRegions hoi4::ImportStrategicRegions(const commonItems::ModFilesystem& hoi4_mod_filesystem)
{
   Log(LogLevel::Info) << "\tImporting original strategic regions";
   StrategicRegionImporter strategic_region_importer;

   std::map<int, StrategicRegion> strategic_regions;
   std::map<int, int> province_to_strategic_region_map;

   for (const auto& filename: hoi4_mod_filesystem.GetAllFilesInFolder("map/strategicregions/"))
   {
      auto new_region = strategic_region_importer.ImportStrategicRegion(filename);
      const auto id = new_region.GetID();
      if (id == 0)
      {
         continue;
      }

      strategic_regions.emplace(id, new_region);

      for (auto province: new_region.GetOldProvinces())
      {
         province_to_strategic_region_map.emplace(province, id);
      }
   }

   return StrategicRegions({.strategic_regions = strategic_regions,
       .original_province_to_strategic_region_map = province_to_strategic_region_map});
}
