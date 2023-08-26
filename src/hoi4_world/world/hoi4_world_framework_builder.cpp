#include "src/hoi4_world/world/hoi4_world_framework_builder.h"

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/hoi4_world/map/coastal_provinces_creator.h"
#include "src/hoi4_world/map/hoi4_province_definition_importer.h"
#include "src/hoi4_world/map/resources_map_importer.h"
#include "src/hoi4_world/map/strategic_regions_importer.h"
#include "src/hoi4_world/states/default_states_importer.h"
#include "src/hoi4_world/states/state_categories.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/maps/map_data_importer.h"

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
   wfb.DefaultProvinceDefinitions();

   auto tmpMapData = maps::MapDataImporter(wfb.province_definitions_).ImportMapData(hoi4_mod_filesystem);
   wfb.DefaultCoastalProvinces(tmpMapData);
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
   wfb.province_definitions_ = {};
   return wfb;
}
WorldFramework WorldFrameworkBuilder::Build()
{
   return WorldFramework(std::move(this->strategic_regions_),
       std::move(this->default_states_),
       std::move(this->resources_map_),
       std::move(this->state_categories_),
       maps::ProvinceDefinitions{std::move(this->province_definitions_)},
       CoastalProvinces{std::move(this->coastal_provinces_)});
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

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultProvinceDefinitions()
{
   province_definitions_ = ImportProvinceDefinitions(hoi4_mod_filesystem_);
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::AddLandProvinces(std::vector<std::string> province_ids)
{
   for (const auto& id: province_ids)
   {
      province_definitions_.land_provinces.emplace(id);
   }
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::AddTestLandProvinces(int count)
{
   for (int i = 0; i < count; ++i)
   {
      test_province_number_ += 10;
      province_definitions_.land_provinces.emplace(fmt::format("{}", test_province_number_));
   }
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::AddSeaProvinces(std::vector<std::string> province_ids)
{
   for (const auto& id: province_ids)
   {
      province_definitions_.sea_provinces.emplace(id);
   }
   return *this;
}

maps::ProvinceDefinitions WorldFrameworkBuilder::CopyProvinceDefinitions()
{
   return maps::ProvinceDefinitions{this->province_definitions_};
}

WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultCoastalProvinces(const maps::MapData& map_data)
{
   this->coastal_provinces_ = CreateCoastalProvinces(map_data,
       this->province_definitions_.land_provinces,
       this->province_definitions_.sea_provinces);
   return *this;
}

WorldFrameworkBuilder& WorldFrameworkBuilder::AddCoastalProvinces(CoastalProvinces::storage_type provinces)
{
   for (const auto& province: provinces)
   {
      this->coastal_provinces_.emplace(province);
   }
   return *this;
}

// WorldFrameworkBuilder& WorldFrameworkBuilder::DefaultXXX()

}  // namespace hoi4