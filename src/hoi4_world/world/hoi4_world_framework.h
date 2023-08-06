#pragma once
#include <src/hoi4_world/map/strategic_regions.h>
#include <src/hoi4_world/states/default_state.h>
#include <src/hoi4_world/states/state_categories.h>

// This represents the input hoi4 elements (eg, state maps, resource maps, etc)
// that are used in conjunction with the vic3 data  and the WorldMapper to create the final hoi4 mod.

namespace hoi4
{
class WorldFramework
{
  public:
   WorldFramework(StrategicRegions&& strategic_regions,
       std::map<int, DefaultState>&& default_states,
       ResourcesMap&& resources_map,
       StateCategories&& state_categories):
       strategic_regions(std::move(strategic_regions)),
       default_states(std::move(default_states)),
       resources_map(std::move(resources_map)),
       state_categories(std::move(state_categories))
   {
   }

   StrategicRegions strategic_regions;
   const std::map<int, DefaultState> default_states;
   const ResourcesMap resources_map;
   const StateCategories state_categories;
};


class WorldFrameworkBuilder
{
  public:
   static WorldFrameworkBuilder CreateDefaultWorldFramework(const commonItems::ModFilesystem& hoi4_mod_filesystem);
   static WorldFrameworkBuilder CreateNullWorldFramework();
   WorldFramework Build();

   WorldFrameworkBuilder& DefaultStrategicRegions();
   WorldFrameworkBuilder& SetStrategicRegions(const StrategicRegions strategic_regions);
   WorldFrameworkBuilder& DefaultDefaultStates();
   WorldFrameworkBuilder& AddDefaultStates(const std::map<int, DefaultState> defaultStates);
   WorldFrameworkBuilder& DefaultResourcesMap();
   WorldFrameworkBuilder& DefaultStateCategories();


  private:
   WorldFrameworkBuilder(): hoi4_mod_filesystem("", {}){};
   commonItems::ModFilesystem hoi4_mod_filesystem;
   StrategicRegions strategic_regions;
   std::map<int, DefaultState> default_states;
   ResourcesMap resources_map;
   StateCategories state_categories;
};
}  // namespace hoi4