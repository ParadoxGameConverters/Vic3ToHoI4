#include "src/mappers/provinces/province_mapper.h"

#include "external/commonItems/Log.h"


std::vector<int> mappers::ProvinceMapper::GetVic3ToHoi4ProvinceMapping(std::string_view vic3_province) const
{
   const auto mapping = vic3_to_hoi4_province_map_.find(std::string(vic3_province));
   if (mapping == vic3_to_hoi4_province_map_.end())
   {
      Log(LogLevel::Warning) << "No mapping found for Vic3 province " << vic3_province;
      return {};
   }

   return mapping->second;
}


std::vector<std::string> mappers::ProvinceMapper::GetHoi4ToVic3ProvinceMapping(const int hoi4_province) const
{
   const auto mapping = hoi4_to_vic3_province_map_.find(hoi4_province);
   if (mapping == hoi4_to_vic3_province_map_.end())
   {
      Log(LogLevel::Warning) << "No mapping found for Hoi4 province " << hoi4_province;
      return {};
   }

   return mapping->second;
}


std::set<std::string> mappers::ProvinceMapper::GetEquivalentVic3Provinces(const std::set<int>& hoi4_provinces) const
{
   std::set<std::string> equivalent_provinces;

   for (const auto hoi4_province: hoi4_provinces)
   {
      const auto mapping = hoi4_to_vic3_province_map_.find(hoi4_province);
      if (mapping == hoi4_to_vic3_province_map_.end())
      {
         continue;
      }

      for (const auto& vic3_province: mapping->second)
      {
         equivalent_provinces.insert(vic3_province);
      }
   }

   return equivalent_provinces;
}


void mappers::ProvinceMapper::CheckAllVic3ProvincesMapped(const std::vector<std::string>& vic3_provinces) const
{
   for (const auto& vic3_province: vic3_provinces)
   {
      if (!vic3_to_hoi4_province_map_.contains(vic3_province))
      {
         Log(LogLevel::Warning) << "No mapping for Vic3 province " << vic3_province;
      }
   }
}