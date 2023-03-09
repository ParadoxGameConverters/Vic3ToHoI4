#ifndef SRC_HOI4WORLD_LOCALIZATIONS_H
#define SRC_HOI4WORLD_LOCALIZATIONS_H



#include "external/commonItems/Localization/LocalizationDatabase.h"



namespace hoi4
{

class Localizations
{
  public:
   Localizations(): country_localizations_(commonItems::LocalizationDatabase("english", {})) {}
   Localizations(commonItems::LocalizationDatabase country_localizations):
       country_localizations_(std::move(country_localizations))
   {
   }

   [[nodiscard]] const commonItems::LocalizationDatabase& GetCountryLocalizations() const
   {
      return country_localizations_;
   }

  private:
   commonItems::LocalizationDatabase country_localizations_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_LOCALIZATIONS_H