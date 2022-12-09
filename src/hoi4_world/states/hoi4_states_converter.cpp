#include "src/hoi4_world/states/hoi4_states_converter.h"

#include <algorithm>
#include <optional>
#include <queue>
#include <vector>

#include "external/commonItems/Log.h"
#include "external/fmt/include/fmt/format.h"
#include "src/maps/map_data.h"



namespace
{

std::map<std::string, int> MapVic3ProvincesToStates(const std::map<int, vic3::State>& states,
    const vic3::ProvinceDefinitions& vic3_province_definitions)
{
   std::map<std::string, int> vic3_province_to_state_id_map;
   std::for_each(states.begin(),
       states.end(),
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


bool AllVic3ProvincesAreInSameState(const std::vector<std::string>& vic3_provinces,
    int state_to_match,
    const std::map<std::string, int>& vic3_province_to_state_id_map)
{
   return std::all_of(vic3_provinces.begin(),
       vic3_provinces.end(),
       [vic3_province_to_state_id_map, state_to_match](const std::string& province_color) {
          const auto& state = vic3_province_to_state_id_map.find(province_color);
          return (state != vic3_province_to_state_id_map.end()) && (state->second == state_to_match);
       });
}


std::map<int, std::set<int>> PlaceHoi4ProvincesInStates(const std::map<std::string, int>& vic3_province_to_state_id_map,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings)
{
   std::map<int, std::set<int>> state_id_to_hoi4_provinces;

   for (const auto& [hoi4_province, vic3_provinces]: hoi4_to_vic3_province_mappings)
   {
      // todo: if we can tell they're invalid hoi4 provinces, skip them here
      // else, trim invalid vic3 provinces

      if (vic3_provinces.empty())
      {
         Log(LogLevel::Warning) << fmt::format("No mapping for hoi4 province {}.", hoi4_province);
         continue;
      }

      std::string first_vic3_province = vic3_provinces.front();
      const auto& first_state = vic3_province_to_state_id_map.find(first_vic3_province);
      if (first_state == vic3_province_to_state_id_map.end())
      {
         Log(LogLevel::Warning) << fmt::format("Vic3 province {} was not in a state.", first_vic3_province);
         continue;
      }
      if (AllVic3ProvincesAreInSameState(vic3_provinces, first_state->second, vic3_province_to_state_id_map))
      {
         if (auto [itr, success] = state_id_to_hoi4_provinces.emplace(first_state->second, std::set{hoi4_province});
             !success)
         {
            itr->second.insert(hoi4_province);
         }
      }
      else
      {
         // todo: handle other cases
      }
   }

   return state_id_to_hoi4_provinces;
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
               if (hoi4_province_definitions.IsLandProvince(std::to_string(neighbor)))
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


std::vector<hoi4::State> CreateStates(const std::map<int, vic3::State>& states,
    const std::map<int, std::set<int>>& state_id_to_hoi4_provinces,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const hoi4::StrategicRegions& strategic_regions,
    const mappers::CountryMapper& country_mapper)
{
   std::vector<hoi4::State> hoi4_states;
   for (const auto& [state_id, hoi4_provinces]: state_id_to_hoi4_provinces)
   {
      const auto state_itr = states.find(state_id);
      if (state_itr == states.end())
      {
         // I can't think how this would happen, so we'd better force people to the forums if it does.
         throw std::runtime_error("Something has gone very wrong.");
      }
      std::optional<std::string> state_owner;
      if (const std::optional<std::string>& source_owner = state_itr->second.GetOwnerTag(); source_owner.has_value())
      {
         state_owner = country_mapper.GetHoiTag(*source_owner);
      }

      const auto initial_connected_province_sets =
          GetConnectedProvinceSets(hoi4_provinces, map_data, hoi4_province_definitions);
      auto final_connected_province_sets =
          ConsolidateProvinceSets(initial_connected_province_sets, strategic_regions.GetProvinceToStrategicRegionMap());

      for (const auto& province_set: final_connected_province_sets)
      {
         hoi4_states.emplace_back(static_cast<int>(hoi4_states.size() + 1U), state_owner, province_set);
      }
   }

   return hoi4_states;
}

}  // namespace



std::vector<hoi4::State> hoi4::StatesConverter::ConvertStates(const std::map<int, vic3::State>& states,
    const vic3::ProvinceDefinitions& vic3_province_definitions,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const hoi4::StrategicRegions& strategic_regions,
    const mappers::CountryMapper& country_mapper)
{
   const std::map<std::string, int> vic3_province_to_state_id_map =
       MapVic3ProvincesToStates(states, vic3_province_definitions);
   const std::map<int, std::set<int>> state_id_to_hoi4_provinces =
       PlaceHoi4ProvincesInStates(vic3_province_to_state_id_map, hoi4_to_vic3_province_mappings);
   return CreateStates(states,
       state_id_to_hoi4_provinces,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);
}