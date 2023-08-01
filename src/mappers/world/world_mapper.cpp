#include "pch.h"
#include "world_mapper.h"
#include "src/mappers/country/country_mapper_creator.h"
#include <src/mappers/technology/tech_mapping.h>
#include <src/mappers/technology/tech_mappings_importer.h>
#include <src/mappers/culture/culture_graphics_mapper.h>
#include <src/mappers/culture/culture_graphics_mapper_importer.h>

namespace mappers
{
/// <summary>
/// Create a world_mapper from file info.
/// </summary>
/// <returns></returns>
world_mapper world_mapper::LoadFromFiles(commonItems::ModFilesystem hoi4_mod_filesystem,
    const vic3::World& source_world)
{
   const mappers::CountryMapper country_mapper =
       mappers::CreateCountryMappings("configurables/country_mappings.txt", source_world.GetCountries());
   const auto province_mapper = mappers::ProvinceMapperImporter(hoi4_mod_filesystem).ImportProvinceMappings();

   province_mapper.CheckAllVic3ProvincesMapped(source_world.GetProvinceDefinitions().GetProvinceDefinitions());

   const std::vector<mappers::TechMapping> tech_mappings = mappers::ImportTechMappings();
   const mappers::CultureGraphicsMapper culture_graphics_mapper =
       mappers::ImportCultureGraphicsMapper("configurables/culture_graphics.txt");

   return world_mapper(std::move(country_mapper),
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

world_mapper::world_mapper(const CountryMapper&& country_mapper,
    const ProvinceMapper&& province_mapper,
    const std::vector<mappers::TechMapping>&& tech_mapper,
    const CultureGraphicsMapper culture_graphics_mapper):
    country_mapper(std::move(country_mapper)),
    province_mapper(std::move(province_mapper)),
    tech_mapper(std::move(tech_mapper)),
    culture_graphics_mapper(std::move(culture_graphics_mapper))
{
}
}  // namespace mappers
