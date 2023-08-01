#pragma once
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper_importer.h"
#include "src/vic3_world/world/vic3_world.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/mappers/culture/culture_graphics_mapper.h"

namespace mappers
{
/// <summary>
/// Container class for all the other mappers.
/// </summary>
class world_mapper
{
  public:
   static world_mapper LoadFromFiles(commonItems::ModFilesystem hoi4_mod_filesystem, const vic3::World& source_world);
   //world_mapper();
   world_mapper(const CountryMapper&& country_mapper,
       const ProvinceMapper&& provice_mapper,
       const std::vector<mappers::TechMapping>&& tech_mapper,
       const CultureGraphicsMapper culture_graphics_mapper);

   const CountryMapper country_mapper;
   const ProvinceMapper province_mapper;
   const std::vector<mappers::TechMapping> tech_mapper;
   const CultureGraphicsMapper culture_graphics_mapper;
};

class WorldMapperBuilder
{

};

}  // namespace mappers
