#pragma once
#include <future>
#include <map>
#include <string>
#include <vector>

#include "src/mappers/country/country_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/mappers/infrastructure/infrastructure_mapper.h"
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
   WorldMapper(const CountryMapper country_mapper,
       const ProvinceMapper province_mapper,
       const std::vector<mappers::TechMapping> tech_mapper,
       const CultureGraphicsMapper culture_graphics_mapper):
       country_mapper(std::move(country_mapper)),
       province_mapper(std::move(province_mapper)),
       tech_mapper(std::move(tech_mapper)),
       culture_graphics_mapper(std::move(culture_graphics_mapper))
   {
   }

   WorldMapper(const WorldMapper& world_mapper) = default;

   const CountryMapper country_mapper;
   const ProvinceMapper province_mapper;
   const std::vector<mappers::TechMapping> tech_mapper;
   const CultureGraphicsMapper culture_graphics_mapper;
};

}  // namespace mappers
