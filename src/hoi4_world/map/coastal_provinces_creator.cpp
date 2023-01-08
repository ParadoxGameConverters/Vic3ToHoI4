#include "src/hoi4_world/map/coastal_provinces_creator.h"

#include "external/commonItems/Log.h"



hoi4::CoastalProvinces hoi4::CreateCoastalProvinces(const maps::MapData& map_data,
    const std::map<int, Province>& provinces)
{
   Log(LogLevel::Info) << "\tInitializing coastal provinces";

   std::map<int, std::vector<int>> coastal_provinces;

   for (const auto& [province_number, province]: provinces)
   {
      if (!province.is_land)
      {
         continue;
      }

      auto neighbors = map_data.GetNeighbors(std::to_string(province_number));
      for (const auto& adjacent_province_number_string: neighbors)
      {
         int adjacent_province_number = 0;
         try
         {
            adjacent_province_number = std::stoi(adjacent_province_number_string);
         }
         catch (...)
         {
            continue;
         }

         if (auto adjacent_province = provinces.find(adjacent_province_number);
             (adjacent_province != provinces.end()) && (adjacent_province->second.type == "ocean"))
         {
            const auto [iterator, success] =
                coastal_provinces.emplace(province_number, std::vector{adjacent_province_number});
            if (!success)
            {
               iterator->second.push_back(adjacent_province_number);
            }
         }
      }
   }

   return {coastal_provinces};
}