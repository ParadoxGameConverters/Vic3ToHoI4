#include "src/hoi4_world/map/hoi4_provinces.h"

#include <fstream>

#include "external/commonItems/Log.h"
#include "external/fmt/include/fmt/format.h"



std::map<int, hoi4::Province> hoi4::ImportProvinces(const commonItems::ModFilesystem& mod_filesystem)
{
   Log(LogLevel::Info) << "\tImporting HoI4 province definitions";
   const auto definitions_location = mod_filesystem.GetActualFileLocation("/map/definition.csv");
   if (!definitions_location)
   {
      throw std::runtime_error("Could not find /map/definition.csv");
   }

   std::ifstream province_definitions(*definitions_location);
   if (!province_definitions.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}/map/definition.csv", *definitions_location));
   }

   std::map<int, Province> provinces;
   while (!province_definitions.eof())
   {
      std::string line;
      getline(province_definitions, line);
      if (line.length() == 0)
      {
         break;
      }

      auto id_separator = line.find_first_of(';');
      auto id = stoi(line.substr(0, id_separator));
      if (id == 0)
      {
         continue;
      }
      line = line.substr(id_separator + 1, line.size());

      auto red_separator = line.find_first_of(';');
      line = line.substr(red_separator + 1, line.size());

      auto green_separator = line.find_first_of(';');
      line = line.substr(green_separator + 1, line.size());

      auto blue_separator = line.find_first_of(';');
      line = line.substr(blue_separator + 1, line.size());

      auto land_sea_separator = line.find_first_of(';');
      auto land_or_sea = line.substr(0, land_sea_separator);
      auto is_land = (land_or_sea == "land");
      line = line.substr(land_sea_separator + 1, line.size());

      auto bool_separator = line.find_first_of(';');
      line = line.substr(bool_separator + 1, line.size());

      auto type_separator = line.find_first_of(';');
      auto type = line.substr(0, type_separator);

      Province new_province(is_land, type);
      provinces.emplace(id, new_province);
   }

   return provinces;
}
