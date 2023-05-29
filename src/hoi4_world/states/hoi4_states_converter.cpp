#include "src/hoi4_world/states/hoi4_states_converter.h"

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



namespace
{

constexpr int MAX_FACTORY_SLOTS = 12;


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


std::vector<int> SortVic3StatesByIndustryDescending(const vic3::Buildings& vic3_buildings,
    const std::map<int, std::set<int>>& vic3_state_id_to_hoi4_provinces)
{
   std::vector<int> vic3_state_ids;
   std::ranges::copy(vic3_state_id_to_hoi4_provinces | std::views::keys, std::back_inserter(vic3_state_ids));
   const auto byVic3Industry = [vic3_buildings](const int& lhs, const int& rhs) {
      return vic3_buildings.GetTotalGoodSalesValueInState(lhs) > vic3_buildings.GetTotalGoodSalesValueInState(rhs);
   };
   std::ranges::sort(vic3_state_ids, byVic3Industry);
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
      const auto& local_significant_provinces = state_region.GetSignificantProvinces();
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
      for (const auto& province: region.GetProvinces())
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
   const int factories_floor = static_cast<int>(std::max(factories, applied_factories));

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


std::map<int, int> CreateVictoryPoints(const std::set<int>& hoi4_provinces,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const std::map<std::string, std::string>& significant_provinces)
{
   std::map<int, int> victory_points;

   std::set<std::string> applied_types;

   for (const auto hoi4_province: hoi4_provinces)
   {
      const auto province_mapping = hoi4_to_vic3_province_mappings.find(hoi4_province);
      if (province_mapping == hoi4_to_vic3_province_mappings.end())
      {
         // shouldn't be possible, since all hoi4 provinces come from the mappings, but just in case
         continue;
      }

      for (const auto& vic3_province: province_mapping->second)
      {
         const auto& significant_province = significant_provinces.find(vic3_province);
         if (significant_province == significant_provinces.end())
         {
            continue;
         }

         if (significant_province->second == "city" && !applied_types.contains("city"))
         {
            victory_points.emplace(hoi4_province, 5);
            applied_types.insert("city");
         }
         else if (significant_province->second == "port" && !applied_types.contains("port"))
         {
            victory_points.emplace(hoi4_province, 4);
            applied_types.insert("port");
         }
         else if (significant_province->second == "farm" && !applied_types.contains("farm"))
         {
            victory_points.emplace(hoi4_province, 3);
            applied_types.insert("farm");
         }
         else if (significant_province->second == "mine" && !applied_types.contains("mine"))
         {
            victory_points.emplace(hoi4_province, 2);
            applied_types.insert("mine");
         }
         else if (significant_province->second == "wood" && !applied_types.contains("wood"))
         {
            victory_points.emplace(hoi4_province, 1);
            applied_types.insert("wood");
         }
      }
   }

   return victory_points;
}


void LogIndustryStats(const std::vector<hoi4::State>& hoi4_states,
    const std::map<int, hoi4::DefaultState>& default_states)
{
   int civilian_factories = 0;
   int military_factories = 0;
   int dockyards = 0;
   std::map<std::string, double> resources;
   for (const hoi4::State& hoi4_state: hoi4_states)
   {
      civilian_factories += hoi4_state.GetCivilianFactories();
      military_factories += hoi4_state.GetMilitaryFactories();
      dockyards += hoi4_state.GetDockyards();
      for (const auto& hoi4_state_resource: hoi4_state.GetResources())
      {
         resources[hoi4_state_resource.first] += hoi4_state_resource.second;
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

   Log(LogLevel::Info) << fmt::format("\t\tTotal factories: {} (vanilla hoi4 had {})",
       civilian_factories + military_factories + dockyards,
       default_civilian_factories + default_military_factories + default_dockyards);
   Log(LogLevel::Info) << fmt::format("\t\t\tCivilian factories: {} (vanilla hoi4 had {})",
       civilian_factories,
       default_civilian_factories);
   Log(LogLevel::Info) << fmt::format("\t\t\tMilitary factories: {} (vanilla hoi4 had {})",
       military_factories,
       default_military_factories);
   Log(LogLevel::Info) << fmt::format("\t\t\tDockyards: {} (vanilla hoi4 had {})", dockyards, default_dockyards);
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

   Log(LogLevel::Info) << fmt::format("\t\tTotal manpower: {} (vanilla hoi4 had {})", manpower, default_manpower);
}


hoi4::States CreateStates(const std::map<int, vic3::State>& vic3_states,
    const vic3::Buildings& vic3_buildings,
    const std::map<int, std::set<int>>& vic3_state_id_to_hoi4_provinces,
    const std::vector<int>& vic3_state_ids_by_vic3_industry,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const hoi4::StrategicRegions& strategic_regions,
    const mappers::CountryMapper& country_mapper,
    const hoi4::StateCategories& state_categories,
    const std::map<int, hoi4::DefaultState>& default_states,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const hoi4::CoastalProvinces& coastal_provinces,
    const hoi4::ResourcesMap& resources_map)
{
   std::vector<hoi4::State> hoi4_states;
   std::map<int, int> province_to_state_id_map;
   std::map<int, int> vic3_state_ids_to_hoi4_state_ids;
   std::unordered_map<std::string, FactoriesStruct> accumulator;
   const std::set<int> wasteland_provinces = GetWastelandProvinces(default_states);
   std::map<std::string, std::string> significant_provinces = GetAllSignificantProvinces(vic3_state_regions);
   std::map<std::string, std::string> vic3_provinces_to_state_names = MapVic3ProvincesToStateNames(vic3_state_regions);
   std::map<std::string, std::string> hoi4_state_names_to_vic3_state_names;

   for (const auto& vic3_state_id: vic3_state_ids_by_vic3_industry)
   {
      const auto& hoi4_provinces = vic3_state_id_to_hoi4_provinces.at(vic3_state_id);
      const auto vic3_state_itr = vic3_states.find(vic3_state_id);
      if (vic3_state_itr == vic3_states.end())
      {
         // I can't think how this would happen, so we'd better force people to the forums if it does.
         throw std::runtime_error("Something has gone very wrong.");
      }
      std::optional<std::string> state_owner;
      if (const std::optional<int>& vic3_state_owner = vic3_state_itr->second.GetOwnerNumber();
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
      const auto [non_wasteland_province_sets, wasteland_province_sets] =
          SortProvinceSetsByWastelandStatus(initial_connected_province_sets, wasteland_provinces);
      const auto final_connected_province_sets =
          ConsolidateProvinceSets(non_wasteland_province_sets, strategic_regions.GetProvinceToStrategicRegionMap());
      const auto final_wasteland_connected_province_sets =
          ConsolidateProvinceSets(wasteland_province_sets, strategic_regions.GetProvinceToStrategicRegionMap());
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
          static_cast<float>(vic3_buildings.GetTotalIndustryGoodSalesValueInState(vic3_state_id)) / 125'000.0F;
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

         const hoi4::Resources resources = AssignResources(province_set, resources_map);

         const std::string category = state_categories.GetBestCategory(
             std::min(civilian_factories + military_factories + dockyards, static_cast<int>(MAX_FACTORY_SLOTS)));


         const int manpower = static_cast<int>(
             total_manpower * static_cast<int>(province_set.size()) / static_cast<int>(hoi4_provinces.size()));

         const std::map<int, int> victory_points =
             CreateVictoryPoints(province_set, hoi4_to_vic3_province_mappings, significant_provinces);

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
                 .naval_base_level = naval_base_level});
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

   LogIndustryStats(hoi4_states, default_states);
   LogManpowerStats(hoi4_states, default_states);

   return {hoi4_states,
       province_to_state_id_map,
       vic3_state_ids_to_hoi4_state_ids,
       hoi4_state_names_to_vic3_state_names};
}

}  // namespace


hoi4::States hoi4::ConvertStates(const std::map<int, vic3::State>& states,
    const vic3::ProvinceDefinitions& vic3_province_definitions,
    const vic3::Buildings& vic3_buildings,
    const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
    const maps::MapData& map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const hoi4::StrategicRegions& strategic_regions,
    const mappers::CountryMapper& country_mapper,
    const hoi4::StateCategories& state_categories,
    const std::map<int, DefaultState>& default_states,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const CoastalProvinces& coastal_provinces,
    const ResourcesMap& resources_map)
{
   const std::map<std::string, int> vic3_province_to_state_id_map =
       MapVic3ProvincesToStates(states, vic3_province_definitions);
   const std::map<int, std::set<int>> vic3_state_id_to_hoi4_provinces =
       PlaceHoi4ProvincesInStates(vic3_province_to_state_id_map,
           hoi4_to_vic3_province_mappings,
           hoi4_province_definitions);
   const std::vector<int> vic3_state_ids_by_vic3_industry =
       SortVic3StatesByIndustryDescending(vic3_buildings, vic3_state_id_to_hoi4_provinces);
   return CreateStates(states,
       vic3_buildings,
       vic3_state_id_to_hoi4_provinces,
       vic3_state_ids_by_vic3_industry,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       state_categories,
       default_states,
       vic3_state_regions,
       coastal_provinces,
       resources_map);
}