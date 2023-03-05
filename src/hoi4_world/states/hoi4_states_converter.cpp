#include "src/hoi4_world/states/hoi4_states_converter.h"

#include <algorithm>
#include <optional>
#include <queue>
#include <ranges>
#include <vector>

#include "external/commonItems/Log.h"
#include "external/fmt/include/fmt/format.h"
#include "src/maps/map_data.h"



namespace
{

constexpr float MAX_FACTORY_SLOTS = 12.0F;


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

   const std::map<int, int>::iterator max_state_count =
       std::max_element(state_counts.begin(), state_counts.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
          return a.second < b.second;
       });
   return max_state_count->first;
}


std::map<int, std::set<int>> PlaceHoi4ProvincesInStates(const std::map<std::string, int>& vic3_province_to_state_id_map,
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
      // todo: prioritize states that control special vic3 provinces
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


bool IsStateCoastal(const std::set<int>& state_provinces, const std::map<int, std::vector<int>>& coastal_provinces)
{
   return false;
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
   if (IsStateCoastal(province_set, {}))
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

   for (int i = 0; static_cast<float>(i) < std::clamp(factories, applied_factories, MAX_FACTORY_SLOTS); i++)
   {
      if (IsStateCoastal(province_set, {}) && (country_factories.docks > 0.0F))
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


void LogIndustryStats(const std::vector<hoi4::State>& hoi4_states,
    const std::map<int, hoi4::DefaultState>& default_states)
{
   int civilian_factories = 0;
   int military_factories = 0;
   int dockyards = 0;
   for (const auto& hoi4_state: hoi4_states)
   {
      civilian_factories += hoi4_state.GetCivilianFactories();
      military_factories += hoi4_state.GetMilitaryFactories();
      dockyards += hoi4_state.GetDockyards();
   }

   int default_civilian_factories = 0;
   int default_military_factories = 0;
   int default_dockyards = 0;
   for (const auto& hoi4_state: default_states | std::views::values)
   {
      default_civilian_factories += hoi4_state.GetCivilianFactories();
      default_military_factories += hoi4_state.GetMilitaryFactories();
      default_dockyards += hoi4_state.GetDockyards();
   }

   Log(LogLevel::Info) << fmt::format("\t\tTotal factories: {} (vanilla hoi4 had {})",
       civilian_factories + military_factories + dockyards,
       default_civilian_factories + default_military_factories + default_dockyards);
   Log(LogLevel::Info) << fmt::format("\t\t\tCivilian factories: {} (vanilla hoi4 had {})",
       civilian_factories,
       default_civilian_factories);
   Log(LogLevel::Info) << fmt::format("\t\t\tMilitary factories: {} (vanilla hoi4 had {})",
       military_factories,
       default_military_factories);
   Log(LogLevel::Info) << fmt::format("\t\t\tDockyards: {} (vanilla hoi4 had {})",
       dockyards,
       default_dockyards);
}


hoi4::States CreateStates(const std::map<int, vic3::State>& vic3_states,
    const std::map<int, std::set<int>>& vic3_state_id_to_hoi4_provinces,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const hoi4::StrategicRegions& strategic_regions,
    const mappers::CountryMapper& country_mapper,
    const hoi4::StateCategories& state_categories,
    const std::map<int, hoi4::DefaultState>& default_states)
{
   std::vector<hoi4::State> hoi4_states;
   std::map<int, int> province_to_state_id_map;
   std::map<int, int> vic3_state_ids_to_hoi4_state_ids;
   std::unordered_map<std::string, FactoriesStruct> accumulator;

   for (const auto& [vic3_state_id, hoi4_provinces]: vic3_state_id_to_hoi4_provinces)
   {
      const auto vic3_state_itr = vic3_states.find(vic3_state_id);
      if (vic3_state_itr == vic3_states.end())
      {
         // I can't think how this would happen, so we'd better force people to the forums if it does.
         throw std::runtime_error("Something has gone very wrong.");
      }
      std::optional<std::string> state_owner;
      if (const std::optional<std::string>& vic3_state_owner = vic3_state_itr->second.GetOwnerTag();
          vic3_state_owner.has_value())
      {
         state_owner = country_mapper.GetHoiTag(*vic3_state_owner);
         if (!state_owner.has_value())
         {
            Log(LogLevel::Warning) << fmt::format("Could not get tag for owner of Vic3 state {}.", vic3_state_id);
         }
      }

      const auto initial_connected_province_sets =
          GetConnectedProvinceSets(hoi4_provinces, map_data, hoi4_province_definitions);
      auto final_connected_province_sets =
          ConsolidateProvinceSets(initial_connected_province_sets, strategic_regions.GetProvinceToStrategicRegionMap());
      if (final_connected_province_sets.size() > 1U)
      {
         Log(LogLevel::Info) << fmt::format("\tState {} was split into {} due to disconnected provinces.",
             hoi4_states.size() + 1U,
             final_connected_province_sets.size());
      }

      const int64_t total_manpower = vic3_state_itr->second.GetPopulation();
      const float total_factories = static_cast<float>(vic3_state_itr->second.GetEmployedPopulation()) / 100'000.0F;
      for (const auto& province_set: final_connected_province_sets)
      {
         int civilian_factories = 0;
         int military_factories = 0;
         int dockyards = 0;
         if (state_owner)
         {
            const auto all_factories =
                ConvertIndustry(total_factories, province_set, hoi4_provinces.size(), *state_owner, accumulator);
            civilian_factories = std::get<0>(all_factories);
            military_factories = std::get<1>(all_factories);
            dockyards = std::get<2>(all_factories);
         }

         const std::string category = state_categories.GetBestCategory(
             std::min(civilian_factories + military_factories + dockyards, static_cast<int>(MAX_FACTORY_SLOTS)));

         const int manpower = static_cast<int>(total_manpower * province_set.size() / hoi4_provinces.size());

         for (const int province: province_set)
         {
            province_to_state_id_map.emplace(province, static_cast<int>(hoi4_states.size() + 1U));
         }
         vic3_state_ids_to_hoi4_state_ids.emplace(vic3_state_id, static_cast<int>(hoi4_states.size() + 1U));
         hoi4_states.emplace_back(static_cast<int>(hoi4_states.size() + 1U),
             hoi4::StateOptions{.owner = state_owner,
                 .provinces = province_set,
                 .manpower = manpower,
                 .category = category,
                 .civilian_factories = civilian_factories,
                 .military_factories = military_factories,
                 .dockyards = dockyards});
      }
   }

   LogIndustryStats(hoi4_states, default_states);

   return {hoi4_states, province_to_state_id_map, vic3_state_ids_to_hoi4_state_ids};
}

}  // namespace


hoi4::States hoi4::StatesConverter::ConvertStates(const std::map<int, vic3::State>& states,
    const vic3::ProvinceDefinitions& vic3_province_definitions,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const hoi4::StrategicRegions& strategic_regions,
    const mappers::CountryMapper& country_mapper,
    const hoi4::StateCategories& state_categories,
    const std::map<int, DefaultState>& default_states)
{
   const std::map<std::string, int> vic3_province_to_state_id_map =
       MapVic3ProvincesToStates(states, vic3_province_definitions);
   const std::map<int, std::set<int>> vic3_state_id_to_hoi4_provinces =
       PlaceHoi4ProvincesInStates(vic3_province_to_state_id_map,
           hoi4_to_vic3_province_mappings,
           hoi4_province_definitions);
   return CreateStates(states,
       vic3_state_id_to_hoi4_provinces,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       state_categories,
       default_states);
}