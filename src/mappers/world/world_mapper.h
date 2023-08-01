#pragma once
#include <string>
#include <vector>
#include <map>

#include "src/mappers/country/country_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/provinces/province_mapper_importer.h"
#include "src/mappers/provinces/province_mapping_types.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/vic3_world/world/vic3_world.h"

namespace mappers
{
/// <summary>
/// Container class for all the other mappers.
/// </summary>
class WorldMapper
{
  public:
   static WorldMapper LoadFromFiles(commonItems::ModFilesystem hoi4_mod_filesystem, const vic3::World& source_world);

   WorldMapper(const CountryMapper&& country_mapper,
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
   std::map<int, std::string> country_mappings;
   Vic3ToHoi4ProvinceMapping vic_hoi_province_mappings;
   Hoi4ToVic3ProvinceMapping hoi_vic_province_mappings;
   std::vector<mappers::TechMapping> tech_mappings;
   CultureGraphicsMapper culture_graphics_mapper;
   WorldMapperBuilder();

  public:
   /// <summary>
   /// Create a mapper with null maps that return nothing.
   /// </summary>
   static WorldMapperBuilder NullMapper();
   /// <summary>
   /// Add one or more countries to the mapper. Element format is {vic3Num, hoiTag}
   /// </summary>
   WorldMapperBuilder AddCountries(std::map<int, std::string> countries);
   /// <summary>
   /// Add one or more provinces to the mapper. Element format is {vic3string, hoiNum}. Only supports 1:1 mappings.
   /// </summary>
   WorldMapperBuilder AddProvinces(std::map<std::string, int> provinces);
   /// <summary>
   /// Add one or more techs to the mapper.
   /// </summary>
   WorldMapperBuilder AddTechs(std::vector<mappers::TechMapping> techs);
   /// <summary>
   /// Set culture graphics mapper. Must be constructed separately.
   /// </summary>
   WorldMapperBuilder SetCultureGraphicsMapper(CultureGraphicsMapper mapper);
   WorldMapper Build();
};

}  // namespace mappers
