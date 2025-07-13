#pragma once



#include <filesystem>

#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/default_state.h"
#include "src/hoi4_world/states/state_categories.h"
#include "src/hoi4_world/world/hoi4_world_framework.h"
#include "src/maps/map_data.h"
#include "src/maps/province_definitions.h"



// This represents the input hoi4 elements (eg, state maps, resource maps, etc)
// that are used in conjunction with the vic3 data  and the WorldMapper to create the final hoi4 mod.

namespace hoi4
{

class WorldFrameworkBuilder
{
  public:
   static WorldFrameworkBuilder CreateDefaultWorldFramework(const commonItems::ModFilesystem& hoi4_mod_filesystem);
   static WorldFrameworkBuilder CreateNullWorldFramework();
   WorldFramework Build();

   WorldFrameworkBuilder& DefaultStrategicRegions();
   WorldFrameworkBuilder& SetStrategicRegions(const StrategicRegions strategic_regions);
   WorldFrameworkBuilder& DefaultDefaultStates();
   WorldFrameworkBuilder& AddDefaultStates(const std::map<int, DefaultState> default_states);
   WorldFrameworkBuilder& AddResources(ResourcesMap resources);
   WorldFrameworkBuilder& DefaultResourcesMap();
   WorldFrameworkBuilder& DefaultStateCategories();
   WorldFrameworkBuilder& SetStateCategories(StateCategories new_categories);
   WorldFrameworkBuilder& DefaultProvinceDefinitions();
   WorldFrameworkBuilder& AddLandProvinces(std::vector<std::string> province_ids);
   /// <summary>
   /// Add land provinces, in the form of "10","20","30",...
   /// </summary>
   /// <param name="count"></param>
   /// <returns></returns>
   WorldFrameworkBuilder& AddTestLandProvinces(int count);
   WorldFrameworkBuilder& AddSeaProvinces(std::vector<std::string> province_ids);
   // for intermediate test stuff
   maps::ProvinceDefinitions CopyProvinceDefinitions();



   // require other data first
   // requires province_definitions
   WorldFrameworkBuilder& DefaultMapData();
   WorldFrameworkBuilder& SetMapData(const maps::MapData& map_data);
   // requires map_data, province_definitions
   WorldFrameworkBuilder& DefaultCoastalProvinces();
   /// <summary>
   /// Add coastal provinces. format is {land_province_id, {adjacent_sea_province_id,...}}
   /// </summary>
   /// <param name="provinces"></param>
   /// <returns></returns>
   WorldFrameworkBuilder& AddCoastalProvinces(const std::map<int, std::vector<int>>& provinces);

  private:
   WorldFrameworkBuilder(): hoi4_mod_filesystem_("", {}) {}
   commonItems::ModFilesystem hoi4_mod_filesystem_;
   StrategicRegions strategic_regions_;
   std::map<int, DefaultState> default_states_;
   ResourcesMap resources_map_;
   StateCategories state_categories_;
   int test_province_number_ = 0;
   maps::ProvinceDefinitionsOptions province_definitions_;
   maps::MapData map_data_;

   // requires province_definitions_
   std::map<int, std::vector<int>> coastal_provinces_;
};
}  // namespace hoi4