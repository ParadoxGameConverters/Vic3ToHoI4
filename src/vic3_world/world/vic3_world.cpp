#include "src/vic3_world/world/vic3_world.h"

namespace vic3
{

WorldBuilder WorldBuilder::CreateNullWorld()
{
   return WorldBuilder();
}

World WorldBuilder::Build()
{
   this->world_options_.province_definitions = ProvinceDefinitions(province_definitions_);
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

WorldBuilder& WorldBuilder::AddStateRegions(const std::map<std::string, StateRegion>& state_regions)
{
   for (const auto& state_region: state_regions)
   {
      this->world_options_.state_regions.emplace(state_region);
   }
   return *this;
}
}