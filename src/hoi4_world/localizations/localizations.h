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
       state_localizations_(commonItems::LocalizationDatabase("english", {})),
       victory_point_localizations_(commonItems::LocalizationDatabase("english", {})),
       character_localizations_(commonItems::LocalizationDatabase("english", {})),
       idea_localizations_(commonItems::LocalizationDatabase("english", {}))
   {
   }
   Localizations(commonItems::LocalizationDatabase country_localizations,
       commonItems::LocalizationDatabase state_localizations,
       commonItems::LocalizationDatabase victory_point_localizations,
       commonItems::LocalizationDatabase character_localizations,
       commonItems::LocalizationDatabase idea_localizations):
       country_localizations_(std::move(country_localizations)),
       state_localizations_(std::move(state_localizations)),
       victory_point_localizations_(std::move(victory_point_localizations)),
       character_localizations_(std::move(character_localizations)),
       idea_localizations_(std::move(idea_localizations))
   {
   }

   [[nodiscard]] const commonItems::LocalizationDatabase& GetCountryLocalizations() const
   {
      return country_localizations_;
   }
   [[nodiscard]] const commonItems::LocalizationDatabase& GetStateLocalizations() const { return state_localizations_; }
   [[nodiscard]] const commonItems::LocalizationDatabase& GetVictoryPointLocalizations() const
   {
      return victory_point_localizations_;
   }
   [[nodiscard]] const commonItems::LocalizationDatabase& GetCharacterLocalizations() const
   {
      return character_localizations_;
   }
   [[nodiscard]] const commonItems::LocalizationDatabase& GetIdeaLocalizations() const { return idea_localizations_; }

  private:
   commonItems::LocalizationDatabase country_localizations_;
   commonItems::LocalizationDatabase state_localizations_;
   commonItems::LocalizationDatabase victory_point_localizations_;
   commonItems::LocalizationDatabase character_localizations_;
   commonItems::LocalizationDatabase idea_localizations_;
};

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_LOCALIZATIONS_H