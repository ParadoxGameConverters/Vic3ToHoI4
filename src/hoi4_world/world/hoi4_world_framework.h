#pragma once
#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/default_state.h"
#include "src/hoi4_world/states/state_categories.h"
#include "src/maps/map_data.h"
#include "src/maps/province_definitions.h"

namespace hoi4
{


// This represents the input hoi4 elements (eg, state maps, resource maps, etc)
// that are used in conjunction with the vic3 data  and the WorldMapper to create the final hoi4 mod.
class WorldFramework
{
  public:
   WorldFramework(StrategicRegions&& strategic_regions,
       std::map<int, DefaultState>&& default_states,
       ResourcesMap&& resources_map,
       StateCategories&& state_categories,
       maps::ProvinceDefinitions&& province_definitions,
       maps::MapData&& map_data,
       CoastalProvinces&& coastal_provinces):
       strategic_regions(std::move(strategic_regions)),
       default_states(std::move(default_states)),
       resources_map(std::move(resources_map)),
       state_categories(std::move(state_categories)),
       province_definitions(std::move(province_definitions)),
       map_data(std::move(map_data)),
       coastal_provinces(std::move(coastal_provinces))
   {
   }

   StrategicRegions strategic_regions;
   const std::map<int, DefaultState> default_states;
   const ResourcesMap resources_map;
   const StateCategories state_categories;
   const maps::ProvinceDefinitions province_definitions;
   const maps::MapData map_data;
   const CoastalProvinces coastal_provinces;
};

}  // namespace hoi4