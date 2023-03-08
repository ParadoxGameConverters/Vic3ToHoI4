#include "src/hoi4_world/map/hoi4_province_definition_importer.h"

#include <fstream>

#include "external/fmt/include/fmt/format.h"
#include "src/maps/utility.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"


maps::ProvinceDefinitions hoi4::ImportProvinceDefinitions(const commonItems::ModFilesystem& mod_filesystem)
{
   const auto path = mod_filesystem.GetActualFileLocation("/map/definition.csv");
   if (!path)
   {
      throw std::runtime_error("Could not find /map/definition.csv");
   }

   std::ifstream definitions(*path);
   if (!definitions.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}/map/definition.csv", *path));
   }

   std::set<std::string> land_provinces;
   std::set<std::string> sea_provinces;
   std::map<std::string, std::string> terrain_types;
   std::map<int, std::string> color_to_province_map;

   while (true)
   {
      std::string line;
      getline(definitions, line);
      if (line.empty())
      {
         break;
      }

      // number
      auto pos = line.find_first_of(';');
      if (pos == std::string::npos)
      {
         continue;
      }
      auto province_name = line.substr(0, pos);
      if (province_name == "0")
      {
         continue;
      }
      line = line.substr(pos + 1U, line.length());

      // red
      pos = line.find_first_of(';');
      if (pos == std::string::npos)
      {
         continue;
      }
      const int red(std::stoi(line.substr(0, pos)));
      line = line.substr(pos + 1, line.length());

      // green
      pos = line.find_first_of(';');
      if (pos == std::string::npos)
      {
         continue;
      }
      const int green(std::stoi(line.substr(0, pos)));
      line = line.substr(pos + 1U, line.length());

      // blue
      pos = line.find_first_of(';');
      if (pos == std::string::npos)
      {
         continue;
      }
      const int blue(std::stoi(line.substr(0, pos)));
      line = line.substr(pos + 1U, line.length());

      // land or sea
      pos = line.find_first_of(';');
      if (pos == std::string::npos)
      {
         continue;
      }
      const std::string land_or_sea = line.substr(0, pos);
      line = line.substr(pos + 1U, line.length());

      // false or true
      pos = line.find_first_of(';');
      if (pos == std::string::npos)
      {
         continue;
      }
      line = line.substr(pos + 1, line.length());

      // terrain
      pos = line.find_first_of(';');
      if (pos == std::string::npos)
      {
         continue;
      }
      const std::string terrain = line.substr(0, pos);
      line = line.substr(pos + 1U, line.length());

      auto color_int = maps::GetIntFromColor(commonItems::Color(std::array{red, green, blue}));
      color_to_province_map.emplace(color_int, province_name);
      if (land_or_sea == "land")
      {
         land_provinces.insert(province_name);
      }
      else if (land_or_sea == "sea")
      {
         sea_provinces.insert(province_name);
      }
      terrain_types.emplace(province_name, terrain);
   }

   return maps::ProvinceDefinitions({.land_provinces = land_provinces,
       .sea_provinces = sea_provinces,
       .terrain_types = terrain_types,
       .color_to_province_map = color_to_province_map});
}
