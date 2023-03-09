#include "src/hoi4_world/localizations/localizations_converter.h"

#include "external/commonItems/Localization/LocalizationBlock.h"
#include "external/commonItems/Localization/LocalizationDatabase.h"



hoi4::Localizations hoi4::ConvertLocalizations()
{
   commonItems::LocalizationDatabase test_localizations("english", {});
   commonItems::LocalizationBlock test_block("test_key", "english");
   test_block.ModifyLocalization("english", "test_localization");
   test_block.ModifyLocalization("spanish", "test_esp_localization");
   test_localizations.AddOrModifyLocalizationBlock(test_block.GetKey(), test_block);

   return {test_localizations};
}