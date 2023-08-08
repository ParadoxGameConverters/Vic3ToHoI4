#include "src/hoi4_world/world/hoi4_world_framework.h"

#include <src/hoi4_world/map/resources_map_importer.h>
#include <src/hoi4_world/map/strategic_regions_importer.h>
#include <src/hoi4_world/states/default_states_importer.h>
#include <src/hoi4_world/states/state_categories.h>

namespace hoi4
{
WorldFrameworkBuilder WorldFrameworkBuilder::CreateDefaultWorldFramework(
    const commonItems::ModFilesystem& hoi4_mod_filesystem)
{
   WorldFrameworkBuilder wfb{};
   wfb.hoi4_mod_filesystem = hoi4_mod_filesystem;
   wfb.DefaultStrategicRegions();
   wfb.DefaultDefaultStates();
   wfb.DefaultResourcesMap();
   wfb.DefaultStateCategories();
   return wfb;
}

WorldFrameworkBuilder WorldFrameworkBuilder::CreateNullWorldFramework()
{
   WorldFrameworkBuilder wfb{};
   wfb.hoi4_mod_filesystem = commonItems::ModFilesystem("test_files/hoi4_world", {});
   wfb.strategic_regions = {};
   wfb.default_states = {};
   wfb.resources_map = {};
   wfb.state_categories = {};
   return wfb;
}
WorldFramework WorldFrameworkBuilder::Build()
{
   return WorldFramework(std::move(this->strategic_regions),
       std::move(this->default_states),
       std::move(this->resources_map),
       std::move(this->state_categories));
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultStrategicRegions()
{
   this->strategic_regions = ImportStrategicRegions(this->hoi4_mod_filesystem);
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::SetStrategicRegions(const StrategicRegions strategic_regions)
{
   this->strategic_regions = strategic_regions;
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultDefaultStates()
{
   this->default_states = hoi4::ImportDefaultStates(this->hoi4_mod_filesystem);
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::AddDefaultStates(const std::map<int, DefaultState> defaultStates)
{
   for (auto& state: defaultStates)
   {
      this->default_states.emplace(state);
   }
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultResourcesMap()
{
   this->resources_map = ImportResources("configurables/resources.txt");
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::AddResources(ResourcesMap resources)
{
   for (auto& resource: resources)
   {
      this->resources_map.emplace(resource);
   }
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultStateCategories()
{
   this->state_categories = StateCategories({
       {1, "pastoral"},
       {2, "rural"},
       {4, "town"},
       {5, "large_town"},
       {6, "city"},
       {8, "large_city"},
       {10, "metropolis"},
       {12, "megalopolis"},
   });
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::SetStateCategories(StateCategories new_categories)
{
   this->state_categories = new_categories;
   return *this;
}

// WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultXXX()

}  // namespace hoi4