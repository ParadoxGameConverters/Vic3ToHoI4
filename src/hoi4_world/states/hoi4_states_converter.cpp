#include "src/hoi4_world/states/hoi4_states_converter.h"

#include <algorithm>
#include <vector>

#include "external/commonItems/Log.h"
#include "external/fmt/include/fmt/format.h"



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


std::vector<hoi4::State> CreateStates(const std::map<int, std::set<int>>& state_id_to_hoi4_provinces)
{
   std::vector<hoi4::State> hoi4_states;
   for (const auto& [state_id, hoi4_provinces]: state_id_to_hoi4_provinces)
   {
      hoi4_states.emplace_back(state_id, hoi4_provinces);
   }

   return hoi4_states;
}

}  // namespace



std::vector<hoi4::State> hoi4::StatesConverter::ConvertStates(const std::map<int, vic3::State>& states,
    const vic3::ProvinceDefinitions& vic3_province_definitions,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings)
{
   const std::map<std::string, int> vic3_province_to_state_id_map =
       MapVic3ProvincesToStates(states, vic3_province_definitions);
   const std::map<int, std::set<int>> state_id_to_hoi4_provinces =
       PlaceHoi4ProvincesInStates(vic3_province_to_state_id_map, hoi4_to_vic3_province_mappings);
   return CreateStates(state_id_to_hoi4_provinces);
}