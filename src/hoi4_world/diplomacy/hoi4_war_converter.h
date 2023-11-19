#ifndef HOI4WORLD_DIPLOMACY_HOI4WARCONVERTER_H
#define HOI4WORLD_DIPLOMACY_HOI4WARCONVERTER_H


#include "src/hoi4_world/diplomacy/hoi4_war.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/vic3_world/wars/war.h"



namespace hoi4
{

std::optional<War> ConvertWar(const vic3::War& source_war,
    const std::set<std::string>& independent_countries,
    const mappers::CountryMapper& country_mapper);

}  // namespace hoi4



#endif  // HOI4WORLD_DIPLOMACY_HOI4WARCONVERTER_H