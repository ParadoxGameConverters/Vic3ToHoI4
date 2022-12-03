#include "src/maps/province_definitions.h"

#include "src/maps/utility.h"



std::optional<std::string> maps::ProvinceDefinitions::GetProvinceFromColor(const commonItems::Color& color) const
{
   const auto color_int = GetIntFromColor(color);

   if (const auto mapping = color_to_province_map_.find(color_int); mapping != color_to_province_map_.end())
   {
      return mapping->second;
   }

   return std::nullopt;
}


std::optional<std::string> maps::ProvinceDefinitions::GetTerrainType(const std::string& province) const
{
   if (const auto terrain = terrain_types_.find(province); terrain != terrain_types_.end())
   {
      return terrain->second;
   }

   return std::nullopt;
}