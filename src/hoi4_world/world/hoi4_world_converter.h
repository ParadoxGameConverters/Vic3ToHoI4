#ifndef SRC_HOI4WORLD_WORLD_HOI4WORLDCONVERTER_H
#define SRC_HOI4WORLD_WORLD_HOI4WORLDCONVERTER_H



#include <external/commonItems/Localization/LocalizationDatabase.h>
#include <external/commonItems/ModLoader/ModFilesystem.h>

#include <map>
#include <memory>
#include <string>

#include "src/configuration/configuration.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/hoi4_world/world/hoi4_world_framework.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/world/world_mapper.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

World ConvertWorld(const commonItems::ModFilesystem& hoi4_mod_filesystem,
    const vic3::World& source_world,
    const mappers::WorldMapper& world_mapper,
    WorldFramework& world_framework_future,
    const configuration::Configuration& config = configuration::defaultConfig);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_WORLD_HOI4WORLDCONVERTER_H
