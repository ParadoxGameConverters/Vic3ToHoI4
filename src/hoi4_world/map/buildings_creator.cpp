#include "src/hoi4_world/map/buildings_creator.h"

#include <fstream>

#include "external/commonItems/Log.h"
#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/map/coastal_provinces.h"
#include "src/hoi4_world/states/hoi4_state.h"



namespace
{

using DefaultPositions = std::map<std::pair<int, int>, hoi4::BuildingPosition>;


struct AllDefaultPositions
{
   DefaultPositions default_arms_factories;
   DefaultPositions default_industrial_complexes;
   DefaultPositions default_air_bases;
   DefaultPositions default_naval_bases;
   DefaultPositions default_bunkers;
   DefaultPositions default_coastal_bunkers;
   DefaultPositions default_dockyards;
   DefaultPositions default_anti_airs;
   DefaultPositions default_synthetic_refineries;
   DefaultPositions default_nuclear_reactors;
   DefaultPositions default_supply_nodes;
   DefaultPositions default_floating_harbors;
};


void ImportDefaultBuilding(const std::smatch& matches, const maps::MapData& map_data, DefaultPositions& positions)
{
   try
   {
      hoi4::BuildingPosition position;
      position.x_coordinate = stof(matches[3].str());
      position.y_coordinate = stof(matches[4].str());
      position.z_coordinate = stof(matches[5].str());
      position.rotation = stof(matches[6].str());

      auto connectingSeaProvince = stoi(matches[7].str());

      if (auto province_name = map_data.GetProvinceName(
              {static_cast<int>(position.x_coordinate), static_cast<int>(position.z_coordinate)});
          province_name)
      {
         int province_number = stoi(*province_name);
         const auto key = std::make_pair(province_number, connectingSeaProvince);
         positions[key] = position;
      }
   }
   catch (...)
   {
      Log(LogLevel::Warning) << "Bad line in /map/buildings.txt";
   }
}


void ProcessLine(const std::string& line, const maps::MapData& map_data, AllDefaultPositions& all_default_positions)
{
   const std::regex pattern("(.+);(.+);(.+);(.+);(.+);(.+);(.+)");
   std::smatch matches;
   if (regex_match(line, matches, pattern))
   {
      if (matches[2] == "air_base")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_air_bases);
      }
      else if (matches[2] == "anti_air_building")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_anti_airs);
      }
      else if (matches[2] == "arms_factory")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_arms_factories);
      }
      else if (matches[2] == "bunker")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_bunkers);
      }
      else if (matches[2] == "coastal_bunker")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_coastal_bunkers);
      }
      else if (matches[2] == "dockyard")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_dockyards);
      }
      else if (matches[2] == "floating_harbor")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_floating_harbors);
      }
      else if (matches[2] == "industrial_complex")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_industrial_complexes);
      }
      else if (matches[2] == "naval_base")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_naval_bases);
      }
      else if (matches[2] == "nuclear_reactor")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_nuclear_reactors);
      }
      else if (matches[2] == "supply_node")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_supply_nodes);
      }
      else if (matches[2] == "synthetic_refinery")
      {
         ImportDefaultBuilding(matches, map_data, all_default_positions.default_synthetic_refineries);
      }
   }
}


AllDefaultPositions ImportDefaultBuildings(const maps::MapData& map_data,
    const commonItems::ModFilesystem& mod_filesystem)
{
   AllDefaultPositions all_default_positions;

   const auto path = mod_filesystem.GetActualFileLocation("/map/buildings.txt");
   if (!path)
   {
      throw std::runtime_error("Could not find /map/buildings.txt");
   }

   std::ifstream buildings_file(*path);
   if (!buildings_file.is_open())
   {
      throw std::runtime_error(fmt::format("Could not open {}/map/buildings.txt", *path));
   }

   while (!buildings_file.eof())
   {
      std::string line;
      getline(buildings_file, line);
      ProcessLine(line, map_data, all_default_positions);
   }

   return all_default_positions;
}


void PlaceBuildingType(const std::vector<hoi4::State>& states,
    const maps::MapData& map_data,
    const DefaultPositions& default_anti_airs,
    std::string_view building_type,
    int number_to_place,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& state: states)
   {
      auto num_placed = 0;

      for (auto theProvince: state.GetProvinces())
      {
         if (auto possible_anti_air = default_anti_airs.find(std::make_pair(theProvince, 0));
             possible_anti_air != default_anti_airs.end())
         {
            int state_id = state.GetId();

            auto position = possible_anti_air->second;
            buildings.emplace_back(hoi4::Building(state_id, std::string(building_type), position));
            num_placed++;

            if (num_placed >= number_to_place)
            {
               break;
            }
         }
      }

      for (auto province: state.GetProvinces())
      {
         if (num_placed >= number_to_place)
         {
            break;
         }

         int state_id = state.GetId();
         if (auto province_points = map_data.GetProvincePoints(std::to_string(province)); province_points)
         {
            const auto centermost_point = province_points->GetCentermostPoint();
            hoi4::BuildingPosition position;
            position.x_coordinate = centermost_point.x;
            position.y_coordinate = 11.0;
            position.z_coordinate = centermost_point.y;
            position.rotation = 0;

            buildings.emplace_back(hoi4::Building(state_id, std::string(building_type), position));
            num_placed++;
         }
         else
         {
            Log(LogLevel::Warning) << fmt::format("Province {} did not have any points. {} not fully set in state {}.",
                province,
                building_type,
                state_id);
            continue;
         }
      }
   }
}


void PlaceAirports(const std::vector<hoi4::State>& states,
    const maps::MapData& map_data,
    const DefaultPositions& default_air_bases,
    std::vector<hoi4::Building>& buildings,
    std::map<int, int>& airport_locations)
{
   PlaceBuildingType(states, map_data, default_air_bases, "air_base", 1, buildings);
   for (const auto& building: buildings)
   {
      if (building.GetType() != "air_base")
      {
         continue;
      }

      const hoi4::BuildingPosition position = building.GetPosition();
      const auto name =
          map_data.GetProvinceName({static_cast<int>(position.x_coordinate), static_cast<int>(position.z_coordinate)});
      if (name)
      {
         try
         {
            airport_locations.emplace(building.GetStateId(), std::stoi(*name));
         }
         catch (...)
         {
         }
      }
   }
}


void AddBunker(int state_id,
    int province,
    const maps::MapData& map_data,
    const DefaultPositions& default_bunkers,
    std::vector<hoi4::Building>& buildings)
{
   hoi4::BuildingPosition position;
   auto position_unset = true;

   if (const auto default_bunker = default_bunkers.find(std::make_pair(province, 0));
       default_bunker != default_bunkers.end())
   {
      position = default_bunker->second;
      position_unset = false;
   }

   if (position_unset)
   {
      auto possible_position = map_data.GetAnyBorderCenter(std::to_string(province));
      if (!possible_position)
      {
         Log(LogLevel::Warning) << fmt::format(
             "Province {} did not have any border points. Bunkers not fully set in state {}.",
             province,
             state_id);
         return;
      }

      position.x_coordinate = possible_position->x;
      position.y_coordinate = 11.0;
      position.z_coordinate = possible_position->y;
      position.rotation = 0.0;

      Log(LogLevel::Warning) << fmt::format("The bunker in {} did not have a location in default HoI4.", province);
   }

   buildings.emplace_back(hoi4::Building(state_id, "bunker", position));
}


void PlaceBunkers(const std::vector<hoi4::State>& states,
    const maps::MapData& map_data,
    const DefaultPositions& default_bunkers,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto state: states)
   {
      for (const auto& province: state.GetProvinces())
      {
         AddBunker(state.GetId(), province, map_data, default_bunkers, buildings);
      }
   }
}


void AddCoastalBunker(int state_id,
    const std::pair<int, std::vector<int>>& province,
    const maps::MapData& map_data,
    const DefaultPositions& default_coastal_bunkers,
    std::vector<hoi4::Building>& buildings)
{
   hoi4::BuildingPosition position;
   auto position_unset = true;

   if (const auto default_bunker = default_coastal_bunkers.find(std::make_pair(province.first, 0));
       default_bunker != default_coastal_bunkers.end())
   {
      position = default_bunker->second;
      position_unset = false;
   }

   if (position_unset)
   {
      const auto possible_position =
          map_data.GetSpecifiedBorderCenter(std::to_string(province.first), std::to_string(province.second[0]));
      if (!possible_position)
      {
         Log(LogLevel::Warning) << fmt::format("Province {} did not have any border points. Coastal bunkers not fully set in state {}.",
             province.first, state_id);
         return;
      }

      position.x_coordinate = possible_position->x;
      position.y_coordinate = 11.0;
      position.z_coordinate = possible_position->y;
      position.rotation = 0.0;

      Log(LogLevel::Warning) << fmt::format(
          "The coastal bunker in {} at ({}, {}) did not have a location in default HoI4.",
          province.first,
          position.x_coordinate,
          position.z_coordinate);
   }

   buildings.emplace_back(hoi4::Building(state_id, "coastal_bunker", position));
}


void PlaceCoastalBunkers(const std::map<int, int>& province_to_state_id_map,
    const std::map<int, std::vector<int>>& actual_coastal_provinces,
    const maps::MapData& map_data,
    const DefaultPositions& default_coastal_bunkers,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& province: actual_coastal_provinces)
   {
      auto province_to_state_mapping = province_to_state_id_map.find(province.first);
      if (province_to_state_mapping == province_to_state_id_map.end())
      {
         Log(LogLevel::Warning) << fmt::format("Could not find state for province {}. Coastal bunker not set.",
             province.first);
         continue;
      }

      AddCoastalBunker(province_to_state_mapping->second, province, map_data, default_coastal_bunkers, buildings);
   }
}


void PlaceDockyards(const std::vector<hoi4::State>& states,
    const hoi4::CoastalProvinces& coastal_provinces,
    std::map<int, std::vector<int>> actual_coastal_provinces,
    const maps::MapData& map_data,
    const DefaultPositions& default_dockyards,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& state: states)
   {
      auto dockyard_placed = false;
      int state_id = state.GetId();

      for (auto province: state.GetProvinces())
      {
         if (auto possible_dockyard = default_dockyards.find(std::make_pair(province, 0));
             possible_dockyard != default_dockyards.end())
         {
            auto position = possible_dockyard->second;
            buildings.emplace_back(hoi4::Building(state_id, "dockyard", position, 0));
            dockyard_placed = true;
            break;
         }
      }

      if (!dockyard_placed)
      {
         std::optional<int> the_province;
         for (auto possible_province: state.GetProvinces())
         {
            if (coastal_provinces.IsProvinceCoastal(possible_province))
            {
               the_province = possible_province;
               break;
            }
         }
         if (the_province)
         {
            auto connecting_sea_provinces = actual_coastal_provinces.find(*the_province);
            if (connecting_sea_provinces != actual_coastal_provinces.end())
            {
               auto centermost_point = map_data.GetSpecifiedBorderCenter(std::to_string(*the_province),
                   std::to_string(connecting_sea_provinces->second[0]));
               if (centermost_point)
               {
                  hoi4::BuildingPosition position;
                  position.x_coordinate = centermost_point->x;
                  position.y_coordinate = 11.0;
                  position.z_coordinate = centermost_point->y;
                  position.rotation = 0;
                  buildings.emplace_back(hoi4::Building(state_id, "dockyard", position, 0));
               }
               else
               {
                  Log(LogLevel::Warning) << fmt::format(
                      "Province {} did not have any points. Dockyard not set in state {}.",
                      *the_province,
                      state_id);
               }
            }
         }
      }
   }
}


void AddFloatingHarbors(int state_id,
    const std::pair<int, std::vector<int>>& province,
    const maps::MapData& map_data,
    const DefaultPositions& default_floating_harbors,
    std::vector<hoi4::Building>& buildings)
{
   hoi4::BuildingPosition position;
   bool position_unset = true;
   int connecting_sea_province = 0;

   for (auto sea_province: province.second)
   {
      // floating harbors are Placed on the sea and "connect" to land tiles,
      // so the adjacent sea province column is filled in with the province ID of the land the floating harbor supports
      // they are also in the state of the connected land tile for buildings.txt purposes
      if (auto default_floating_harbor = default_floating_harbors.find(std::make_pair(sea_province, province.first));
          default_floating_harbor != default_floating_harbors.end())
      {
         position = default_floating_harbor->second;
         connecting_sea_province = province.first;
         position_unset = false;
      }
   }

   if (position_unset)
   {
      connecting_sea_province = province.first;
      auto possible_position =
          map_data.GetSpecifiedBorderCenter(std::to_string(province.second[0]), std::to_string(province.first));
      if (!possible_position)
      {
         Log(LogLevel::Warning) << fmt::format("Could not find position for province {}. Floating Harbor not set.",
             province.first);
         return;
      }

      position.x_coordinate = possible_position->x;
      position.y_coordinate = 11.0;
      position.z_coordinate = possible_position->y;
      position.rotation = 0.0;

      Log(LogLevel::Warning) << fmt::format(
          "The Floating Harbor at province {} at ({}, {}) did not have a location in default HoI4.",
          province.first,
          position.x_coordinate,
          position.z_coordinate);
   }

   buildings.emplace_back(hoi4::Building(state_id, "floating_harbor", position, connecting_sea_province));
}


void PlaceFloatingHarbors(const std::map<int, int>& province_to_state_id_map,
    const std::map<int, std::vector<int>>& actual_coastal_provinces,
    const maps::MapData& map_data,
    const DefaultPositions& default_floating_harbors,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& province: actual_coastal_provinces)
   {
      auto province_to_state_id_mapping = province_to_state_id_map.find(province.first);
      if (province_to_state_id_mapping == province_to_state_id_map.end())
      {
         Log(LogLevel::Warning) << fmt::format("Could not find state for province {}. Floating Harbor not set.",
             province.first);
         continue;
      }

      AddFloatingHarbors(province_to_state_id_mapping->second, province, map_data, default_floating_harbors, buildings);
   }
}


void PlaceIndustrialComplexes(const std::vector<hoi4::State>& states,
    const maps::MapData& map_data,
    const DefaultPositions& default_industrial_complexes,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& state: states)
   {
      int state_id = state.GetId();

      auto num_placed = 0;
      for (auto theProvince: state.GetProvinces())
      {
         if (auto possible_industrial_complex = default_industrial_complexes.find(std::make_pair(theProvince, 0));
             possible_industrial_complex != default_industrial_complexes.end())
         {
            auto position = possible_industrial_complex->second;
            buildings.emplace_back(hoi4::Building(state_id, "industrial_complex", position, 0));
            num_placed++;
         }

         if (num_placed > 3)
         {
            break;
         }
      }

      for (auto province: state.GetProvinces())
      {
         auto province_points = map_data.GetProvincePoints(std::to_string(province));
         if (province_points)
         {
            const auto centermost_point = province_points->GetCentermostPoint();
            hoi4::BuildingPosition position;
            position.x_coordinate = centermost_point.x;
            position.y_coordinate = 11.0;
            position.z_coordinate = centermost_point.y;
            position.rotation = 0;
            buildings.emplace_back(hoi4::Building(state_id, "industrial_complex", position, 0));
            num_placed++;
         }
         else
         {
            Log(LogLevel::Warning) << fmt::format(
                "Province {} did not have any points. Industrial complexes not fully set in state {}.",
                province,
                state_id);
            break;
         }

         if (num_placed >= 6)
         {
            break;
         }
      }
   }
}


void AddNavalBase(int state_id,
    const std::pair<int, std::vector<int>>& province,
    const maps::MapData& map_data,
    const DefaultPositions& default_naval_bases,
    std::vector<hoi4::Building>& buildings)
{
   hoi4::BuildingPosition position;
   auto position_unset = true;
   auto connecting_sea_province = 0;

   for (auto sea_province: province.second)
   {
      if (auto default_naval_base = default_naval_bases.find(std::make_pair(province.first, sea_province));
          default_naval_base != default_naval_bases.end())
      {
         position = default_naval_base->second;
         connecting_sea_province = sea_province;
         position_unset = false;
      }
   }

   if (position_unset)
   {
      connecting_sea_province = province.second[0];
      auto possible_position =
          map_data.GetSpecifiedBorderCenter(std::to_string(province.first), std::to_string(province.second[0]));
      if (!possible_position)
      {
         Log(LogLevel::Warning) << "Could not find position for province " << province.first << ". Naval base not set.";
         return;
      }

      position.x_coordinate = possible_position->x;
      position.y_coordinate = 11.0;
      position.z_coordinate = possible_position->y;
      position.rotation = 0.0;

      Log(LogLevel::Warning) << fmt::format(
          "The naval base from {} to {} at ({}, {}) did not have a location in default HoI4.",
          province.first,
          connecting_sea_province,
          position.x_coordinate,
          position.z_coordinate);
   }

   buildings.emplace_back(hoi4::Building(state_id, "naval_base", position, connecting_sea_province));
}


void PlaceNavalBases(const std::map<int, int>& province_to_state_id_map,
    const std::map<int, std::vector<int>>& actual_coastal_provinces,
    const maps::MapData& map_data,
    const DefaultPositions& default_naval_bases,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& province: actual_coastal_provinces)
   {
      auto province_to_state_mapping = province_to_state_id_map.find(province.first);
      if (province_to_state_mapping == province_to_state_id_map.end())
      {
         Log(LogLevel::Warning) << fmt::format("Could not find state for province {}. Naval base not set.",
             province.first);
         continue;
      }

      AddNavalBase(province_to_state_mapping->second, province, map_data, default_naval_bases, buildings);
   }
}


void PlaceNuclearReactors(const std::vector<hoi4::State>& states,
    const maps::MapData& map_data,
    const DefaultPositions& default_nuclear_reactors,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& state: states)
   {
      if (state.GetProvinces().empty())
      {
         continue;
      }

      int state_id = state.GetId();
      auto reactor_placed = false;

      for (auto province: state.GetProvinces())
      {
         if (auto possible_reactor = default_nuclear_reactors.find(std::make_pair(province, 0));
             possible_reactor != default_nuclear_reactors.end())
         {
            auto position = possible_reactor->second;
            buildings.emplace_back(hoi4::Building(state_id, "nuclear_reactor", position, 0));
            reactor_placed = true;
            break;
         }
      }
      if (!reactor_placed)
      {
         const auto first_province = *state.GetProvinces().begin();
         auto province_points = map_data.GetProvincePoints(std::to_string(first_province));
         if (province_points)
         {
            const auto centermost_point = province_points->GetCentermostPoint();
            hoi4::BuildingPosition position;
            position.x_coordinate = centermost_point.x;
            position.y_coordinate = 11.0;
            position.z_coordinate = centermost_point.y;
            position.rotation = 0;
            buildings.emplace_back(hoi4::Building(state_id, "nuclear_reactor", position, 0));
         }
         else
         {
            Log(LogLevel::Warning) << fmt::format(
                "Province {} did not have any points. Nuclear reactor not set for state {}.",
                first_province,
                state_id);
         }
      }
   }
}


void AddSupplyNodes(int state_id,
    int province,
    const maps::MapData& map_data,
    const DefaultPositions& default_supply_nodes,
    std::vector<hoi4::Building>& buildings)
{
   hoi4::BuildingPosition position;
   auto position_unset = true;

   const auto default_supply_node = default_supply_nodes.find(std::make_pair(province, 0));
   if (default_supply_node != default_supply_nodes.end())
   {
      position = default_supply_node->second;
      position_unset = false;
   }

   if (position_unset)
   {
      auto possible_position = map_data.GetAnyBorderCenter(std::to_string(province));
      if (!possible_position)
      {
         Log(LogLevel::Warning) << fmt::format("Could not find position for province {}. Supply Node not set.",
             province);
         return;
      }

      position.x_coordinate = possible_position->x;
      position.y_coordinate = 11.0;
      position.z_coordinate = possible_position->y;
      position.rotation = 0.0;

      Log(LogLevel::Warning) << fmt::format(
          "The Supply Node in {} at ({}, {}) did not have a location in default HoI4.",
          province,
          position.x_coordinate,
          position.z_coordinate);
   }

   buildings.emplace_back(hoi4::Building(state_id, "supply_node", position, 0));
}


void PlaceSupplyNodes(const std::map<int, int>& province_to_state_id_map,
    const maps::MapData& map_data,
    const DefaultPositions& default_supply_nodes,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& province_and_state_id: province_to_state_id_map)
   {
      AddSupplyNodes(province_and_state_id.second,
          province_and_state_id.first,
          map_data,
          default_supply_nodes,
          buildings);
   }
}


void PlaceSyntheticRefineries(const std::vector<hoi4::State>& states,
    const maps::MapData& map_data,
    const DefaultPositions& default_synthetic_refineries_nodes,
    std::vector<hoi4::Building>& buildings)
{
   for (const auto& state: states)
   {
      if (state.GetProvinces().empty())
      {
         continue;
      }

      int state_id = state.GetId();
      auto refinery_placed = false;

      for (auto province: state.GetProvinces())
      {
         if (auto possible_refinery = default_synthetic_refineries_nodes.find(std::make_pair(province, 0));
             possible_refinery != default_synthetic_refineries_nodes.end())
         {
            auto position = possible_refinery->second;
            buildings.emplace_back(hoi4::Building(state_id, "synthetic_refinery", position, 0));
            refinery_placed = true;
            break;
         }
      }
      if (!refinery_placed)
      {
         const auto first_province = *state.GetProvinces().begin();
         auto province_points = map_data.GetProvincePoints(std::to_string(first_province));
         if (province_points)
         {
            const auto centermost_point = province_points->GetCentermostPoint();
            hoi4::BuildingPosition position;
            position.x_coordinate = centermost_point.x;
            position.y_coordinate = 11.0;
            position.z_coordinate = centermost_point.y;
            position.rotation = 0;
            buildings.emplace_back(hoi4::Building(state_id, "synthetic_refinery", position, 0));
         }
         else
         {
            Log(LogLevel::Warning) << fmt::format(
                "Province {} did not have any points. Synthetic refinery not set for state {}.",
                first_province,
                state_id);
         }
      }
   }
}


hoi4::Buildings PlaceBuildings(const hoi4::States& states,
    const hoi4::CoastalProvinces& coastal_provinces,
    const maps::MapData& map_data,
    const AllDefaultPositions& all_default_positions)
{
   std::vector<hoi4::Building> buildings;
   std::map<int, int> airport_locations;

   const auto& actual_coastal_provinces = coastal_provinces.GetCoastalProvinces();

   PlaceAirports(states.states, map_data, all_default_positions.default_air_bases, buildings, airport_locations);
   PlaceBuildingType(states.states,
       map_data,
       all_default_positions.default_anti_airs,
       "anti_air_building",
       3,
       buildings);
   PlaceBuildingType(states.states,
       map_data,
       all_default_positions.default_arms_factories,
       "arms_factory",
       6,
       buildings);
   PlaceBunkers(states.states, map_data, all_default_positions.default_bunkers, buildings);
   PlaceCoastalBunkers(states.province_to_state_id_map,
       actual_coastal_provinces,
       map_data,
       all_default_positions.default_coastal_bunkers,
       buildings);
   PlaceDockyards(states.states,
       coastal_provinces,
       actual_coastal_provinces,
       map_data,
       all_default_positions.default_dockyards,
       buildings);
   PlaceFloatingHarbors(states.province_to_state_id_map,
       actual_coastal_provinces,
       map_data,
       all_default_positions.default_floating_harbors,
       buildings);
   PlaceIndustrialComplexes(states.states, map_data, all_default_positions.default_industrial_complexes, buildings);
   PlaceNavalBases(states.province_to_state_id_map,
       actual_coastal_provinces,
       map_data,
       all_default_positions.default_naval_bases,
       buildings);
   PlaceNuclearReactors(states.states, map_data, all_default_positions.default_nuclear_reactors, buildings);
   PlaceSupplyNodes(states.province_to_state_id_map, map_data, all_default_positions.default_supply_nodes, buildings);
   PlaceSyntheticRefineries(states.states, map_data, all_default_positions.default_synthetic_refineries, buildings);

   return {buildings, airport_locations};
}

}  // namespace


hoi4::Buildings hoi4::ImportBuildings(const States& states,
    const CoastalProvinces& coastal_provinces,
    const maps::MapData& map_data,
    const commonItems::ModFilesystem& mod_filesystem)
{
   Log(LogLevel::Info) << "\tCreating buildings";

   const auto all_default_buildings = ImportDefaultBuildings(map_data, mod_filesystem);
   return PlaceBuildings(states, coastal_provinces, map_data, all_default_buildings);
}