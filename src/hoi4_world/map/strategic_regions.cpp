#include "src/hoi4_world/map/strategic_regions.h"

#include <external/commonItems/Log.h>

#include <algorithm>
#include <ranges>



namespace
{

std::map<int, int> DetermineUsedRegions(const hoi4::State& state,
    const std::map<int, int>& province_to_strategic_region_map)
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

      if (auto [iterator, success] = used_regions.emplace(mapping->second, 1); !success)
      {
         ++iterator->second;
      }
   }

   return used_regions;
}


std::optional<int> DetermineMostUsedRegion(const std::map<int, int>& used_regions)
{
   if (!used_regions.empty())
   {
      return std::ranges::max_element(used_regions, [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
         return a.second < b.second;
      })->first;
   }

   return std::nullopt;
}


void AddProvinceToRegion(int region_number, int province_id, std::map<int, hoi4::StrategicRegion>& strategic_regions)
{
   auto region = strategic_regions.find(region_number);
   if (region == strategic_regions.end())
   {
      Log(LogLevel::Warning) << "Strategic region " << region_number << " was not in the list of regions.";
      return;
   }

   region->second.AddNewProvince(province_id);
}


std::set<int> DetermineAssignedProvinces(const std::map<int, hoi4::StrategicRegion>& strategic_regions)
{
   std::set<int> assigned_provinces;
   for (const hoi4::StrategicRegion& strategic_region: strategic_regions | std::ranges::views::values)
   {
      for (int province: strategic_region.GetNewProvinces())
      {
         assigned_provinces.insert(province);
      }
   }

   return assigned_provinces;
}


void AddSurroundedProvincesToRegions(const std::map<int, int>& original_province_to_strategic_region_map,
    const std::set<int>& assigned_provinces,
    const maps::MapData& hoi4_map_data,
    const std::map<int, int>& new_province_to_strategic_region_map,
    std::map<int, hoi4::StrategicRegion>& strategic_regions)
{
   for (const auto& province: original_province_to_strategic_region_map | std::views::keys)
   {
      // this only applies to unassigned provinces
      if (assigned_provinces.contains(province))
      {
         continue;
      }

      bool has_unassigned_neighbor = false;
      std::map<int, int> used_regions;  // region ID -> number of provinces in that region
      for (const std::string& neighbor_string: hoi4_map_data.GetNeighbors(std::to_string(province)))
      {
         const int neighbor = std::stoi(neighbor_string);

         // unassigned neighbors indicate oceans, which means this case doesn't apply
         if (!assigned_provinces.contains(neighbor))
         {
            has_unassigned_neighbor = true;
            break;
         }

         const auto itr = new_province_to_strategic_region_map.find(neighbor);
         used_regions[itr->second]++;
      }

      if (has_unassigned_neighbor)
      {
         continue;
      }

      if (const auto best_region = DetermineMostUsedRegion(used_regions); best_region)
      {
         AddProvinceToRegion(*best_region, province, strategic_regions);
      }
   }
}


void AddLeftoverProvincesToRegions(const std::set<int>& assigned_provinces,
    const std::map<int, int>& province_to_strategic_region_map,
    std::map<int, hoi4::StrategicRegion>& strategic_regions)
{
   for (const auto& [province, strategic_region]: province_to_strategic_region_map)
   {
      if (assigned_provinces.contains(province))
      {
         continue;
      }
      AddProvinceToRegion(strategic_region, province, strategic_regions);
   }
}


void AddProvincesToRegion(int region_number,
    const hoi4::State& state,
    std::map<int, hoi4::StrategicRegion>& strategic_regions)
{
   for (const auto& province: state.GetProvinces())
   {
      AddProvinceToRegion(region_number, province, strategic_regions);
   }
}

}  // namespace



void hoi4::StrategicRegions::UpdateToMatchNewStates(const std::vector<State>& states,
    const maps::MapData& hoi4_map_data)
{
   Log(LogLevel::Info) << "\t\tUpdating strategic regions";
   new_province_to_strategic_region_map_.clear();

   for (const auto& state: states)
   {
      const auto used_regions = DetermineUsedRegions(state, original_province_to_strategic_region_map_);
      if (const auto best_region = DetermineMostUsedRegion(used_regions); best_region)
      {
         AddProvincesToRegion(*best_region, state, strategic_regions_);
      }
   }



   std::map<int, int> temp_province_to_strategic_region_map;
   for (const auto& [region_number, region]: strategic_regions_)
   {
      for (auto province: region.GetNewProvinces())
      {
         temp_province_to_strategic_region_map.emplace(province, region_number);
      }
   }

   AddSurroundedProvincesToRegions(original_province_to_strategic_region_map_,
       DetermineAssignedProvinces(strategic_regions_),
       hoi4_map_data,
       temp_province_to_strategic_region_map,
       strategic_regions_);
   AddLeftoverProvincesToRegions(DetermineAssignedProvinces(strategic_regions_),
       original_province_to_strategic_region_map_,
       strategic_regions_);

   for (const auto& [region_number, region]: strategic_regions_)
   {
      for (auto province: region.GetNewProvinces())
      {
         new_province_to_strategic_region_map_.emplace(province, region_number);
      }
   }
}