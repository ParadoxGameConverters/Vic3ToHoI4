#include "src/hoi4_world/map/hoi4_province_definition_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Parser.h>
#include <external/fmt/include/fmt/format.h>

#include <fstream>

#include "src/maps/utility.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"



namespace
{

std::map<int, std::string> ImportContinentDefinitions(const commonItems::ModFilesystem& mod_filesystem)
{
   std::map<int, std::string> continent_definitions;
   int continent_index = 1;

   commonItems::parser continent_parser;
   continent_parser.registerRegex(commonItems::catchallRegex,
       [&continent_definitions, &continent_index](const std::string& continent, [[maybe_unused]] std::istream& stream) {
          continent_definitions.emplace(continent_index, continent);
          ++continent_index;
       });

   commonItems::parser continents_parser;
   continents_parser.registerKeyword("continents", [&continent_parser](std::istream& stream) {
      continent_parser.parseStream(stream);
   });

   const auto path = mod_filesystem.GetActualFileLocation("map/continent.txt");
   if (!path)
   {
      throw std::runtime_error("Could not find /map/continent.txt");
   }
   continents_parser.parseFile(*path);

   return continent_definitions;
}

}  // namespace


maps::ProvinceDefinitionsOptions hoi4::ImportProvinceDefinitions(const commonItems::ModFilesystem& mod_filesystem)
{
   const std::map<int, std::string> continent_definitions = ImportContinentDefinitions(mod_filesystem);

   const auto path = mod_filesystem.GetActualFileLocation("map/definition.csv");
   if (!path)
   {
      throw std::runtime_error("Could not find /map/definition.csv");
   }

   std::ifstream definitions(*path);
   if (!definitions.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}/map/definition.csv", path->string()));
   }

   std::set<std::string> land_provinces;
   std::set<std::string> sea_provinces;
   std::map<std::string, std::string> terrain_types;
   std::map<std::string, std::string> continents;
   std::map<int, std::string> color_to_province_map;

   while (true)
   {
      if (definitions.eof())
      {
         break;
      }
      std::string line;
      getline(definitions, line);
      if (line.empty())
      {
         continue;
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

      const int continent_number = std::stoi(line);
      if (const auto itr = continent_definitions.find(continent_number); itr != continent_definitions.end())
      {
         continents.emplace(province_name, itr->second);
      }
   }

   return {.land_provinces = land_provinces,
       .sea_provinces = sea_provinces,
       .terrain_types = terrain_types,
       .continents = continents,
       .color_to_province_map = color_to_province_map};
}
