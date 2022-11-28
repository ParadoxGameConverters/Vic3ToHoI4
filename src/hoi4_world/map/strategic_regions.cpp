#include "src/hoi4_world/map/strategic_regions.h"

#include <algorithm>

#include "external/commonItems/Log.h"



namespace
{

std::map<int, int> DetermineUsedRegions(const hoi4::State& state, std::map<int, int>& province_to_strategic_region_map)
{
   std::map<int, int> used_regions;  // region ID -> number of provinces in that region

   for (auto province: state.GetProvinces())
   {
      auto mapping = province_to_strategic_region_map.find(province);
      if (mapping == province_to_strategic_region_map.end())
      {
         Log(LogLevel::Warning) << "Province " << province << " had no original strategic region";
         continue;
      }

      auto [iterator, success] = used_regions.emplace(mapping->second, 1);
      if (!success)
      {
          ++iterator->second;
      }
      province_to_strategic_region_map.erase(mapping);
   }

   return used_regions;
}


std::optional<int> DetermineMostUsedRegion(const std::map<int, int>& used_regions)
{
   if (!used_regions.empty())
   {
      return std::max_element(used_regions.begin(),
          used_regions.end(),
          [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
             return a.second < b.second;
          })
          ->first;
   }

   return std::nullopt;
}


void AddProvinceToRegion(int regionNumber, int provinceId, std::map<int, hoi4::StrategicRegion>& strategic_regions)
{
   auto region = strategic_regions.find(regionNumber);
   if (region == strategic_regions.end())
   {
      Log(LogLevel::Warning) << "Strategic region " << regionNumber << " was not in the list of regions.";
      return;
   }

   region->second.AddNewProvince(provinceId);
}


void AddLeftoverProvincesToRegions(std::map<int, int>& province_to_strategic_region_map,
    std::map<int, hoi4::StrategicRegion>& strategic_regions)
{
   for (const auto& mapping: province_to_strategic_region_map)
   {
      AddProvinceToRegion(mapping.second, mapping.first, strategic_regions);
   }
}


void AddProvincesToRegion(int regionNumber,
    const hoi4::State& state,
    std::map<int, hoi4::StrategicRegion>& strategic_regions)
{
   for (const auto& province: state.GetProvinces())
   {
      AddProvinceToRegion(regionNumber, province, strategic_regions);
   }
}

}  // namespace



void hoi4::StrategicRegions::UpdateToMatchNewStates(const std::vector<State>& states)
{
   Log(LogLevel::Info) << "\t\tUpdating strategic regions";
   for (const auto& state: states)
   {
      const auto used_regions = DetermineUsedRegions(state, province_to_strategic_region_map_);
      const auto best_region = DetermineMostUsedRegion(used_regions);
      if (best_region)
      {
         AddProvincesToRegion(*best_region, state, strategic_regions_);
      }
   }

   AddLeftoverProvincesToRegions(province_to_strategic_region_map_, strategic_regions_);

   province_to_strategic_region_map_.clear();
   for (const auto& [region_number, region]: strategic_regions_)
   {
      for (auto province: region.GetNewProvinces())
      {
         province_to_strategic_region_map_.emplace(province, region_number);
      }
   }
}