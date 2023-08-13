#pragma once
#include <src/hoi4_world/map/strategic_regions.h>
#include <src/hoi4_world/states/default_state.h>
#include <src/hoi4_world/states/state_categories.h>
#include <src/hoi4_world/world/hoi4_world_framework.h>

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
   WorldFrameworkBuilder& AddDefaultStates(const std::map<int, DefaultState> defaultStates);
   WorldFrameworkBuilder& AddResources(ResourcesMap resources);
   WorldFrameworkBuilder& DefaultResourcesMap();
   WorldFrameworkBuilder& DefaultStateCategories();
   WorldFrameworkBuilder& SetStateCategories(StateCategories new_categories);


  private:
   WorldFrameworkBuilder(): hoi4_mod_filesystem_("", {}){};
   commonItems::ModFilesystem hoi4_mod_filesystem_;
   StrategicRegions strategic_regions_;
   std::map<int, DefaultState> default_states_;
   ResourcesMap resources_map_;
   StateCategories state_categories_;
};
}  // namespace hoi4