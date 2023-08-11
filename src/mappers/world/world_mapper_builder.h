#pragma once

#include "src/mappers/country/country_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/provinces/province_mapper_importer.h"
#include "src/mappers/provinces/province_mapping_types.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/mappers/world/world_mapper.h"
#include "src/vic3_world/world/vic3_world.h"
#include "src/vic3_world/world/vic3_world_builder.h"

namespace mappers
{

class WorldMapperBuilder
{
   std::map<int, std::string> country_mappings;
   Vic3ToHoi4ProvinceMapping vic_hoi_province_mappings;
   Hoi4ToVic3ProvinceMapping hoi_vic_province_mappings;
   std::vector<mappers::TechMapping> tech_mappings;
   CultureGraphicsMapper culture_graphics_mapper;
   WorldMapperBuilder(): culture_graphics_mapper({}) {}


  public:
   /// <summary>
   /// Create a mapper with null maps that return nothing.
   /// </summary>
   static WorldMapperBuilder CreateNullMapper();
   /// <summary>
   /// Create a mapper with the default maps that looks up data in the included files.
   /// </summary>
   /// <returns></returns>
   static WorldMapperBuilder CreateDefaultMapper(commonItems::ModFilesystem hoi4_mod_filesystem,
       const vic3::World& source_world);
   /// <summary>
   /// Use the default country mapper with the countries in the target source world.
   /// </summary>
   WorldMapperBuilder& DefaultCountryMapper(const vic3::World& source_world);
   /// <summary>
   /// Add one or more countries to the mapper. Element format is {vic3Num, hoiTag}
   /// </summary>
   WorldMapperBuilder& AddCountries(const std::map<int, std::string>& countries);
   /// <summary>
   /// Add one or more provinces to the mapper. Element format is {vic3string, hoiNum}. Only supports 1:1 mappings.
   /// </summary>
   WorldMapperBuilder& AddProvinces(const std::map<std::string, int>& provinces);
   /// <summary>
   /// Adds a series of provinces. 1:1 map between vic and hoi of style {"x0000nn",n*10}.
   /// </summary>
   WorldMapperBuilder& AddTestProvinces(int count);
   /// <summary>
   /// Sets tech manager to the default tech manager, since we don't usually change this.
   /// </summary>
   WorldMapperBuilder& DefaultTechMapper();
   /// <summary>
   /// Add one or more techs to the mapper. Element format is {{vic3reqs...}, (optional)limit, hoi4techs}
   /// </summary>
   WorldMapperBuilder& AddTechs(const std::vector<mappers::TechMapping>& techs);
   /// <summary>
   /// Sets culture graphics manager to the default culture graphics manager, since we don't usually change this.
   /// </summary>
   WorldMapperBuilder& DefaultCultureGraphicsMapper();
   /// <summary>
   /// Set culture graphics mapper. Must be constructed separately.
   /// </summary>
   WorldMapperBuilder& SetCultureGraphicsMapper(CultureGraphicsMapper mapper);

   /// <summary>
   /// Copies province data to a worldbuilder.
   /// </summary>
   /// <param name="world"></param>
   void CopyToVicWorld(vic3::WorldBuilder& world);
   WorldMapper Build();
};
}  // namespace mappers