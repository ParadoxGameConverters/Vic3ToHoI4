#ifndef SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H
#define SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H



#include <map>

#include "src/hoi4_world/map/hoi4_province.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/mappers/provinces/province_mapping_types.h"
#include "src/maps/map_data.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/states/vic3_state.h"



namespace hoi4
{

class StatesConverter
{
  public:
   std::vector<State> ConvertStates(const std::map<int, vic3::State>& states,
       const vic3::ProvinceDefinitions& vic3_province_definitions,
       const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
       const maps::MapData& map_data,
       const std::map<int, hoi4::Province>& provinces,
       const hoi4::StrategicRegions& strategic_regions);

  private:
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H