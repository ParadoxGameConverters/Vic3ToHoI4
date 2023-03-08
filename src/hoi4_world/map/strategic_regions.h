#ifndef SRC_HOI4WORLD_MAP_STRATEGICREGIONS_H
#define SRC_HOI4WORLD_MAP_STRATEGICREGIONS_H



#include <map>

#include "src/configuration/configuration.h"
#include "src/hoi4_world/map/strategic_region.h"
#include "src/hoi4_world/states/hoi4_state.h"



namespace hoi4
{

struct StrategicRegionsOptions
{
   std::map<int, StrategicRegion> strategic_regions;
   std::map<int, int> province_to_strategic_region_map;
};

class StrategicRegions
{
  public:
   StrategicRegions() = default;
   explicit StrategicRegions(StrategicRegionsOptions options):
       strategic_regions_(std::move(options.strategic_regions)),
       province_to_strategic_region_map_(std::move(options.province_to_strategic_region_map))
   {
   }

   [[nodiscard]] const std::map<int, StrategicRegion>& GetStrategicRegions() const { return strategic_regions_; }
   [[nodiscard]] const std::map<int, int>& GetProvinceToStrategicRegionMap() const
   {
      return province_to_strategic_region_map_;
   }

   void UpdateToMatchNewStates(const std::vector<State>& states);

  private:
   std::map<int, StrategicRegion> strategic_regions_;
   std::map<int, int> province_to_strategic_region_map_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_MAP_STRATEGICREGIONS_H