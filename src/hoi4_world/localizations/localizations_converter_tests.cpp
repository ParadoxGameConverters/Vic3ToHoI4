#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/localizations/localizations_converter.h"



namespace hoi4
{

TEST(Hoi4worldLocalizationsLocalizationsconverter, NoCountryMappingsNoCountryLocalizations)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("VIC", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("VIC", block_one);
   commonItems::LocalizationBlock block_two("TWO", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("TWO", block_two);


   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations, {}, {});

   EXPECT_EQ(hoi4_localizations.GetCountryLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, NoVic3LocalizationsNoHoi4CountryLocalizations)
{
   const commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});

   const Localizations hoi4_localizations =
       ConvertLocalizations(vic3_localizations, {{"VIC", "HOI"}, {"TWO", "TWO"}}, {});

   EXPECT_EQ(hoi4_localizations.GetCountryLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, CountryLocalizationsCanBeConverted)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("VIC", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("VIC", block_one);
   commonItems::LocalizationBlock block_one_adj("VIC_ADJ", "english");
   block_one_adj.ModifyLocalization("english", "testy");
   block_one_adj.ModifyLocalization("spanish", "pruebamente");
   vic3_localizations.AddOrModifyLocalizationBlock("VIC_ADJ", block_one_adj);
   commonItems::LocalizationBlock block_two("TWO", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("TWO", block_two);
   commonItems::LocalizationBlock block_two_adj("TWO_ADJ", "english");
   block_two_adj.ModifyLocalization("english", "testy two");
   block_two_adj.ModifyLocalization("spanish", "pruebamente dos");
   vic3_localizations.AddOrModifyLocalizationBlock("TWO_ADJ", block_two_adj);


   const Localizations hoi4_localizations =
       ConvertLocalizations(vic3_localizations, {{"VIC", "HOI"}, {"TWO", "TWO"}}, {});

   const std::optional<commonItems::LocalizationBlock> hoi_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("HOI");
   ASSERT_TRUE(hoi_localization_block.has_value());
   EXPECT_THAT(hoi_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("HOI_DEF");
   ASSERT_TRUE(hoi_def_localization_block.has_value());
   EXPECT_THAT(hoi_def_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("HOI_ADJ");
   ASSERT_TRUE(hoi_adj_localization_block.has_value());
   EXPECT_THAT(hoi_adj_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "testy"), testing::Pair("spanish", "pruebamente")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO");
   ASSERT_TRUE(two_localization_block.has_value());
   EXPECT_THAT(two_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));

   const std::optional<commonItems::LocalizationBlock> two_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO_DEF");
   ASSERT_TRUE(two_def_localization_block.has_value());
   EXPECT_THAT(two_def_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));

   const std::optional<commonItems::LocalizationBlock> two_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO_ADJ");
   ASSERT_TRUE(two_adj_localization_block.has_value());
   EXPECT_THAT(two_adj_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "testy two"),
           testing::Pair("spanish", "pruebamente dos")));
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, NoStateNameMappingsNoStateLocalizations)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("VIC", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("VIC", block_one);
   commonItems::LocalizationBlock block_two("TWO", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("TWO", block_two);


   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations, {}, {});

   EXPECT_EQ(hoi4_localizations.GetStateLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, NoVic3LocalizationsNoHoi4StateLocalizations)
{
   const commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});

   const Localizations hoi4_localizations =
       ConvertLocalizations(vic3_localizations, {}, {{"STATE_ONE", "REGION_ONE"}, {"STATE_TWO", "REGION_TWO"}});

   EXPECT_EQ(hoi4_localizations.GetStateLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, StateLocalizationsCanBeConverted)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("REGION_ONE", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("REGION_ONE", block_one);
   commonItems::LocalizationBlock block_two("REGION_TWO", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("REGION_TWO", block_two);

   const Localizations hoi4_localizations =
       ConvertLocalizations(vic3_localizations, {}, {{"STATE_ONE", "REGION_ONE"}, {"STATE_TWO", "REGION_TWO"}});

   const std::optional<commonItems::LocalizationBlock> one_localization_block =
       hoi4_localizations.GetStateLocalizations().GetLocalizationBlock("STATE_ONE");
   ASSERT_TRUE(one_localization_block.has_value());
   EXPECT_THAT(one_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetStateLocalizations().GetLocalizationBlock("STATE_TWO");
   ASSERT_TRUE(two_localization_block.has_value());
   EXPECT_THAT(two_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));
}

}  // namespace hoi4