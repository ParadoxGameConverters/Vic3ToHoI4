#ifndef SRC_HOI4WORLD_LOCALIZATIONS_LOCALIZATIONSCONVERTER_H
#define SRC_HOI4WORLD_LOCALIZATIONS_LOCALIZATIONSCONVERTER_H



#include "external/commonItems/Localization/LocalizationDatabase.h"
#include "src/hoi4_world/localizations/localizations.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/states/state_region.h"



namespace hoi4
{

Localizations ConvertLocalizations(const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<int, std::string>& country_mappings,
    const std::map<std::string, std::string>& hoi4_state_names_to_vic3_state_names,
    const std::map<std::string, vic3::StateRegion>& vic3_state_regions,
    const mappers::ProvinceMapper& province_mapper,
    const std::map<int, vic3::Country>& vic3_countries,
    const std::map<int, vic3::Character>& vic3_characters);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_LOCALIZATIONS_LOCALIZATIONSCONVERTER_H