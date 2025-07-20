#pragma once
#include "src/vic3_world/world/vic3_world.h"

namespace vic3
{

class WorldBuilder
{
  public:
   static WorldBuilder CreateNullWorld();
   World Build();
   WorldBuilder& AddProvinces(const std::vector<std::string>& provinces);
   WorldBuilder& AddStates(const std::map<int, State>& states);
   /// <summary>
   /// Add test states. Each state has a separate owner (state num == state owner, starting at 1) and the provinces in
   /// the list. equivalent to {auto_increasing_int, {.owner = auto_increasing_int, .regions = {regionList}}
   /// </summary>
   WorldBuilder& AddTestStates(const std::vector<std::set<int>>& province_list_lists);
   WorldBuilder& AddStateRegions(const std::map<std::string, StateRegion>& state_regions);
   /// <summary>
   /// Adds state regions with the specified provinces in the state region. Starts at REGION_001, and includes provinces
   /// with the names "x0000nn" for the n's specified in the list. Contains Zero significant provinces.
   /// </summary>
   WorldBuilder& AddTestStateRegions(const std::vector<std::set<int>>& province_sets);
   // Add buildings. Element format is {building,...}
   WorldBuilder& AddBuildings(const std::vector<vic3::Building>& buildings);

  private:
   WorldBuilder() = default;
   WorldOptions world_options_ = {};
   std::vector<std::string> province_definitions_ = {};
   int state_region_number_ = 1;
   int state_number_ = 1;
   std::map<int, std::vector<vic3::Building>> buildings_ = {};
};
}  // namespace vic3