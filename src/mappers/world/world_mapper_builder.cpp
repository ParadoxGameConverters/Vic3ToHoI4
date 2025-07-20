#include "world_mapper_builder.h"

#include <external/fmt/include/fmt/format.h>
#include <src/hoi4_world/states/hoi4_states_converter.h>

#include "src/mappers/country/country_mapper_creator.h"
#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper_importer.h"
#include "src/mappers/resources/resource_mapper_importer.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/mappers/technology/tech_mappings_importer.h"
#include "src/vic3_world/world/vic3_world.h"
#include "src/vic3_world/world/vic3_world_builder.h"

namespace mappers
{

WorldMapperBuilder WorldMapperBuilder::CreateDefaultMapper(commonItems::ModFilesystem hoi4_mod_filesystem,
    const vic3::World& source_world)
{
   auto province_mapper =
       mappers::ProvinceMapperImporter(hoi4_mod_filesystem, source_world.GetStateRegions()).ImportProvinceMappings();


   WorldMapperBuilder builder = WorldMapperBuilder();
   builder.DefaultCountryMapper(source_world);
   builder.hoi_vic_province_mappings_ = province_mapper.GetHoi4ToVic3ProvinceMappings();
   builder.vic_hoi_province_mappings_ = province_mapper.GetVic3ToHoi4ProvinceMappings();
   builder.DefaultTechMapper();
   builder.DefaultCultureGraphicsMapper();
   builder.LoadResourceMapper("configurables/resource_mappings.txt");
   return builder;
}

WorldMapperBuilder WorldMapperBuilder::CreateNullMapper()
{
   WorldMapperBuilder builder = WorldMapperBuilder();
   builder.country_mappings_ = std::map<int, std::string>();
   builder.hoi_vic_province_mappings_ = {};
   builder.vic_hoi_province_mappings_ = {};
   builder.tech_mappings_ = {};
   builder.culture_graphics_mapper_ = CultureGraphicsMapper({});
   builder.resource_mapper_ = ResourceMapper({});
   return builder;
}

WorldMapper WorldMapperBuilder::Build()
{
   return {
       std::move(CountryMapper(this->country_mappings_)),
       std::move(ProvinceMapper(this->vic_hoi_province_mappings_, this->hoi_vic_province_mappings_)),
       std::move(tech_mappings_),
       std::move(this->culture_graphics_mapper_),
       std::move(this->resource_mapper_),
   };
}

WorldMapperBuilder& WorldMapperBuilder::DefaultCountryMapper(const vic3::World& source_world)
{
   const mappers::CountryMapper country_mapper =
       mappers::CreateCountryMappings("configurables/country_mappings.txt", source_world.GetCountries());
   this->country_mappings_ = country_mapper.GetCountryMappings();
   return *this;
}

WorldMapperBuilder& WorldMapperBuilder::AddCountries(const std::map<int, std::string>& countries)
{
   for (auto& el: countries)
   {
      this->country_mappings_.insert(el);
   }

   return *this;
}

WorldMapperBuilder& WorldMapperBuilder::AddProvinces(const std::map<std::string, int>& provinces)
{
   for (auto& el: provinces)
   {
      // need to wrap second in an extra set of {} to make el.second a list element and not a size
      std::pair<std::string, std::vector<int>> vic_hoi_elem = {el.first, {el.second}};
      std::pair<int, std::vector<std::string>> hoi_vic_elem = {el.second, {el.first}};
      this->hoi_vic_province_mappings_.insert(hoi_vic_elem);
      this->vic_hoi_province_mappings_.insert(vic_hoi_elem);
   }

   return *this;
}

WorldMapperBuilder& WorldMapperBuilder::AddTestProvinces(int count)
{
   for (int i = 1; i <= count; ++i)
   {
      const std::string vic_value = fmt::format("x0000{:0>2}", i);
      // need to wrap second in an extra set of {} to make el.second a list element and not a size
      std::pair<std::string, std::vector<int>> vic_hoi_elem = {vic_value, {i * 10}};
      std::pair<int, std::vector<std::string>> hoi_vic_elem = {i * 10, {vic_value}};
      this->hoi_vic_province_mappings_.insert(hoi_vic_elem);
      this->vic_hoi_province_mappings_.insert(vic_hoi_elem);
   }

   return *this;
}

WorldMapperBuilder& WorldMapperBuilder::AddTechs(const std::vector<mappers::TechMapping>& techs)
{
   for (auto& el: techs)
   {
      this->tech_mappings_.push_back(el);
   }

   return *this;
}

WorldMapperBuilder& WorldMapperBuilder::SetCultureGraphicsMapper(CultureGraphicsMapper mapper)
{
   this->culture_graphics_mapper_ = mapper;
   return *this;
}

WorldMapperBuilder& WorldMapperBuilder::LoadResourceMapper(const std::string& resource_mapping)
{
   this->resource_mapper_ = mappers::ImportResourceMapper(resource_mapping);
   return *this;
}


WorldMapperBuilder& WorldMapperBuilder::DefaultCultureGraphicsMapper()
{
   this->culture_graphics_mapper_ = mappers::ImportCultureGraphicsMapper("configurables/culture_graphics.txt");
   return *this;
}

WorldMapperBuilder& WorldMapperBuilder::DefaultTechMapper()
{
   this->tech_mappings_ = mappers::ImportTechMappings();
   return *this;
}

void WorldMapperBuilder::CopyToVicWorld(vic3::WorldBuilder& world)
{
   for (auto& pair: this->vic_hoi_province_mappings_)
   {
      world.AddProvinces({pair.first});
   }
}
}  // namespace mappers
