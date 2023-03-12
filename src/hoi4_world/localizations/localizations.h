#ifndef SRC_HOI4WORLD_LOCALIZATIONS_H
#define SRC_HOI4WORLD_LOCALIZATIONS_H



#include "external/commonItems/Localization/LocalizationDatabase.h"



namespace hoi4
{

class Localizations
{
  public:
   Localizations():
       country_localizations_(commonItems::LocalizationDatabase("english", {})),
       state_localizations_(commonItems::LocalizationDatabase("english", {}))
   {
   }
   Localizations(commonItems::LocalizationDatabase country_localizations,
       commonItems::LocalizationDatabase state_localizations):
       country_localizations_(std::move(country_localizations)),
       state_localizations_(std::move(state_localizations))
   {
   }

   [[nodiscard]] const commonItems::LocalizationDatabase& GetCountryLocalizations() const
   {
      return country_localizations_;
   }
   [[nodiscard]] const commonItems::LocalizationDatabase& GetStateLocalizations() const { return state_localizations_; }

  private:
   commonItems::LocalizationDatabase country_localizations_;
   commonItems::LocalizationDatabase state_localizations_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_LOCALIZATIONS_H