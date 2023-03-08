#include "src/hoi4_world/map/coastal_provinces_creator.h"

#include "external/commonItems/Log.h"



hoi4::CoastalProvinces hoi4::CreateCoastalProvinces(const maps::MapData& map_data,
    const std::set<std::string>& land_provinces,
    const std::set<std::string>& sea_provinces)
{
   Log(LogLevel::Info) << "\tInitializing coastal provinces";

   std::map<int, std::vector<int>> coastal_provinces;

   for (const auto& province_name: land_provinces)
   {
      auto neighbors = map_data.GetNeighbors(province_name);
      for (const auto& adjacent_province_number_string: neighbors)
      {
         if (!sea_provinces.contains(adjacent_province_number_string))
         {
            continue;
         }

         int province_number = 0;
         int adjacent_province_number = 0;
         try
         {
            province_number = std::stoi(province_name);
            adjacent_province_number = std::stoi(adjacent_province_number_string);
         }
         catch (...)
         {
            continue;
         }

         const auto [iterator, success] =
             coastal_provinces.emplace(province_number, std::vector{adjacent_province_number});
         if (!success)
         {
            iterator->second.push_back(adjacent_province_number);
         }
      }
   }

   return CoastalProvinces(coastal_provinces);
}