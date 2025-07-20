#include "vic3_world_builder.h"

#include <external/fmt/include/fmt/format.h>



namespace vic3
{

WorldBuilder WorldBuilder::CreateNullWorld()
{
   return {};
}

World WorldBuilder::Build()
{
   this->world_options_.province_definitions = ProvinceDefinitions(province_definitions_);
   this->world_options_.buildings = vic3::Buildings(buildings_);
   return World(world_options_);
}

WorldBuilder& WorldBuilder::AddProvinces(const std::vector<std::string>& provinces)
{
   for (const auto& province: provinces)
   {
      this->province_definitions_.emplace_back(province);
   }
   return *this;
}

WorldBuilder& WorldBuilder::AddStates(const std::map<int, State>& states)
{
   for (const auto& state: states)
   {
      this->world_options_.states.emplace(state);
   }
   return *this;
}

WorldBuilder& WorldBuilder::AddTestStates(const std::vector<std::set<int>>& province_list_lists)
{
   for (const auto& province_list: province_list_lists)
   {
      const int state_num = state_number_++;
      this->world_options_.states.emplace(state_num,
          vic3::State(vic3::StateOptions{.id = state_num, .owner_number = state_num, .provinces = province_list}));
   }
   return *this;
}

WorldBuilder& WorldBuilder::AddStateRegions(const std::map<std::string, StateRegion>& state_regions)
{
   for (const auto& state_region: state_regions)
   {
      this->world_options_.state_regions.emplace(state_region);
   }
   return *this;
}

WorldBuilder& WorldBuilder::AddTestStateRegions(const std::vector<std::set<int>>& province_sets)
{
   for (auto& province_set: province_sets)
   {
      std::set<std::string> province_names = {};
      for (auto& province_num: province_set)
      {
         province_names.emplace(fmt::format("x0000{:0>2}", province_num));
      }

      this->world_options_.state_regions.emplace(fmt::format("REGION_{:0>3}", this->state_region_number_++),
          StateRegion({}, province_names));
   }
   return *this;
}

WorldBuilder& WorldBuilder::AddBuildings(const std::vector<vic3::Building>& buildings)
{
   for (auto& building: buildings)
   {
      this->buildings_[building.GetStateNumber().value()].emplace_back(building);
   }
   return *this;
}

}  // namespace vic3