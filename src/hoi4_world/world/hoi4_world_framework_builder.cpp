#include "src/hoi4_world/world/hoi4_world_framework_builder.h"

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
   wfb.hoi4_mod_filesystem_ = hoi4_mod_filesystem;
   wfb.DefaultStrategicRegions();
   wfb.DefaultDefaultStates();
   wfb.DefaultResourcesMap();
   wfb.DefaultStateCategories();
   return wfb;
}

WorldFrameworkBuilder WorldFrameworkBuilder::CreateNullWorldFramework()
{
   WorldFrameworkBuilder wfb{};
   wfb.hoi4_mod_filesystem_ = commonItems::ModFilesystem("test_files/hoi4_world", {});
   wfb.strategic_regions_ = {};
   wfb.default_states_ = {};
   wfb.resources_map_ = {};
   wfb.state_categories_ = {};
   return wfb;
}
WorldFramework WorldFrameworkBuilder::Build()
{
   return WorldFramework(std::move(this->strategic_regions_),
       std::move(this->default_states_),
       std::move(this->resources_map_),
       std::move(this->state_categories_));
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultStrategicRegions()
{
   this->strategic_regions_ = ImportStrategicRegions(this->hoi4_mod_filesystem_);
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::SetStrategicRegions(const StrategicRegions strategic_regions)
{
   this->strategic_regions_ = strategic_regions;
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultDefaultStates()
{
   this->default_states_ = hoi4::ImportDefaultStates(this->hoi4_mod_filesystem_);
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::AddDefaultStates(const std::map<int, DefaultState> defaultStates)
{
   for (auto& state: defaultStates)
   {
      this->default_states_.emplace(state);
   }
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultResourcesMap()
{
   this->resources_map_ = ImportResources("configurables/resources.txt");
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::AddResources(ResourcesMap resources)
{
   for (auto& resource: resources)
   {
      this->resources_map_.emplace(resource);
   }
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultStateCategories()
{
   this->state_categories_ = StateCategories({
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
   this->state_categories_ = new_categories;
   return *this;
}

// WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultXXX()

}  // namespace hoi4