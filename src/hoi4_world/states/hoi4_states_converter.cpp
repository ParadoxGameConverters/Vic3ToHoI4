#include "src/hoi4_world/states/hoi4_states_converter.h"

#include <src/hoi4_world/world/hoi4_world.h>

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <optional>
#include <queue>
#include <ranges>
#include <vector>

#include "external/commonItems/Log.h"
#include "external/fmt/include/fmt/format.h"
#include "src/maps/map_data.h"
#include "src/out_hoi4/world/out_world.h"
#include "src/support/converter_utils.h"


namespace
{

constexpr int MAX_FACTORY_SLOTS = 12;


bool AllVic3ProvincesAreInSameState(const std::vector<std::string>& vic3_provinces,
    int state_to_match,
    const std::map<std::string, int>& vic3_province_to_state_id_map)
{
   return std::ranges::all_of(vic3_provinces,
       [&vic3_province_to_state_id_map, state_to_match](const std::string& province_color) {
          const auto& state = vic3_province_to_state_id_map.find(province_color);
          return (state != vic3_province_to_state_id_map.end()) && (state->second == state_to_match);
       });
}


std::optional<int> GetStateIfSharedByAllProvinces(const std::vector<std::string>& vic3_provinces,
    const std::map<std::string, int>& vic3_province_to_state_id_map)
{
   std::string first_vic3_province = vic3_provinces.front();
   const auto& first_state = vic3_province_to_state_id_map.find(first_vic3_province);
   if (first_state == vic3_province_to_state_id_map.end())
   {
      Log(LogLevel::Warning) << fmt::format("Vic3 province {} was not in a state.", first_vic3_province);
      return std::nullopt;
   }

   if (AllVic3ProvincesAreInSameState(vic3_provinces, first_state->second, vic3_province_to_state_id_map))
   {
      return first_state->second;
   }

   return std::nullopt;
}


std::optional<int> FindStateWithMostOfSignificantProvinceType(const std::string& significant_province_type,
    const std::vector<std::string>& vic3_provinces,
    const std::map<std::string, vic3::ProvinceType>& significant_vic3_provinces,
    const std::map<std::string, int>& vic3_province_to_state_id_map)
{
   std::map<int, int> states_to_counts;
   for (const auto& vic3_province: vic3_provinces)
   {
      const auto& significant_province_itr = significant_vic3_provinces.find(vic3_province);
      if (significant_province_itr == significant_vic3_provinces.end())
      {
         continue;
      }
      if (significant_province_itr->second != significant_province_type)
      {
         continue;
      }

      const auto& state = vic3_province_to_state_id_map.find(vic3_province);
      if (state == vic3_province_to_state_id_map.end())
      {
         Log(LogLevel::Warning) << fmt::format("Vic3 province {} was not in a state.", vic3_province);
         continue;
      }

      if (auto [itr, success] = states_to_counts.emplace(state->second, 1); !success)
      {
         itr->second++;
      }
   }

   if (!states_to_counts.empty())
   {
      return std::ranges::max_element(states_to_counts, [](const auto& a, const auto& b) {
         return a.second < b.second;
      })->first;
   }

   return std::nullopt;
}


std::optional<int> DetermineStateWithMostImportantProvinces(const std::vector<std::string>& vic3_provinces,
    const std::map<std::string, int>& vic3_province_to_state_id_map,
    const std::map<std::string, vic3::ProvinceType>& significant_vic3_provinces)
{
   if (const std::optional<int> possible_state = FindStateWithMostOfSignificantProvinceType("city",
           vic3_provinces,
           significant_vic3_provinces,
           vic3_province_to_state_id_map);
       possible_state.has_value())
   {
      return *possible_state;
   }

   return FindStateWithMostOfSignificantProvinceType("port",
       vic3_provinces,
       significant_vic3_provinces,
       vic3_province_to_state_id_map);
}


std::optional<int> DetermineStateWithMostProvinces(const std::vector<std::string>& vic3_provinces,
    const std::map<std::string, int>& vic3_province_to_state_id_map)
{
   std::map<int, int> state_counts;
   for (const auto& vic3_province: vic3_provinces)
   {
      const auto& state = vic3_province_to_state_id_map.find(vic3_province);
      if (state == vic3_province_to_state_id_map.end())
      {
         Log(LogLevel::Warning) << fmt::format("Vic3 province {} was not in a state.", vic3_province);
         continue;
      }
      auto [iterator, success] = state_counts.emplace(state->second, 1);
      if (!success)
      {
         iterator->second++;
      }
   }

   if (state_counts.empty())
   {
      return std::nullopt;
   }

   const auto max_state_count =
       std::ranges::max_element(state_counts, [](std::pair<int, int> a, std::pair<int, int> b) {
          return a.second < b.second;
       });
   return max_state_count->first;
}


std::map<int, std::set<int>> PlaceHoi4ProvincesInStates(
    const std::map<std::string, vic3::ProvinceType>& significant_vic3_provinces,
    const std::map<std::string, int>& vic3_province_to_state_id_map,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const maps::ProvinceDefinitions& hoi4_province_definitions)
{
   std::map<int, std::set<int>> state_id_to_hoi4_provinces;

   for (const auto& [hoi4_province, vic3_provinces]: hoi4_to_vic3_province_mappings)
   {
      if (!hoi4_province_definitions.IsLandProvince(std::to_string(hoi4_province)))
      {
         continue;
      }

      if (vic3_provinces.empty())
      {
         Log(LogLevel::Warning) << fmt::format("No mapping for hoi4 province {}.", hoi4_province);
         continue;
      }

      std::optional<int> state_number;
      state_number = GetStateIfSharedByAllProvinces(vic3_provinces, vic3_province_to_state_id_map);
      if (!state_number)
      {
         state_number = DetermineStateWithMostImportantProvinces(vic3_provinces,
             vic3_province_to_state_id_map,
             significant_vic3_provinces);
      }
      if (!state_number.has_value())
      {
         state_number = DetermineStateWithMostProvinces(vic3_provinces, vic3_province_to_state_id_map);
      }

      if (state_number.has_value())
      {
         if (auto [itr, success] = state_id_to_hoi4_provinces.emplace(*state_number, std::set{hoi4_province}); !success)
         {
            itr->second.insert(hoi4_province);
         }
      }
   }

   return state_id_to_hoi4_provinces;
}


std::vector<int> SortVic3StatesByIndustryDescending(const vic3::Buildings& vic3_buildings,
    const std::map<int, std::set<int>>& vic3_state_id_to_hoi4_provinces)
{
   std::vector<int> vic3_state_ids;
   std::ranges::copy(vic3_state_id_to_hoi4_provinces | std::views::keys, std::back_inserter(vic3_state_ids));
   const auto ByVic3Industry = [vic3_buildings](const int& lhs, const int& rhs) {
      return vic3_buildings.GetTotalGoodSalesValueInState(lhs) > vic3_buildings.GetTotalGoodSalesValueInState(rhs);
   };
   std::ranges::sort(vic3_state_ids, ByVic3Industry);
   return vic3_state_ids;
}


std::set<int> GetWastelandProvinces(const std::map<int, hoi4::DefaultState>& default_states)
{
   std::set<int> wasteland_provinces;
   for (const hoi4::DefaultState& default_state: default_states | std::views::values)
   {
      if (!default_state.IsImpassable())
      {
         continue;
      }

      wasteland_provinces.insert(default_state.GetProvinces().begin(), default_state.GetProvinces().end());
   }

   return wasteland_provinces;
}


std::map<std::string, std::string> GetAllSignificantProvinces(
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions)
{
   std::map<std::string, std::string> significant_provinces;

   for (const vic3::StateRegion& state_region: vic3_state_regions | std::views::values)
   {
      const std::map<vic3::ProvinceId, vic3::ProvinceType>& local_significant_provinces =
          state_region.GetSignificantProvinces();
      significant_provinces.insert(local_significant_provinces.begin(), local_significant_provinces.end());
   }

   return significant_provinces;
}


std::map<std::string, std::string> MapVic3ProvincesToStateNames(
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions)
{
   std::map<std::string, std::string> vic3_provinces_to_state_names;
   for (const auto& [region_name, region]: vic3_state_regions)
   {
      for (const vic3::ProvinceId& province: region.GetProvinces())
      {
         vic3_provinces_to_state_names.emplace(province, region_name);
      }
   }

   return vic3_provinces_to_state_names;
}


std::vector<std::set<int>> GetConnectedProvinceSets(std::set<int> province_numbers,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions)
{
   std::vector<std::set<int>> connected_province_sets;
   while (!province_numbers.empty())
   {
      std::set<int> connected_province_set;

      std::queue<int> open_provinces;
      open_provinces.push(*province_numbers.begin());
      std::set<int> closed_provinces{*province_numbers.begin()};
      while (!open_provinces.empty() && !province_numbers.empty())
      {
         auto current_province = open_provinces.front();
         open_provinces.pop();
         if (province_numbers.contains(current_province))
         {
            connected_province_set.insert(current_province);
            province_numbers.erase(current_province);
         }

         for (const auto& neighbor_string: map_data.GetNeighbors(std::to_string(current_province)))
         {
            int neighbor = 0;
            try
            {
               neighbor = std::stoi(neighbor_string);
            }
            catch (...)
            {
               continue;
            }
            if (!closed_provinces.contains(neighbor))
            {
               if (hoi4_province_definitions.IsLandProvince(std::to_string(neighbor)) &&
                   province_numbers.contains(neighbor))
               {
                  open_provinces.push(neighbor);
                  closed_provinces.insert(neighbor);
               }
            }
         }
      }

      connected_province_sets.push_back(connected_province_set);
   }

   return connected_province_sets;
}


std::pair<std::vector<std::set<int>>, std::vector<std::set<int>>> SortProvinceSetsByWastelandStatus(
    const std::vector<std::set<int>>& connected_province_sets,
    const std::set<int>& wasteland_provinces)
{
   std::vector<std::set<int>> non_wasteland_province_sets;
   std::vector<std::set<int>> wasteland_province_sets;
   for (const auto& province_set: connected_province_sets)
   {
      std::set<int> non_wasteland_province_set;
      std::set<int> wasteland_province_set;
      for (const auto province: province_set)
      {
         if (wasteland_provinces.contains(province))
         {
            wasteland_province_set.insert(province);
         }
         else
         {
            non_wasteland_province_set.insert(province);
         }
      }

      if (!non_wasteland_province_set.empty())
      {
         non_wasteland_province_sets.push_back(non_wasteland_province_set);
      }
      if (!wasteland_province_set.empty())
      {
         wasteland_province_sets.push_back(wasteland_province_set);
      }
   }

   return {non_wasteland_province_sets, wasteland_province_sets};
}


std::vector<std::set<int>> ConsolidateProvinceSets(std::vector<std::set<int>> connected_province_sets,
    const std::map<int, int>& province_to_strategic_region_map)
{
   std::vector<std::set<int>> new_connected_province_sets;
   while (!connected_province_sets.empty())
   {
      auto base_set = connected_province_sets.front();
      connected_province_sets.erase(connected_province_sets.begin());

      std::optional<int> strategic_region;
      if (const auto& mapping = province_to_strategic_region_map.find(*base_set.begin());
          mapping != province_to_strategic_region_map.end())
      {
         strategic_region = mapping->second;
      }

      for (auto current_set = connected_province_sets.begin(); current_set != connected_province_sets.end();)
      {
         if (const auto& mapping = province_to_strategic_region_map.find(*current_set->begin());
             mapping != province_to_strategic_region_map.end() && strategic_region &&
             *strategic_region == mapping->second)
         {
            base_set.insert(current_set->begin(), current_set->end());
            current_set = connected_province_sets.erase(current_set);
            continue;
         }
         ++current_set;
      }

      new_connected_province_sets.push_back(base_set);
   }

   return new_connected_province_sets;
}


void RecordStateNamesMapping(const std::set<int>& province_set,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const std::map<std::string, std::string>& vic3_provinces_to_state_names,
    int hoi4_state_number,
    std::map<std::string, std::string>& hoi4_state_names_to_vic3_state_names)
{
   const auto& first_province = *province_set.begin();
   const auto& mapping = hoi4_to_vic3_province_mappings.find(first_province);
   if (mapping == hoi4_to_vic3_province_mappings.end())
   {
      return;
   }

   if (mapping->second.empty())
   {
      return;
   }

   const auto& state_name_mapping = vic3_provinces_to_state_names.find(mapping->second[0]);
   if (state_name_mapping == vic3_provinces_to_state_names.end())
   {
      return;
   }

   hoi4_state_names_to_vic3_state_names.emplace(fmt::format("STATE_{}", hoi4_state_number), state_name_mapping->second);
}


bool IsStateCoastal(const std::set<int>& state_provinces, const hoi4::CoastalProvinces& coastal_provinces)
{
   return std::ranges::any_of(state_provinces, [coastal_provinces](int province) {
      return coastal_provinces.IsProvinceCoastal(province);
   });
}


struct FactoriesStruct
{
   float military;
   float civilian;
   float docks;
};
std::tuple<int, int, int> ConvertIndustry(const float& total_factories,
    const std::set<int>& province_set,
    int num_hoi4_provinces,
    const std::string& state_owner,
    const hoi4::CoastalProvinces& coastal_provinces,
    std::unordered_map<std::string, FactoriesStruct>& accumulator)
{
   const float factories = total_factories * province_set.size() / num_hoi4_provinces;

   int civilian_factories = 0;
   int military_factories = 0;
   int dockyards = 0;

   if (!accumulator.contains(state_owner))
   {
      accumulator[state_owner] = {0, 0, 0};
   }
   auto& country_factories = accumulator[state_owner];
   if (IsStateCoastal(province_set, coastal_provinces))
   {
      //		20% chance of dockyard
      //		57% chance of civilian factory
      //		23% chance of military factory
      country_factories.military += 0.23F * factories;
      country_factories.civilian += 0.57F * factories;
      country_factories.docks += 0.20F * factories;
   }
   else
   {
      //		 0% chance of dockyard
      //		71% chance of civilian factory
      //		29% chance of military factory
      country_factories.military += 0.29F * factories;
      country_factories.civilian += 0.71F * factories;
   }

   const float applied_factories = std::max(0.0F, country_factories.military) +
                                   std::max(0.0F, country_factories.civilian) +
                                   (IsStateCoastal(province_set, {}) ? std::max(0.0F, country_factories.docks) : 0.0F);
   const int factories_floor = static_cast<int>(std::round(std::max(factories, applied_factories)));

   for (int i = 0; i < std::min(factories_floor, MAX_FACTORY_SLOTS); i++)
   {
      if (IsStateCoastal(province_set, coastal_provinces) && (country_factories.docks > 0.0F))
      {
         dockyards++;
         country_factories.docks -= 1.0F;
      }
      else if (country_factories.military >= 0.0F)
      {
         military_factories++;
         country_factories.military -= 1.0F;
      }
      else
      {
         civilian_factories++;
         country_factories.civilian -= 1.0F;
      }
   }

   return {civilian_factories, military_factories, dockyards};
}


std::tuple<std::optional<int>, std::optional<int>> DetermineNavalBase(const std::set<int>& hoi4_provinces,
    const std::map<std::string, std::string>& significant_vic3_provinces,
    const hoi4::CoastalProvinces& coastal_provinces,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings)
{
   for (const auto& hoi4_province: hoi4_provinces)
   {
      if (!coastal_provinces.IsProvinceCoastal(hoi4_province))
      {
         continue;
      }

      const auto province_mapping = hoi4_to_vic3_province_mappings.find(hoi4_province);
      if (province_mapping == hoi4_to_vic3_province_mappings.end())
      {
         continue;
      }

      if (std::ranges::none_of(province_mapping->second,
              [significant_vic3_provinces](const std::string& vic3_province) {
                 if (const auto& significant_province = significant_vic3_provinces.find(vic3_province);
                     significant_province != significant_vic3_provinces.end())
                 {
                    return significant_province->second == "port";
                 }

                 return false;
              }))
      {
         continue;
      }

      return {hoi4_province, 1};
   }

   return {std::nullopt, std::nullopt};
}


int DetermineAirbaseLevel(int total_factories, int total_infrastructure)
{
   constexpr int kNumFactoriesPerAirbase = 4;
   constexpr int kAirbasesForInfrastructureLevel = 3;

   int airbase_level = total_factories / kNumFactoriesPerAirbase;
   if (total_infrastructure >= kAirbasesForInfrastructureLevel)
   {
      ++airbase_level;
   }

   return airbase_level;
}


hoi4::Resources AssignResources(const std::set<int>& provinces, const hoi4::ResourcesMap& resources_map)
{
   hoi4::Resources resources;

   for (const int province: provinces)
   {
      const auto resources_itr = resources_map.find(province);
      if (resources_itr == resources_map.end())
      {
         continue;
      }

      for (const auto& resource: resources_itr->second)
      {
         auto [iterator, success] = resources.insert(resource);
         if (!success)
         {
            iterator->second += resource.second;
         }
      }
   }

   return resources;
}


std::optional<int> GetBestHoi4Province(const mappers::Hoi4ToVic3ProvinceMapping::const_iterator province_mapping,
    const mappers::Vic3ToHoi4ProvinceMapping& vic3_to_hoi4_province_mappings,
    const std::set<int>& hoi4_provinces)
{
   const auto reverse_province_mapping = vic3_to_hoi4_province_mappings.find(province_mapping->second.front());
   if (reverse_province_mapping == vic3_to_hoi4_province_mappings.end())
   {
      // again, shouldn't be possible but just in case
      return std::nullopt;
   }

   for (const auto& province: reverse_province_mapping->second)
   {
      if (hoi4_provinces.contains(province))
      {
         return province;
      }
   }

   return std::nullopt;
}


std::optional<int> GetVictoryPointValue(const std::string& special_province_type)
{
   static const std::map<std::string, int> vp_values{
       {"city", 5},
       {"port", 4},
       {"farm", 3},
       {"mine", 2},
       {"wood", 1},
   };
   if (const auto& mapping = vp_values.find(special_province_type); mapping != vp_values.end())
   {
      return mapping->second;
   }

   return std::nullopt;
}


std::map<int, int> SetPossibleVictoryPoints(const std::set<int>& hoi4_provinces,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const mappers::Vic3ToHoi4ProvinceMapping& vic3_to_hoi4_province_mappings,
    const std::map<std::string, std::string>& significant_provinces)
{
   std::map<int, int> possible_victory_points;

   std::set<std::string> applied_types;
   for (const auto hoi4_province: hoi4_provinces)
   {
      const auto province_mapping = hoi4_to_vic3_province_mappings.find(hoi4_province);
      if (province_mapping == hoi4_to_vic3_province_mappings.end() || province_mapping->second.empty())
      {
         // shouldn't be possible, since all hoi4 provinces come from the mappings, but just in case
         continue;
      }

      std::optional<int> best_hoi4_province =
          GetBestHoi4Province(province_mapping, vic3_to_hoi4_province_mappings, hoi4_provinces);
      if (!best_hoi4_province)
      {
         // yet another case that shouldn't be possible
         continue;
      }

      for (const auto& vic3_province: province_mapping->second)
      {
         const auto& significant_province = significant_provinces.find(vic3_province);
         if (significant_province == significant_provinces.end())
         {
            continue;
         }
         const std::string& special_province_type = significant_province->second;

         if (applied_types.contains(special_province_type))
         {
            continue;
         }

         const std::optional<int> possible_vp_value = GetVictoryPointValue(special_province_type);
         if (!possible_vp_value)
         {
            continue;
         }
         if (auto [itr, success] = possible_victory_points.emplace(*best_hoi4_province, *possible_vp_value); !success)
         {
            if (itr->second < *possible_vp_value)
            {
               itr->second = *possible_vp_value;
            }
         }
         applied_types.insert(special_province_type);
      }
   }

   return possible_victory_points;
}


std::map<int, int> CreateVictoryPoints(const std::set<int>& hoi4_provinces,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const mappers::Vic3ToHoi4ProvinceMapping& vic3_to_hoi4_province_mappings,
    const std::map<std::string, std::string>& significant_provinces,
    int factory_vps,
    bool debug)
{
   std::map<int, int> possible_victory_points = SetPossibleVictoryPoints(hoi4_provinces,
       hoi4_to_vic3_province_mappings,
       vic3_to_hoi4_province_mappings,
       significant_provinces);
   if (debug)
   {
      return possible_victory_points;
   }

   std::vector<std::pair<int, int>> sorted_possible_victory_points;
   sorted_possible_victory_points.reserve(possible_victory_points.size());
   std::ranges::copy(possible_victory_points,
       std::inserter(sorted_possible_victory_points, sorted_possible_victory_points.end()));
   std::ranges::sort(sorted_possible_victory_points, [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
      return a.second > b.second;
   });

   if (!sorted_possible_victory_points.empty())
   {
      return {{sorted_possible_victory_points[0].first, std::max(factory_vps, 1)}};
   }

   return {};
}


void LogIndustryStats(const std::vector<hoi4::State>& hoi4_states,
    const std::map<int, hoi4::DefaultState>& default_states,
    const hoi4::StateCategories& state_categories)
{
   int civilian_factories = 0;
   int military_factories = 0;
   int dockyards = 0;
   std::map<std::string, double> resources;
   std::map<std::string, int> state_category_counts;
   std::map<int, int> state_slot_differences;
   std::map<int, int> state_factory_numbers;
   for (const hoi4::State& hoi4_state: hoi4_states)
   {
      civilian_factories += hoi4_state.GetCivilianFactories();
      military_factories += hoi4_state.GetMilitaryFactories();
      dockyards += hoi4_state.GetDockyards();
      for (const auto& hoi4_state_resource: hoi4_state.GetResources())
      {
         resources[hoi4_state_resource.first] += hoi4_state_resource.second;
      }
      if (auto [category_itr, category_success] = state_category_counts.emplace(hoi4_state.GetCategory(), 1);
          !category_success)
      {
         category_itr->second++;
      }

      const int total_factories =
          hoi4_state.GetCivilianFactories() + hoi4_state.GetMilitaryFactories() + hoi4_state.GetDockyards();
      if (auto [factories_itr, factories_success] = state_factory_numbers.emplace(total_factories, 1);
          !factories_success)
      {
         factories_itr->second++;
      }

      const int excess_slots = state_categories.GetNumSlots(hoi4_state.GetCategory()) - total_factories;
      if (auto [diff_itr, diff_success] = state_slot_differences.emplace(excess_slots, 1); !diff_success)
      {
         diff_itr->second++;
      }
   }

   int default_civilian_factories = 0;
   int default_military_factories = 0;
   int default_dockyards = 0;
   std::map<std::string, double> default_resources;
   for (const hoi4::DefaultState& hoi4_state: default_states | std::views::values)
   {
      default_civilian_factories += hoi4_state.GetCivilianFactories();
      default_military_factories += hoi4_state.GetMilitaryFactories();
      default_dockyards += hoi4_state.GetDockyards();
      for (const auto& default_hoi4_state_resource: hoi4_state.GetResources())
      {
         default_resources[default_hoi4_state_resource.first] += default_hoi4_state_resource.second;
      }
   }

   OutputStats("Total factories",
       civilian_factories + military_factories + dockyards,
       default_civilian_factories + default_military_factories + default_dockyards);
   OutputStats("Civilian factories", civilian_factories, default_civilian_factories);
   OutputStats("Military factories", military_factories, default_military_factories);
   OutputStats("Dockyards", dockyards, default_dockyards);

   for (const auto& [factories, num_states]: state_factory_numbers)
   {
      Log(LogLevel::Info) << fmt::format("\t\t\t{} states had {} factories", num_states, factories);
   }
   Log(LogLevel::Info) << "\t\tState categories:";
   for (const auto& [category_name, num_states]: state_category_counts)
   {
      Log(LogLevel::Info) << fmt::format("\t\t\t{} states are {}", num_states, category_name);
   }
   for (const auto& [difference, num_states]: state_slot_differences)
   {
      Log(LogLevel::Info) << fmt::format("\t\t\t{} states had {} excess slots", num_states, difference);
   }
   for (const auto& r: resources)
   {
      Log(LogLevel::Info) << fmt::format("\t\t\tConverter Resource {}:{}", r.first, r.second);
   }
   for (const auto& dr: default_resources)
   {
      Log(LogLevel::Info) << fmt::format("\t\t\tDefault Resource {}:{}", dr.first, dr.second);
   }
}


void LogManpowerStats(const std::vector<hoi4::State>& hoi4_states,
    const std::map<int, hoi4::DefaultState>& default_states)
{
   int64_t manpower = std::accumulate(hoi4_states.begin(),
       hoi4_states.end(),
       static_cast<int64_t>(0),
       [](int64_t total, const hoi4::State& state) {
          return static_cast<uint64_t>(total + state.GetManpower());
       });

   int64_t default_manpower = std::accumulate(default_states.begin(),
       default_states.end(),
       static_cast<int64_t>(0),
       [](int64_t total, const std::pair<int, hoi4::DefaultState>& state) {
          return total + state.second.GetManpower();
       });

   OutputStats("Manpower", manpower, default_manpower);
}

void LogInfrastructure(mappers::InfrastructureMapper infrastructure_mapper)
{
   OutputStats("Infrastructure",
       infrastructure_mapper.GetConvertedInfrastructure(),
       infrastructure_mapper.GetTargetInfrastructure());
   Log(LogLevel::Info) << fmt::format("\tfudge factor is {}", infrastructure_mapper.GetFudgeFactor());
}


hoi4::States CreateStates(const vic3::World& source_world,
    const mappers::WorldMapper& world_mapper,
    const hoi4::WorldFramework& world_framework,
    const std::map<int, std::set<int>>& vic3_state_id_to_hoi4_provinces,
    const std::vector<int>& vic3_state_ids_by_vic3_industry,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const hoi4::CoastalProvinces& coastal_provinces,
    bool debug)
{
   std::vector<hoi4::State> hoi4_states;
   std::map<int, int> province_to_state_id_map;
   std::map<int, int> vic3_state_ids_to_hoi4_state_ids;
   std::unordered_map<std::string, FactoriesStruct> accumulator;

   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings =
       world_mapper.province_mapper.GetHoi4ToVic3ProvinceMappings();
   mappers::Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_mappings =
       world_mapper.province_mapper.GetVic3ToHoi4ProvinceMappings();
   const std::set<int> wasteland_provinces = GetWastelandProvinces(world_framework.default_states);
   std::map<std::string, std::string> significant_provinces =
       GetAllSignificantProvinces(source_world.GetStateRegions());
   std::map<std::string, std::string> vic3_provinces_to_state_names =
       MapVic3ProvincesToStateNames(source_world.GetStateRegions());
   std::map<std::string, std::string> hoi4_state_names_to_vic3_state_names;
   mappers::InfrastructureMapper infrastructure_mapper(source_world.GetStates());
   for (const auto& vic3_state_id: vic3_state_ids_by_vic3_industry)
   {
      const auto& hoi4_provinces = vic3_state_id_to_hoi4_provinces.at(vic3_state_id);
      const auto vic3_state_itr = source_world.GetStates().find(vic3_state_id);
      if (vic3_state_itr == source_world.GetStates().end())
      {
         // I can't think how this would happen, so we'd better force people to the forums if it does.
         throw std::runtime_error("Something has gone very wrong.");
      }
      std::optional<std::string> state_owner;
      if (const std::optional<int>& vic3_state_owner = vic3_state_itr->second.GetOwnerNumber();
          vic3_state_owner.has_value())
      {
         state_owner = world_mapper.country_mapper.GetHoiTag(*vic3_state_owner);
         if (!state_owner.has_value())
         {
            Log(LogLevel::Warning) << fmt::format("Could not get tag for owner of Vic3 state {}.", vic3_state_id);
         }
      }

      const auto initial_connected_province_sets =
          GetConnectedProvinceSets(hoi4_provinces, map_data, hoi4_province_definitions);
      const auto [non_wasteland_province_sets, wasteland_province_sets] =
          SortProvinceSetsByWastelandStatus(initial_connected_province_sets, wasteland_provinces);
      const auto final_connected_province_sets = ConsolidateProvinceSets(non_wasteland_province_sets,
          world_framework.strategic_regions.GetProvinceToStrategicRegionMap());
      const auto final_wasteland_connected_province_sets = ConsolidateProvinceSets(wasteland_province_sets,
          world_framework.strategic_regions.GetProvinceToStrategicRegionMap());
      if (final_connected_province_sets.size() > 1U)
      {
         Log(LogLevel::Info) << fmt::format("\tState {} was split into {} due to disconnected provinces.",
             hoi4_states.size() + 1U,
             final_connected_province_sets.size());
      }

      int total_wasteland_provinces = std::accumulate(final_wasteland_connected_province_sets.begin(),
          final_wasteland_connected_province_sets.end(),
          0,
          [](const int total, const std::set<int>& province_set) {
             return total + static_cast<int>(province_set.size());
          });
      int total_non_wasteland_provinces = static_cast<int>(hoi4_provinces.size()) - total_wasteland_provinces;
      const int64_t total_manpower = vic3_state_itr->second.GetPopulation();
      const float total_factories =
          static_cast<float>(source_world.GetBuildings().GetTotalGoodSalesValueInState(vic3_state_id)) / 175'000.0F;
      for (const auto& province_set: final_connected_province_sets)
      {
         RecordStateNamesMapping(province_set,
             hoi4_to_vic3_province_mappings,
             vic3_provinces_to_state_names,
             static_cast<int>(hoi4_states.size() + 1U),
             hoi4_state_names_to_vic3_state_names);

         int civilian_factories = 0;
         int military_factories = 0;
         int dockyards = 0;
         if (state_owner)
         {
            const auto all_factories = ConvertIndustry(total_factories,
                province_set,
                total_non_wasteland_provinces,
                *state_owner,
                coastal_provinces,
                accumulator);
            civilian_factories = std::get<0>(all_factories);
            military_factories = std::get<1>(all_factories);
            dockyards = std::get<2>(all_factories);
         }

         const auto [naval_base_location, naval_base_level] =
             DetermineNavalBase(province_set, significant_provinces, coastal_provinces, hoi4_to_vic3_province_mappings);

         int infrastructure = infrastructure_mapper.Map(vic3_state_itr->second.GetInfrastructure());

         const int air_base_level =
             DetermineAirbaseLevel(civilian_factories + military_factories + dockyards, infrastructure);

         const hoi4::Resources resources = AssignResources(province_set, world_framework.resources_map);

         const std::string category = world_framework.state_categories.GetBestCategory(
             std::min(civilian_factories + military_factories + dockyards + 2, static_cast<int>(MAX_FACTORY_SLOTS)));


         const int manpower = static_cast<int>(
             total_manpower * static_cast<int>(province_set.size()) / static_cast<int>(hoi4_provinces.size()));

         const std::map<int, int> victory_points = CreateVictoryPoints(province_set,
             hoi4_to_vic3_province_mappings,
             vic3_to_hoi4_province_mappings,
             significant_provinces,
             static_cast<int>(
                 std::round((static_cast<float>(civilian_factories + military_factories + dockyards)) / 2.0F)),
             debug);

         std::set<std::string> cores;
         if (vic3_state_itr->second.IsIncorporated())
         {
            cores.insert(*state_owner);
         }

         for (const int province: province_set)
         {
            province_to_state_id_map.emplace(province, static_cast<int>(hoi4_states.size() + 1U));
         }
         vic3_state_ids_to_hoi4_state_ids.emplace(vic3_state_id, static_cast<int>(hoi4_states.size() + 1U));
         hoi4_states.emplace_back(static_cast<int>(hoi4_states.size() + 1U),
             hoi4::StateOptions{.owner = state_owner,
                 .provinces = province_set,
                 .manpower = manpower,
                 .resources = resources,
                 .category = category,
                 .victory_points = victory_points,
                 .civilian_factories = civilian_factories,
                 .military_factories = military_factories,
                 .dockyards = dockyards,
                 .naval_base_location = naval_base_location,
                 .naval_base_level = naval_base_level,
                 .air_base_level = air_base_level,
                 .cores = cores,
                 .vic3_infrastructure = vic3_state_itr->second.GetInfrastructure(),
                 .infrastructure = infrastructure});
      }
      for (const auto& province_set: final_wasteland_connected_province_sets)
      {
         RecordStateNamesMapping(province_set,
             hoi4_to_vic3_province_mappings,
             vic3_provinces_to_state_names,
             static_cast<int>(hoi4_states.size() + 1U),
             hoi4_state_names_to_vic3_state_names);

         const int manpower = static_cast<int>(
             total_manpower * static_cast<int>(province_set.size()) / static_cast<int>(hoi4_provinces.size()));

         for (const int province: province_set)
         {
            province_to_state_id_map.emplace(province, static_cast<int>(hoi4_states.size() + 1U));
         }
         vic3_state_ids_to_hoi4_state_ids.emplace(vic3_state_id, static_cast<int>(hoi4_states.size() + 1U));
         hoi4_states.emplace_back(static_cast<int>(hoi4_states.size() + 1U),
             hoi4::StateOptions{.owner = state_owner,
                 .provinces = province_set,
                 .manpower = manpower,
                 .category = "wasteland"});
      }
   }

   LogIndustryStats(hoi4_states, world_framework.default_states, world_framework.state_categories);
   LogManpowerStats(hoi4_states, world_framework.default_states);
   LogInfrastructure(infrastructure_mapper);

   return {hoi4_states,
       province_to_state_id_map,
       vic3_state_ids_to_hoi4_state_ids,
       hoi4_state_names_to_vic3_state_names};
}

}  // namespace

namespace hoi4
{

std::map<std::string, int> MapVic3ProvincesToStates(const std::map<int, vic3::State>& states,
    const vic3::ProvinceDefinitions& vic3_province_definitions)
{
   std::map<std::string, int> vic3_province_to_state_id_map;
   std::ranges::for_each(states,
       [&vic3_province_to_state_id_map, vic3_province_definitions](const std::pair<int, vic3::State>& state) {
          for (const auto& province: state.second.GetProvinces())
          {
             const auto possible_province_color = vic3_province_definitions.GetProvinceDefinition(province);
             if (possible_province_color.has_value())
             {
                vic3_province_to_state_id_map.emplace(possible_province_color.value(), state.first);
             }
             else
             {
                Log(LogLevel::Warning) << fmt::format("No definition for province {}.", province);
             }
          }
       });

   return vic3_province_to_state_id_map;
}

hoi4::States ConvertStates(const vic3::World& source_world,
    const mappers::WorldMapper& world_mapper,
    const hoi4::WorldFramework& world_framework,
    const std::map<std::string, vic3::ProvinceType>& significant_vic3_provinces,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const CoastalProvinces& coastal_provinces,
    bool debug)
{
   const std::map<std::string, int> vic3_province_to_state_id_map =
       MapVic3ProvincesToStates(source_world.GetStates(), source_world.GetProvinceDefinitions());
   const std::map<int, std::set<int>> vic3_state_id_to_hoi4_provinces =
       PlaceHoi4ProvincesInStates(significant_vic3_provinces,
           vic3_province_to_state_id_map,
           world_mapper.province_mapper.GetHoi4ToVic3ProvinceMappings(),
           hoi4_province_definitions);
   const std::vector<int> vic3_state_ids_by_vic3_industry =
       SortVic3StatesByIndustryDescending(source_world.GetBuildings(), vic3_state_id_to_hoi4_provinces);
   return CreateStates(source_world,
       world_mapper,
       world_framework,
       vic3_state_id_to_hoi4_provinces,
       vic3_state_ids_by_vic3_industry,
       map_data,
       hoi4_province_definitions,
       coastal_provinces,
       debug);
}
}  // namespace hoi4