#ifndef SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H
#define SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H



#include <map>

#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/states/hoi4_states.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapping_types.h"
#include "src/maps/map_data.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/states/vic3_state.h"



namespace hoi4
{

class StatesConverter
{
  public:
   States ConvertStates(const std::map<int, vic3::State>& states,
       const vic3::ProvinceDefinitions& vic3_province_definitions,
       const mappers::Hoi4ToVic3ProvinceMapping& hoi4_to_vic3_province_mappings,
       const maps::MapData& map_data,
       const maps::ProvinceDefinitions& hoi4_province_definitions,
       const hoi4::StrategicRegions& strategic_regions,
       const mappers::CountryMapper& country_mapper);

  private:
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_STATES_HOI4STATESCONVERTER_H