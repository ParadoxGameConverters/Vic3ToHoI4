#include "vic3_world_builder.h"

#include <external/fmt/include/fmt/format.h>

namespace vic3
{
WorldBuilder WorldBuilder::CreateNullWorld()
{
   return WorldBuilder();
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

WorldBuilder& WorldBuilder::AddTestStates(const std::vector<std::set<int>>& provinceListLists)
{
   for (const auto& provinceList: provinceListLists)
   {
      const int stateNum = state_number_++;
      this->world_options_.states.emplace(stateNum,
          vic3::State(vic3::StateOptions{.id = stateNum, .owner_number = stateNum, .provinces = provinceList}));
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

WorldBuilder& WorldBuilder::AddTestStateRegions(const std::vector<std::set<int>>& provinceSets)
{
   for (auto& provinceSet: provinceSets)
   {
      std::set<std::string> provinceNames = {};
      for (auto& provinceNum: provinceSet)
      {
         provinceNames.emplace(fmt::format("x0000{:0>2}", provinceNum));
      }

      this->world_options_.state_regions.emplace(fmt::format("REGION_{:0>3}", this->state_region_number_++),
          StateRegion({}, provinceNames));
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