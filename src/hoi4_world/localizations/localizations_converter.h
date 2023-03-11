#ifndef SRC_HOI4WORLD_LOCALIZATIONS_LOCALIZATIONSCONVERTER_H
#define SRC_HOI4WORLD_LOCALIZATIONS_LOCALIZATIONSCONVERTER_H



#include "external/commonItems/Localization/LocalizationDatabase.h"
#include "src/hoi4_world/localizations/localizations.h"
#include "src/mappers/country/country_mapper.h"



namespace hoi4
{

Localizations ConvertLocalizations(const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<std::string, std::string>& country_mappings);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_LOCALIZATIONS_LOCALIZATIONSCONVERTER_H