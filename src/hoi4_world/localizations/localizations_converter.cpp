#include "src/hoi4_world/localizations/localizations_converter.h"

#include "external/commonItems/Localization/LocalizationBlock.h"
#include "external/commonItems/Localization/LocalizationDatabase.h"
#include "external/fmt/include/fmt/format.h"



hoi4::Localizations hoi4::ConvertLocalizations(const commonItems::LocalizationDatabase& vic3_localizations,
    const std::map<std::string, std::string>& country_mappings)
{
   commonItems::LocalizationDatabase country_localizations("english",
       {"braz_por", "french", "german", "japanese", "polish", "russian", "spanish"});

   for (const auto& [vic3_country, hoi4_country]: country_mappings)
   {
      const auto& country_localization_block = vic3_localizations.GetLocalizationBlock(vic3_country);
      if (!country_localization_block)
      {
         continue;
      }

      country_localizations.AddOrModifyLocalizationBlock(hoi4_country, *country_localization_block);
      country_localizations.AddOrModifyLocalizationBlock(fmt::format("{}_DEF", hoi4_country),
          *country_localization_block);

      const auto& adjective_localization_block =
          vic3_localizations.GetLocalizationBlock(fmt::format("{}_ADJ", vic3_country));
      if (!adjective_localization_block)
      {
         continue;
      }
      country_localizations.AddOrModifyLocalizationBlock(fmt::format("{}_ADJ", hoi4_country),
          *adjective_localization_block);
   }

   return {country_localizations};
}