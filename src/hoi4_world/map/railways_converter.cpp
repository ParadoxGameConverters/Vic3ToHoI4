#include "src/hoi4_world/map/railways_converter.h"

#include <queue>
#include <ranges>

#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/map/possible_path.h"


#pragma optimize("", off)
namespace
{

std::vector<std::pair<std::string, std::string>> DetermineVic3Endpoints(
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions)
{
   std::vector<std::pair<std::string, std::string>> vic3_endpoints;

   for (const vic3::StateRegion& vic3_state_region: vic3_state_regions | std::views::values)
   {
      const std::map<std::string, std::string> significant_provinces = vic3_state_region.GetSignificantProvinces();
      std::string city_province;
      std::vector<std::string> other_signficant_provinces;
      for (const auto& [province, type]: significant_provinces)
      {
         if (type == "city")
         {
            city_province = province;
         }
         else
         {
            other_signficant_provinces.push_back(province);
         }
      }
      if (city_province.empty())
      {
         continue;
      }

      for (const auto& significant_province: significant_provinces | std::views::keys)
      {
         if (significant_province == city_province)
         {
            continue;
         }

         vic3_endpoints.emplace_back(city_province, significant_province);
      }
   }

   return vic3_endpoints;
}


std::vector<std::pair<int, int>> ConvertVic3EndpointsToHoi4Endpoints(
    const std::vector<std::pair<std::string, std::string>>& vic3_endpoints,
    const mappers::ProvinceMapper& province_mapper)
{
   std::vector<std::pair<int, int>> hoi4_endpoints;

   for (const auto& [vic3_start_point, vic3_end_point]: vic3_endpoints)
   {
      std::vector<int> hoi4_province_start_points = province_mapper.GetVic3ToHoi4ProvinceMapping(vic3_start_point);
      std::vector<int> hoi4_province_end_points = province_mapper.GetVic3ToHoi4ProvinceMapping(vic3_end_point);
      if (hoi4_province_start_points.empty() || hoi4_province_end_points.empty())
      {
         Log(LogLevel::Info) << fmt::format("No hoi4 provinces for significant vi3 province {} or {}.",
             vic3_start_point,
             vic3_end_point);
         continue;
      }

      int hoi4_province_start_point = *hoi4_province_start_points.begin();
      int hoi4_province_end_point = *hoi4_province_end_points.begin();
      if (hoi4_province_start_point == hoi4_province_end_point)
      {
         continue;
      }

      hoi4_endpoints.emplace_back(hoi4_province_start_point, hoi4_province_end_point);
   }

   return hoi4_endpoints;
}


constexpr int urban_cost = 1;
constexpr int plains_cost = 2;
constexpr int forest_cost = 3;
constexpr int hills_cost = 3;
constexpr int desert_cost = 5;
constexpr int marsh_cost = 5;
constexpr int jungle_cost = 8;
constexpr int mountain_cost = 8;
constexpr int unhandled_cost = 100;
int GetCostForTerrainType(std::string_view terrain_type)
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


std::optional<maps::Point> GetCentermostPoint(const std::string& province, const maps::MapData& hoi4_map_data)
{
   const auto possible_province_points = hoi4_map_data.GetProvincePoints(province);
   if (!possible_province_points)
   {
      return std::nullopt;
   }
   return possible_province_points->GetCentermostPoint();
}


double GetDistanceBetweenProvinces(const std::string& province_one,
    const std::string& province_two,
    const maps::MapData& hoi4_map_data)
{
   const auto province_one_centermost_point = GetCentermostPoint(province_one, hoi4_map_data);
   if (!province_one_centermost_point)
   {
      return std::numeric_limits<double>::max();
   }

   const auto province_two_centermost_point = GetCentermostPoint(province_two, hoi4_map_data);
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
   const std::optional<std::string> possible_terrain_type = hoi4_province_definitions.GetTerrainType(neighbor_number);
   if (!possible_terrain_type)
   {
      return std::numeric_limits<double>::max();
   }

   return GetCostForTerrainType(*possible_terrain_type) *
          GetDistanceBetweenProvinces(neighbor_number, last_province, hoi4_map_data);
}


void FindNextPaths(const hoi4::PossiblePath& possible_railway_path,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    std::set<int>& reached_provinces,
    std::priority_queue<hoi4::PossiblePath>& possible_railway_paths)
{
   const std::optional<int> last_province = possible_railway_path.GetLastProvince();
   if (!last_province)
   {
      return;
   }

   for (const auto& neighbor_number_string: hoi4_map_data.GetNeighbors(std::to_string(*last_province)))
   {
      int neighbor_number = std::stoi(neighbor_number_string);
      if (reached_provinces.contains(neighbor_number))
      {
         continue;
      }
      reached_provinces.insert(neighbor_number);

      if (!hoi4_province_definitions.IsLandProvince(neighbor_number_string))
      {
         continue;
      }

      hoi4::PossiblePath new_possible_railway_path = possible_railway_path;
      new_possible_railway_path.AddProvince(neighbor_number,
          DeterminePathCost(hoi4_province_definitions,
              neighbor_number_string,
              std::to_string(*last_province),
              hoi4_map_data));
      possible_railway_paths.push(new_possible_railway_path);
   }
}


std::optional<hoi4::PossiblePath> FindPath(int start_province,
    int end_province,
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

      const auto last_province = possible_railway_path.GetLastProvince();
      if (!last_province || *last_province == end_province)
      {
         break;
      }

      possible_railway_paths.pop();

      FindNextPaths(possible_railway_path,
          hoi4_map_data,
          hoi4_province_definitions,
          reached_provinces,
          possible_railway_paths);
   }

   if (possible_railway_paths.empty())
   {
      return std::nullopt;
   }
   return possible_railway_paths.top();
}


void BuildPath(int start_province,
    int end_province,
    const int railway_level,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions,
    std::vector<hoi4::PossiblePath>& possible_paths)
{
   auto possible_path = FindPath(start_province, end_province, hoi4_map_data, hoi4_province_definitions);
   if (!possible_path)
   {
      return;
   }

   possible_path->SetLevel(railway_level);
   possible_paths.emplace_back(*possible_path);
}


std::vector<hoi4::PossiblePath> FindAllHoi4Paths(const std::vector<std::pair<int, int>>& hoi4_endpoints,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions)
{
   std::vector<hoi4::PossiblePath> possible_paths;
   for (const auto& [start_point, end_point]: hoi4_endpoints)
   {
      BuildPath(start_point, end_point, 3, hoi4_map_data, hoi4_province_definitions, possible_paths);
   }

   return possible_paths;
}


std::vector<hoi4::Railway> GetRailwaysFromEndpoints(const std::vector<std::pair<int, int>>& endpoints,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions)
{
   std::vector<hoi4::Railway> railways;

   for (const hoi4::PossiblePath& possible_path: FindAllHoi4Paths(endpoints, hoi4_map_data, hoi4_province_definitions))
   {
      hoi4::Railway railway(possible_path.GetLevel(), possible_path.GetProvinces());
      railways.push_back(railway);
   }

   return railways;
}

}  // namespace



hoi4::Railways hoi4::ConvertRailways(const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const mappers::ProvinceMapper& province_mapper,
    const maps::MapData& hoi4_map_data,
    const maps::ProvinceDefinitions& hoi4_province_definitions)
{
   const std::vector<std::pair<std::string, std::string>> vic3_endpoints = DetermineVic3Endpoints(vic3_state_regions);
   const std::vector<std::pair<int, int>> hoi4_endpoints =
       ConvertVic3EndpointsToHoi4Endpoints(vic3_endpoints, province_mapper);
   const std::vector<Railway> railways =
       GetRailwaysFromEndpoints(hoi4_endpoints, hoi4_map_data, hoi4_province_definitions);

   return {railways, {}};
}
#pragma optimize("", on)