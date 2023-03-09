#ifndef SRC_HOI4WORLD_LOCALIZATIONS_H
#define SRC_HOI4WORLD_LOCALIZATIONS_H



#include "external/commonItems/Localization/LocalizationDatabase.h"



namespace hoi4
{

class Localizations
{
  public:
   Localizations(): test_localizations_(commonItems::LocalizationDatabase("english", {})) {}
   Localizations(commonItems::LocalizationDatabase test_localizations):
       test_localizations_(std::move(test_localizations))
   {
   }

   [[nodiscard]] const commonItems::LocalizationDatabase& GetTestLocalizations() const { return test_localizations_; }

  private:
   commonItems::LocalizationDatabase test_localizations_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_LOCALIZATIONS_H