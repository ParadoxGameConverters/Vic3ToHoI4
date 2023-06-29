#include "src/hoi4_world/map/railways_converter.h"

#include <numeric>
#include <queue>
#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/map/possible_path.h"



namespace
{

using ProvinceType = std::string;


std::map<std::string, ProvinceType> GatherVic3SignificantProvinces(
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions)
{
   std::map<std::string, ProvinceType> vic3_significant_provinces;

   for (const vic3::StateRegion& vic3_state_region: vic3_state_regions | std::views::values)
   {
      const std::map<std::string, std::string>& significant_provinces = vic3_state_region.GetSignificantProvinces();
      for (const auto& [province, type]: significant_provinces)
      {
         vic3_significant_provinces.emplace(province, type);
      }
   }

   return vic3_significant_provinces;
}


std::vector<int> DetermineAllowedEndpointAmounts(const std::vector<hoi4::State>& states)
{
   std::vector<int> allowed_endpoint_amounts;
   allowed_endpoint_amounts.resize(states.size() + 1);
   for (const hoi4::State& state: states)
   {
      const int allowed_endpoints = std::max(1, static_cast<int>(state.GetProvinces().size() / 6));
      allowed_endpoint_amounts[state.GetId()] = allowed_endpoints;
   }

   return allowed_endpoint_amounts;
}


std::map<int, std::string> LimitProvinces(int limit, const std::map<int, std::string>& provinces)
{
   // quick return if all provinces are allowed
   if (limit >= provinces.size())
   {
      return provinces;
   }

   // always include cities and ports
   std::map<int, std::string> limited_provinces;
   for (const auto& [province, type]: provinces)
   {
      if (type == "city")
      {
         limited_provinces.emplace(province, type);
      }
      if (type == "port")
      {
         limited_provinces.emplace(province, type);
      }
   }

   // add remaining provinces up to limit. Because we use a map, we don't need to worry about double-inserts
   for (const auto& [province, type]: provinces)
   {
      if (limit <= limited_provinces.size())
      {
         return limited_provinces;
      }

      limited_provinces.emplace(province, type);
   }

   return limited_provinces;
}


std::map<hoi4::StateId, std::map<int, ProvinceType>> ConvertVic3SignificantProvincesToHoi4SignificantProvinces(
    const std::map<std::string, ProvinceType>& vic3_significant_provinces,
    const mappers::ProvinceMapper& province_mapper,
    const hoi4::States& states)
{
   std::map<hoi4::StateId, std::map<int, ProvinceType>> all_hoi4_significant_provinces;

   const std::map<int, hoi4::StateId>& province_to_state_id_map = states.province_to_state_id_map;

   for (const auto& [vic3_significant_province, province_type]: vic3_significant_provinces)
   {
      const std::vector<int> hoi4_provinces = province_mapper.GetVic3ToHoi4ProvinceMapping(vic3_significant_province);
      if (hoi4_provinces.empty())
      {
         Log(LogLevel::Info) << fmt::format("No hoi4 provinces for significant vic3 province {}.",
             vic3_significant_province);
         continue;
      }

      int hoi4_province = hoi4_provinces.front();

      const auto state_itr = province_to_state_id_map.find(hoi4_province);
      if (state_itr == province_to_state_id_map.end())
      {
         continue;
      }

      const hoi4::StateId& state_id = state_itr->second;
      if (auto [itr, success] = all_hoi4_significant_provinces.emplace(state_id,
              std::map<int, ProvinceType>{{hoi4_province, province_type}});
          !success)
      {
         itr->second.emplace(hoi4_province, province_type);
      }
   }

   std::map<hoi4::StateId, std::map<int, ProvinceType>> restricted_hoi4_significant_provinces;

   const std::vector<int> allowed_endpoint_amounts = DetermineAllowedEndpointAmounts(states.states);
   for (const auto& [state_id, significant_provinces]: all_hoi4_significant_provinces)
   {
      if (state_id + 1 > allowed_endpoint_amounts.size())
      {
         continue;
      }
      if (states.states.at(state_id - 1).GetCategory() == "wasteland")
      {
         continue;
      }

      const std::map<int, std::string> limited_provinces =
          LimitProvinces(allowed_endpoint_amounts[state_id], significant_provinces);
      restricted_hoi4_significant_provinces.emplace(state_id, limited_provinces);
   }

   return restricted_hoi4_significant_provinces;
}


std::optional<int> GetMostSignificantProvince(const std::map<int, ProvinceType>& provinces)
{
   for (const auto& [province, type]: provinces)
   {
      if (type == "city")
      {
         return province;
      }
   }
   for (const auto& [province, type]: provinces)
   {
      if (type == "port")
      {
         return province;
      }
   }
   if (!provinces.empty())
   {
      return provinces.begin()->first;
   }

   return std::nullopt;
}


std::vector<std::pair<int, int>> DetermineIntrastateEndpoints(
    const std::map<hoi4::StateId, std::map<int, ProvinceType>>& significant_provinces)
{
   std::vector<std::pair<int, int>> intrastate_endpoints;

   for (const std::map<int, ProvinceType>& state_significant_provinces: significant_provinces | std::views::values)
   {
      const std::optional<int> most_significant_province = GetMostSignificantProvince(state_significant_provinces);
      if (!most_significant_province)
      {
         continue;
      }

      for (int province: state_significant_provinces | std::views::keys)
      {
         if (province == *most_significant_province)
         {
            continue;
         }

         intrastate_endpoints.emplace_back(*most_significant_province, province);
      }
   }

   return intrastate_endpoints;
}


std::optional<float> GetInfrastructureLevel(int province, const hoi4::States& states)
{
   const auto state_itr = states.province_to_state_id_map.find(province);
   if (state_itr == states.province_to_state_id_map.end())
   {
      return std::nullopt;
   }

   if (states.states.size() < state_itr->second)
   {
      return std::nullopt;
   }

   return states.states.at(state_itr->second - 1).GetVic3Infrastructure();
}


float DeterminePathDevelopment(const std::vector<int>& provinces, const hoi4::States& states)
{
   float total_development = 0.F;

   for (const int province: provinces)
   {
      if (std::optional<float> new_development = GetInfrastructureLevel(province, states); new_development)
      {
         total_development += *new_development;
      }
   }

   return total_development / static_cast<float>(provinces.size());
}


constexpr float urban_cost = 1.F;
constexpr float plains_cost = 2.F;
constexpr float forest_cost = 3.F;
constexpr float hills_cost = 4.F;
constexpr float desert_cost = 5.F;
constexpr float marsh_cost = 6.F;
constexpr float jungle_cost = 7.F;
constexpr float mountain_cost = 8.F;
constexpr float unhandled_cost = 100.F;
float GetCostForTerrainType(std::string_view terrain_type)
{
   if (terrain_type == "urban")
   {
      return urban_cost;
   }
   if (terrain_type == "plains")
   {
      return plains_cost;
   }
   if (terrain_type == "forest")
   {
      return forest_cost;
   }
   if (terrain_type == "hills")
   {
      return hills_cost;
   }
   if (terrain_type == "desert")
   {
      return desert_cost;
   }
   if (terrain_type == "marsh")
   {
      return marsh_cost;
   }
   if (terrain_type == "jungle")
   {
      return jungle_cost;
   }
   if (terrain_type == "mountain")
   {
      return mountain_cost;
   }

   Log(LogLevel::Warning) << fmt::format("Unhandled terrain type {}. Please inform the converter team.", terrain_type);
   return unhandled_cost;
}


double GetDistanceBetweenProvinces(const std::string& province_one,
    const std::string& province_two,
    const maps::MapData& hoi4_map_data)
{
   const auto province_one_centermost_point = hoi4_map_data.GetCentermostPoint(province_one);
   if (!province_one_centermost_point)
   {
      return std::numeric_limits<double>::max();
   }

   const auto province_two_centermost_point = hoi4_map_data.GetCentermostPoint(province_two);
   if (!province_two_centermost_point)
   {
      return std::numeric_limits<double>::max();
   }

   const int delta_x = province_one_centermost_point->x - province_two_centermost_point->x;
   const int delta_y = province_one_centermost_point->y - province_two_centermost_point->y;
   return std::sqrt(delta_x * delta_x + delta_y * delta_y);
}


double DeterminePathCost(const maps::ProvinceDefinitions& hoi4_province_definitions,
    const std::string& neighbor_number,
    const std::string& last_province,
    const maps::MapData& hoi4_map_data)
{
   const std::optional<std::string> possible_current_terrain_type =
       hoi4_province_definitions.GetTerrainType(last_province);
   if (!possible_current_terrain_type)
   {
      return std::numeric_limits<double>::max();
   }

   const std::optional<std::string> possible_neighbor_terrain_type =
       hoi4_province_definitions.GetTerrainType(neighbor_number);
   if (!possible_neighbor_terrain_type)
   {
      return std::numeric_limits<double>::max();
   }

   return static_cast<double>(GetCostForTerrainType(*possible_current_terrain_type) +
                              GetCostForTerrainType(*possible_neighbor_terrain_type) / 2.0F) *
          GetDistanceBetweenProvinces(neighbor_number, last_province, hoi4_map_data);
}


void FindNextPaths(const hoi4::PossiblePath& possible_railway_path,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const std::set<int>& allowed_provinces,
    const std::set<int>& reached_provinces,
    std::priority_queue<hoi4::PossiblePath>& possible_railway_paths)
{
   const int last_province = possible_railway_path.GetLastProvince();
   for (const auto& neighbor_number_string: hoi4_map_data.GetNeighbors(std::to_string(last_province)))
   {
      int neighbor_number = 0;
      try
      {
         neighbor_number = std::stoi(neighbor_number_string);
      }
      catch (...)
      {
         continue;
      }
      if (!allowed_provinces.contains(neighbor_number))
      {
         continue;
      }
      if (reached_provinces.contains(neighbor_number))
      {
         continue;
      }

      if (!hoi4_province_definitions.IsLandProvince(neighbor_number_string))
      {
         continue;
      }

      hoi4::PossiblePath new_possible_railway_path = possible_railway_path;
      new_possible_railway_path.AddProvince(neighbor_number,
          DeterminePathCost(hoi4_province_definitions,
              neighbor_number_string,
              std::to_string(last_province),
              hoi4_map_data));
      possible_railway_paths.push(new_possible_railway_path);
   }
}


std::set<int> DetermineAllowedProvinces(int start_province, int end_province, const hoi4::States& states)
{
   std::set<int> allowed_provinces;

   const auto& start_itr = states.province_to_state_id_map.find(start_province);
   if (start_itr == states.province_to_state_id_map.end() || start_itr->second > states.states.size())
   {
      return {};
   }

   const std::set<int>& start_state_provinces = states.states.at(start_itr->second - 1).GetProvinces();
   allowed_provinces.insert(start_state_provinces.begin(), start_state_provinces.end());

   const auto& end_itr = states.province_to_state_id_map.find(end_province);
   if (end_itr == states.province_to_state_id_map.end() || end_itr->second > states.states.size())
   {
      return {};
   }

   const std::set<int>& end_state_provinces = states.states.at(end_itr->second - 1).GetProvinces();
   allowed_provinces.insert(end_state_provinces.begin(), end_state_provinces.end());

   return allowed_provinces;
}


std::optional<hoi4::PossiblePath> FindPath(int start_province,
    int end_province,
    const std::set<int>& allowed_provinces,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions)
{
   std::priority_queue<hoi4::PossiblePath> possible_railway_paths;
   std::set reached_provinces{start_province};

   const hoi4::PossiblePath starting_path(start_province);
   possible_railway_paths.push(starting_path);

   while (!possible_railway_paths.empty())
   {
      hoi4::PossiblePath possible_railway_path = possible_railway_paths.top();

      const int last_province = possible_railway_path.GetLastProvince();
      if (possible_railway_path.GetLastProvince() == end_province)
      {
         return possible_railway_paths.top();
      }
      reached_provinces.insert(last_province);

      possible_railway_paths.pop();

      FindNextPaths(possible_railway_path,
          hoi4_map_data,
          hoi4_province_definitions,
          allowed_provinces,
          reached_provinces,
          possible_railway_paths);
   }

   return std::nullopt;
}


void BuildPath(int start_province,
    int end_province,
    const hoi4::States& states,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    std::vector<hoi4::PossiblePath>& possible_paths)
{
   const std::set<int> allowed_provinces = DetermineAllowedProvinces(start_province, end_province, states);
   auto possible_path =
       FindPath(start_province, end_province, allowed_provinces, hoi4_map_data, hoi4_province_definitions);
   if (!possible_path)
   {
      return;
   }

   possible_path->SetDevelopment(DeterminePathDevelopment(possible_path->GetProvinces(), states));
   possible_paths.emplace_back(*possible_path);
}


std::vector<hoi4::PossiblePath> FindAllHoi4Paths(const std::vector<std::pair<int, int>>& hoi4_endpoints,
    const hoi4::States& states,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions)
{
   std::vector<hoi4::PossiblePath> possible_paths;
   for (const auto& [start_point, end_point]: hoi4_endpoints)
   {
      BuildPath(start_point, end_point, states, hoi4_map_data, hoi4_province_definitions, possible_paths);
   }

   return possible_paths;
}


std::map<int, std::set<int>> DetermineNeighboringStates(const std::map<int, int>& province_to_state_id_map,
    const maps::MapData& hoi4_map_data)
{
   std::map<int, std::set<int>> neighboring_states;

   for (const auto& [province_id, state_id]: province_to_state_id_map)
   {
      const std::set<std::string> province_neighbors = hoi4_map_data.GetNeighbors(std::to_string(province_id));
      for (const std::string& province_neighbor: province_neighbors)
      {
         int province_neighbor_id = 0;
         try
         {
            province_neighbor_id = std::stoi(province_neighbor);
         }
         catch (...)
         {
            continue;
         }

         int neighbor_state_id = 0;
         if (const auto& neighbor_state_mapping = province_to_state_id_map.find(province_neighbor_id);
             neighbor_state_mapping != province_to_state_id_map.end())
         {
            neighbor_state_id = neighbor_state_mapping->second;
         }
         else
         {
            continue;
         }
         if (state_id == neighbor_state_id)
         {
            continue;
         }

         auto [iterator, success] = neighboring_states.emplace(state_id, std::set{neighbor_state_id});
         if (!success)
         {
            iterator->second.emplace(neighbor_state_id);
         }
      }
   }

   return neighboring_states;
}


std::set<int> GetSignificantProvincesInState(int state_id,
    const std::map<hoi4::StateId, std::map<int, ProvinceType>>& significant_provinces_in_states)
{
   std::set<int> significant_provinces;

   const auto significant_provinces_in_states_itr = significant_provinces_in_states.find(state_id);
   if (significant_provinces_in_states_itr != significant_provinces_in_states.end())
   {
      for (int province: significant_provinces_in_states_itr->second | std::views::keys)
      {
         significant_provinces.insert(province);
      }
   }
   return significant_provinces;
}


std::vector<std::pair<int, int>> EnumerateAllInterstateConnections(const std::set<int>& first_state_significant_points,
    const std::set<int>& second_state_significant_points)
{
   std::set<std::pair<int, int>> interstate_connections_set;

   for (const int first_state_point: first_state_significant_points)
   {
      for (const int second_state_point: second_state_significant_points)
      {
         interstate_connections_set.emplace(first_state_point, second_state_point);
      }
   }

   std::vector<std::pair<int, int>> interstate_connections;
   for (const auto& interstate_connection: interstate_connections_set)
   {
      if (interstate_connection.first == interstate_connection.second)
      {
         continue;
      }
      interstate_connections.push_back(interstate_connection);
   }

   return interstate_connections;
}


std::vector<hoi4::PossiblePath> ConnectStatesWithRailways(
    const std::map<hoi4::StateId, std::map<int, ProvinceType>>& significant_hoi4_provinces,
    const hoi4::States& hoi4_states,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions)
{
   const std::map<int, std::set<int>> neighboring_states =
       DetermineNeighboringStates(hoi4_states.province_to_state_id_map, hoi4_map_data);

   std::vector<hoi4::PossiblePath> interstate_paths;
   for (const hoi4::State& state: hoi4_states.states)
   {
      int id = state.GetId();
      const std::set<int>& state_significant_provinces = GetSignificantProvincesInState(id, significant_hoi4_provinces);

      const auto neighbors_itr = neighboring_states.find(id);
      if (neighbors_itr == neighboring_states.end())
      {
         continue;
      }
      for (const int neighbor_id: neighbors_itr->second)
      {
         // skip connections already done from the other direction
         if (neighbor_id < id)
         {
            continue;
         }

         const std::set<int>& neighbor_significant_provinces =
             GetSignificantProvincesInState(neighbor_id, significant_hoi4_provinces);
         const std::vector<std::pair<int, int>> interstate_connections =
             EnumerateAllInterstateConnections(state_significant_provinces, neighbor_significant_provinces);
         std::vector<hoi4::PossiblePath> all_interstate_paths =
             FindAllHoi4Paths(interstate_connections, hoi4_states, hoi4_map_data, hoi4_province_definitions);
         if (all_interstate_paths.empty())
         {
            continue;
         }

         std::ranges::sort(all_interstate_paths,
             [](const hoi4::PossiblePath& first_path, const hoi4::PossiblePath& second_path) {
                return first_path.GetCost() < second_path.GetCost();
             });
         interstate_paths.push_back(all_interstate_paths.front());
      }
   }

   return interstate_paths;
}


std::set<int> ListEndpoints(const std::vector<hoi4::PossiblePath>& all_paths)
{
   std::set<int> endpoints;
   for (const auto& path: all_paths)
   {
      endpoints.insert(path.GetFirstProvince());
      endpoints.insert(path.GetLastProvince());
   }

   return endpoints;
}


std::vector<hoi4::PossiblePath> SplitPaths(const std::vector<hoi4::PossiblePath>& all_paths, const hoi4::States& states)
{
   std::vector<hoi4::PossiblePath> split_paths;
   std::set<std::pair<int, int>> handled_paths;

   const std::set<int> endpoints = ListEndpoints(all_paths);

   for (const hoi4::PossiblePath& path: all_paths)
   {
      hoi4::PossiblePath split_path(path.GetFirstProvince());
      for (int province: path.GetProvinces())
      {
         if (province == path.GetFirstProvince())
         {
            continue;
         }

         split_path.AddProvince(province, 0.0);

         if (province == path.GetLastProvince())
         {
            continue;
         }

         if (!endpoints.contains(province))
         {
            continue;
         }

         std::pair<int, int> current_endpoints = {split_path.GetFirstProvince(), split_path.GetLastProvince()};
         if (!handled_paths.contains(current_endpoints))
         {
            split_paths.push_back(split_path);
            handled_paths.insert(current_endpoints);
         }
         split_path.ReplaceProvinces({province});
      }
      split_path.SetDevelopment(DeterminePathDevelopment(split_path.GetProvinces(), states));

      std::pair<int, int> current_endpoints = {split_path.GetFirstProvince(), split_path.GetLastProvince()};
      if (!handled_paths.contains(current_endpoints))
      {
         split_paths.push_back(split_path);
         handled_paths.insert(current_endpoints);
      }
   }

   return split_paths;
}


std::set<int> GatherVictoryPointLocations(const hoi4::States& hoi4_states)
{
   std::set<int> victory_point_positions;

   for (const hoi4::State& state: hoi4_states.states)
   {
      for (int victory_point_position: state.GetVictoryPoints() | std::views::keys)
      {
         victory_point_positions.insert(victory_point_position);
      }
   }

   return victory_point_positions;
}


std::set<int> GatherNavalBaseLocations(const hoi4::States& hoi4_states)
{
   std::set<int> naval_base_positions;

   for (const hoi4::State& state: hoi4_states.states)
   {
      if (const std::optional<int> possible_naval_base = state.GetNavalBaseLocation(); possible_naval_base)
      {
         naval_base_positions.insert(*possible_naval_base);
      }
   }

   return naval_base_positions;
}


std::map<int, int> CountInstancesOfEndpoints(const std::vector<hoi4::PossiblePath>& all_paths)
{
   std::map<int, int> num_uses_of_endpoints;
   for (const hoi4::PossiblePath& path: all_paths)
   {
      if (auto [itr, success] = num_uses_of_endpoints.emplace(path.GetFirstProvince(), 1); !success)
      {
         itr->second++;
      }

      if (auto [itr, success] = num_uses_of_endpoints.emplace(path.GetLastProvince(), 1); !success)
      {
         itr->second++;
      }
   }

   return num_uses_of_endpoints;
}


std::vector<hoi4::PossiblePath> TrimPaths(const std::vector<hoi4::PossiblePath>& all_paths,
    const std::set<int>& victory_point_locations,
    const std::set<int>& naval_base_locations)
{
   std::vector<hoi4::PossiblePath> trimmed_paths;

   const std::map<int, int> num_endpoint_counts = CountInstancesOfEndpoints(all_paths);

   for (const hoi4::PossiblePath& path: all_paths)
   {
      const auto first_itr = num_endpoint_counts.find(path.GetFirstProvince());
      if (first_itr == num_endpoint_counts.end())
      {
         continue;
      }
      const auto last_itr = num_endpoint_counts.find(path.GetLastProvince());
      if (last_itr == num_endpoint_counts.end())
      {
         continue;
      }

      if (first_itr->second == 1 && !victory_point_locations.contains(path.GetFirstProvince()) &&
          !naval_base_locations.contains(path.GetFirstProvince()))
      {
         continue;
      }
      if (last_itr->second == 1 && !victory_point_locations.contains(path.GetLastProvince()) &&
          !naval_base_locations.contains(path.GetLastProvince()))
      {
         continue;
      }

      trimmed_paths.push_back(path);
   }

   return trimmed_paths;
}


std::map<int, std::vector<hoi4::PossiblePath>> MapEndpointsAndPaths(const std::vector<hoi4::PossiblePath>& paths)
{
   std::map<int, std::vector<hoi4::PossiblePath>> endpoints_to_paths;
   for (const hoi4::PossiblePath& path: paths)
   {
      const auto first_province = path.GetFirstProvince();
      const auto last_province = path.GetLastProvince();

      if (auto [iterator, success] = endpoints_to_paths.emplace(first_province, std::vector{path}); !success)
      {
         if (std::ranges::none_of(iterator->second, [path](const hoi4::PossiblePath& stored_path) {
                return stored_path == path;
             }))
         {
            iterator->second.push_back(path);
         }
      }
      if (auto [iterator, success] = endpoints_to_paths.emplace(last_province, std::vector{path}); !success)
      {
         if (std::ranges::none_of(iterator->second, [path](const hoi4::PossiblePath& stored_path) {
                return stored_path == path;
             }))
         {
            iterator->second.push_back(path);
         }
      }
   }

   return endpoints_to_paths;
}


bool EndpointIsRemovable(const int endpoint, const std::set<int>& naval_locations, const std::set<int>& vp_locations)
{
   return !naval_locations.contains(endpoint) && !vp_locations.contains(endpoint);
}


hoi4::PossiblePath MergeTwoPaths(const int join_point,
    const hoi4::PossiblePath& path_one,
    const hoi4::PossiblePath& path_two,
    const hoi4::States& states)
{
   hoi4::PossiblePath merged_path;

   std::vector<int> merged_provinces;
   if (const int path_one_first_province = path_one.GetFirstProvince(); path_one_first_province == join_point)
   {
      std::copy(path_one.GetProvinces().rbegin(), path_one.GetProvinces().rend(), std::back_inserter(merged_provinces));
   }
   else
   {
      std::ranges::copy(path_one.GetProvinces(), std::back_inserter(merged_provinces));
   }

   merged_provinces.pop_back();
   if (const int path_two_first_province = path_two.GetFirstProvince(); path_two_first_province == join_point)
   {
      std::ranges::copy(path_two.GetProvinces(), std::back_inserter(merged_provinces));
   }
   else
   {
      std::copy(path_two.GetProvinces().rbegin(), path_two.GetProvinces().rend(), std::back_inserter(merged_provinces));
   }

   merged_path.ReplaceProvinces(merged_provinces);
   merged_path.SetDevelopment(DeterminePathDevelopment(merged_path.GetProvinces(), states));

   return merged_path;
}


std::vector<hoi4::PossiblePath> MergePaths(const std::vector<hoi4::PossiblePath>& paths,
    const std::set<int>& naval_locations,
    const std::set<int>& vp_locations,
    const hoi4::States& states)
{
   std::map<int, std::vector<hoi4::PossiblePath>> endpoints_to_paths = MapEndpointsAndPaths(paths);

   std::vector<hoi4::PossiblePath> merged_paths;
   std::set<int> removed_endpoints;
   for (const auto& [endpoint, endpoint_paths]: endpoints_to_paths)
   {
      if (endpoint_paths.size() == 2 && EndpointIsRemovable(endpoint, naval_locations, vp_locations))
      {
         removed_endpoints.insert(endpoint);
         const hoi4::PossiblePath merged_path = MergeTwoPaths(endpoint, endpoint_paths[0], endpoint_paths[1], states);
         merged_paths.push_back(merged_path);
         continue;
      }
   }

   for (const auto& [endpoint, mapped_paths]: endpoints_to_paths)
   {
      if (removed_endpoints.contains(endpoint))
      {
         continue;
      }
      for (const hoi4::PossiblePath& path: mapped_paths)
      {
         const int first_province = path.GetFirstProvince();
         const int last_province = path.GetLastProvince();

         if (endpoint == first_province && !removed_endpoints.contains(first_province) &&
             !removed_endpoints.contains(last_province))
         {
            merged_paths.push_back(path);
         }
      }
   }

   return merged_paths;
}


std::tuple<float, float, float, float> DetermineRailwayLevelRequirements(const std::vector<hoi4::PossiblePath>& paths)
{
   if (paths.empty())
   {
      return {0.F, 0.F, 0.F, 0.F};
   }

   std::vector<float> development_levels;
   development_levels.reserve(paths.size());
   std::ranges::transform(paths, std::back_inserter(development_levels), [](const hoi4::PossiblePath& path) {
      return path.GetDevelopment();
   });
   std::ranges::sort(development_levels);

   const float level_four = development_levels.at(development_levels.size() * 99 / 100);
   const float level_three = development_levels.at(development_levels.size() * 96 / 100);
   const float level_two = development_levels.at(development_levels.size() * 50 / 100);
   const float level_one =
       std::accumulate(development_levels.begin(), development_levels.end(), 0.F) / development_levels.size() / 10.F;

   return {level_one, level_two, level_three, level_four};
}


int DetermineRailwayLevel(const hoi4::PossiblePath& path,
    const float level_one,
    const float level_two,
    const float level_three,
    const float level_four)
{
   const float total_development = path.GetDevelopment();
   if (total_development >= level_four)
   {
      return 4;
   }
   if (total_development >= level_three)
   {
      return 3;
   }
   if (total_development >= level_two)
   {
      return 2;
   }
   if (total_development >= level_one)
   {
      return 1;
   }

   return 0;
}


std::vector<hoi4::Railway> GetRailwaysFromPaths(const std::vector<hoi4::PossiblePath>& paths)
{
   std::vector<hoi4::Railway> railways;

   const auto [level_one_requirement, level_two_requirement, level_three_requirement, level_four_requirement] =
       DetermineRailwayLevelRequirements(paths);

   for (const hoi4::PossiblePath& possible_path: paths)
   {
      hoi4::Railway railway(DetermineRailwayLevel(possible_path,
                                level_one_requirement,
                                level_two_requirement,
                                level_three_requirement,
                                level_four_requirement),
          possible_path.GetProvinces());
      railways.push_back(railway);
   }

   return railways;
}


std::set<int> GetSupplyNodesFromPaths(const std::vector<hoi4::PossiblePath>& paths,
    const std::set<int>& naval_base_locations)
{
   std::set<int> endpoints;

   for (const hoi4::PossiblePath& possible_path: paths)
   {
      if (int province = possible_path.GetFirstProvince(); !naval_base_locations.contains(province))
      {
         endpoints.insert(province);
      }
      if (int province = possible_path.GetLastProvince(); !naval_base_locations.contains(province))
      {
         endpoints.insert(province);
      }
   }

   return endpoints;
}

}  // namespace



hoi4::Railways hoi4::ConvertRailways(const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const mappers::ProvinceMapper& province_mapper,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    const States& hoi4_states)
{
   Log(LogLevel::Info) << "\tCreating railways";
   const std::map<std::string, ProvinceType> significant_vic3_provinces =
       GatherVic3SignificantProvinces(vic3_state_regions);
   const std::map<StateId, std::map<int, ProvinceType>> significant_hoi4_provinces =
       ConvertVic3SignificantProvincesToHoi4SignificantProvinces(significant_vic3_provinces,
           province_mapper,
           hoi4_states);

   const std::vector<std::pair<int, int>> intrastate_endpoints =
       DetermineIntrastateEndpoints(significant_hoi4_provinces);
   const std::vector<hoi4::PossiblePath> intrastate_paths =
       FindAllHoi4Paths(intrastate_endpoints, hoi4_states, hoi4_map_data, hoi4_province_definitions);

   const std::vector<hoi4::PossiblePath> interstate_paths =
       ConnectStatesWithRailways(significant_hoi4_provinces, hoi4_states, hoi4_map_data, hoi4_province_definitions);
   std::vector<hoi4::PossiblePath> all_paths = intrastate_paths;
   all_paths.insert(all_paths.end(), interstate_paths.begin(), interstate_paths.end());

   const std::set<int> vp_locations = GatherVictoryPointLocations(hoi4_states);
   const std::set<int> naval_base_locations = GatherNavalBaseLocations(hoi4_states);
   const std::vector<hoi4::PossiblePath> split_paths = SplitPaths(all_paths, hoi4_states);
   const std::vector<hoi4::PossiblePath> trimmed_paths = TrimPaths(split_paths, vp_locations, naval_base_locations);
   const std::vector<hoi4::PossiblePath> merged_paths =
       MergePaths(trimmed_paths, naval_base_locations, vp_locations, hoi4_states);

   const std::vector<Railway> railways = GetRailwaysFromPaths(merged_paths);
   const std::set<int> endpoints = GetSupplyNodesFromPaths(merged_paths, naval_base_locations);

   return {railways, endpoints};
}