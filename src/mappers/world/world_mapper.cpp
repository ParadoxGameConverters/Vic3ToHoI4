#include "world_mapper.h"

#include <src/mappers/culture/culture_graphics_mapper.h>
#include <src/mappers/culture/culture_graphics_mapper_importer.h>
#include <src/mappers/technology/tech_mapping.h>
#include <src/mappers/technology/tech_mappings_importer.h>

#include "pch.h"
#include "src/mappers/country/country_mapper_creator.h"

namespace mappers
{
/// <summary>
/// Create a world_mapper from file info.
/// </summary>
/// <returns></returns>
WorldMapper WorldMapper::LoadFromFiles(commonItems::ModFilesystem hoi4_mod_filesystem, const vic3::World& source_world)
{
   const mappers::CountryMapper country_mapper =
       mappers::CreateCountryMappings("configurables/country_mappings.txt", source_world.GetCountries());
   const auto province_mapper = mappers::ProvinceMapperImporter(hoi4_mod_filesystem).ImportProvinceMappings();

   province_mapper.CheckAllVic3ProvincesMapped(source_world.GetProvinceDefinitions().GetProvinceDefinitions());

   const std::vector<mappers::TechMapping> tech_mappings = mappers::ImportTechMappings();
   const mappers::CultureGraphicsMapper culture_graphics_mapper =
       mappers::ImportCultureGraphicsMapper("configurables/culture_graphics.txt");

   return WorldMapper(std::move(country_mapper),
       std::move(province_mapper),
       std::move(tech_mappings),
       std::move(culture_graphics_mapper));
}

///// <summary>
///// Create a default-valued world_mapper that basically does nothing
///// </summary>
// world_mapper::world_mapper(): country_mapper(), province_mapper()
//{
// }

WorldMapper::WorldMapper(const CountryMapper&& country_mapper,
    const ProvinceMapper&& province_mapper,
    const std::vector<mappers::TechMapping>&& tech_mapper,
    const CultureGraphicsMapper culture_graphics_mapper):
    country_mapper(std::move(country_mapper)),
    province_mapper(std::move(province_mapper)),
    tech_mapper(std::move(tech_mapper)),
    culture_graphics_mapper(std::move(culture_graphics_mapper))
{
}

WorldMapperBuilder::WorldMapperBuilder(): culture_graphics_mapper({})
{
}

WorldMapperBuilder WorldMapperBuilder::NullMapper()
{
   WorldMapperBuilder builder = WorldMapperBuilder();
   builder.country_mappings = std::map<int, std::string>();
   builder.hoi_vic_province_mappings = {};
   builder.vic_hoi_province_mappings = {};
   builder.tech_mappings = {};
   builder.culture_graphics_mapper = CultureGraphicsMapper({});
   return builder;
}

WorldMapper WorldMapperBuilder::Build()
{
   return WorldMapper(std::move(CountryMapper(this->country_mappings)),
       std::move(ProvinceMapper(this->vic_hoi_province_mappings, this->hoi_vic_province_mappings)),
       std::move(tech_mappings),
       std::move(this->culture_graphics_mapper));
}

WorldMapperBuilder WorldMapperBuilder::AddCountries(std::map<int, std::string> countries)
{
   for (auto& el : countries)
   {
      this->country_mappings.insert(el);
   }
   return *this;
}

WorldMapperBuilder WorldMapperBuilder::AddProvinces(std::map<std::string, int> provinces)
{
   for (auto& el : provinces)
   {
      std::pair<int, std::vector<std::string>> other = {el.second, {el.first}};
      this->hoi_vic_province_mappings.insert(other);
      this->vic_hoi_province_mappings.insert(el);
   }

   return *this;
}

WorldMapperBuilder WorldMapperBuilder::AddTechs(std::vector<mappers::TechMapping> techs)
{
   for (auto& el: techs)
   {
      this->tech_mappings.push_back(el);
   }

   return *this;
}

WorldMapperBuilder WorldMapperBuilder::SetCultureGraphicsMapper(CultureGraphicsMapper mapper)
{
   this->culture_graphics_mapper = mapper;
   return *this;
}
}  // namespace mappers
