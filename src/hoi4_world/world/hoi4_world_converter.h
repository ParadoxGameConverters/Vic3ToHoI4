#ifndef SRC_HOI4WORLD_WORLD_HOI4WORLDCONVERTER_H
#define SRC_HOI4WORLD_WORLD_HOI4WORLDCONVERTER_H



#include <map>
#include <memory>
#include <string>

#include "external/commonItems/Localization/LocalizationDatabase.h"
#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

World ConvertWorld(const commonItems::ModFilesystem& hoi4_mod_filesystem,
    const vic3::World& source_world,
    const mappers::CountryMapper& country_mapper,
    const mappers::ProvinceMapper& province_mapper);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_WORLD_HOI4WORLDCONVERTER_H