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


   const Localizations hoi4_localizations =
       ConvertLocalizations(vic3_localizations, {}, {}, {}, mappers::ProvinceMapper({}, {}), {}, {});

   EXPECT_EQ(hoi4_localizations.GetCountryLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, NoVic3LocalizationsNoHoi4CountryLocalizations)
{
   const commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});

   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {
           {1, "HOI"},
           {2, "TWO"},
       },
       {},
       {},
       mappers::ProvinceMapper({}, {}),
       {},
       {});

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


   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {
           {1, "HOI"},
           {2, "TWO"},
       },
       {},
       {},
       mappers::ProvinceMapper({}, {}),
       {
           {1, vic3::Country({.tag = "VIC"})},
           {2, vic3::Country({.tag = "TWO"})},
       },
       {});

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


   const Localizations hoi4_localizations =
       ConvertLocalizations(vic3_localizations, {}, {}, {}, mappers::ProvinceMapper({}, {}), {}, {});

   EXPECT_EQ(hoi4_localizations.GetStateLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, NoVic3LocalizationsNoHoi4StateLocalizations)
{
   const commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});

   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {},
       {
           {"STATE_ONE", "REGION_ONE"},
           {"STATE_TWO", "REGION_TWO"},
       },
       {},
       mappers::ProvinceMapper({}, {}),
       {},
       {});

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

   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {},
       {
           {"STATE_ONE", "REGION_ONE"},
           {"STATE_TWO", "REGION_TWO"},
       },
       {},
       mappers::ProvinceMapper({}, {}),
       {},
       {});

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


TEST(Hoi4worldLocalizationsLocalizationsconverter, NoRegionsNoVictoryPointLocalizations)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("HUB_NAME_REGION_ONE_city", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_ONE_city", block_one);
   commonItems::LocalizationBlock block_two("HUB_NAME_REGION_ONE_mine", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_ONE_mine", block_two);

   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {},
       {},
       {},
       mappers::ProvinceMapper(
           {
               {"x123456", {1}},
               {"x2468AC", {2}},
           },
           {}),
       {},
       {});

   EXPECT_EQ(hoi4_localizations.GetVictoryPointLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, NoSignificantProvincesNoVictoryPointLocalizations)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("HUB_NAME_REGION_ONE_city", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_ONE_city", block_one);
   commonItems::LocalizationBlock block_two("HUB_NAME_REGION_ONE_mine", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_ONE_mine", block_two);

   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {},
       {},
       {
           {"REGION_ONE", vic3::StateRegion({}, {})},
       },
       mappers::ProvinceMapper(
           {
               {"x123456", {1}},
               {"x2468AC", {2}},
           },
           {}),
       {},
       {});

   EXPECT_EQ(hoi4_localizations.GetVictoryPointLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, VictoryPointLocalizationsCanBeConverted)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("HUB_NAME_REGION_ONE_city", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_ONE_city", block_one);
   commonItems::LocalizationBlock block_two("HUB_NAME_REGION_ONE_mine", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_ONE_mine", block_two);

   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {},
       {},
       {
           {
               "REGION_ONE",
               vic3::StateRegion({{"x123456", "city"}, {"x2468AC", "mine"}}, {}),
           },
       },
       mappers::ProvinceMapper(
           {
               {"x123456", {1}},
               {"x2468AC", {2}},
           },
           {}),
       {},
       {});

   const std::optional<commonItems::LocalizationBlock> one_localization_block =
       hoi4_localizations.GetVictoryPointLocalizations().GetLocalizationBlock("VICTORY_POINTS_1");
   ASSERT_TRUE(one_localization_block.has_value());
   EXPECT_THAT(one_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetVictoryPointLocalizations().GetLocalizationBlock("VICTORY_POINTS_2");
   ASSERT_TRUE(two_localization_block.has_value());
   EXPECT_THAT(two_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, SkippedVictoryPointLocalizationsAreLogged)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("HUB_NAME_REGION_ONE_city", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_ONE_city", block_one);

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {},
       {},
       {
           {"REGION_ONE", vic3::StateRegion({{"x123456", "city"}, {"x2468AC", "mine"}}, {})},
       },
       mappers::ProvinceMapper({{"x2468AC", {2}}}, {}),
       {},
       {});

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] 2 VP localizations were skipped."));

   EXPECT_EQ(hoi4_localizations.GetVictoryPointLocalizations().size(), 0);
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, CharacterLocalizationsCanBeConverted)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("Doug", "english");
   block_one.ModifyLocalization("english", "Douggie");
   block_one.ModifyLocalization("spanish", "Dougesa");
   vic3_localizations.AddOrModifyLocalizationBlock("Doug", block_one);
   commonItems::LocalizationBlock block_two("Romney1", "english");
   block_two.ModifyLocalization("english", "Romney");
   block_two.ModifyLocalization("spanish", "Romney");
   vic3_localizations.AddOrModifyLocalizationBlock("Romney1", block_two);


   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {},
       {},
       {},
       mappers::ProvinceMapper({}, {}),
       {},
       {
           {1, vic3::Character({.first_name = "Doug", .last_name = "Romney1"})},
       });


   const std::optional<commonItems::LocalizationBlock> one_localization_block =
       hoi4_localizations.GetCharacterLocalizations().GetLocalizationBlock("Doug");
   ASSERT_TRUE(one_localization_block.has_value());
   EXPECT_THAT(one_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Douggie"), testing::Pair("spanish", "Dougesa")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetCharacterLocalizations().GetLocalizationBlock("Romney1");
   ASSERT_TRUE(two_localization_block.has_value());
   EXPECT_THAT(two_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Romney"), testing::Pair("spanish", "Romney")));

   const std::optional<commonItems::LocalizationBlock> three_localization_block =
       hoi4_localizations.GetCharacterLocalizations().GetLocalizationBlock("Doug_Romney1");
   ASSERT_TRUE(three_localization_block.has_value());
   EXPECT_THAT(three_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "$Doug$ $Romney1$"),
           testing::Pair("spanish", "$Doug$ $Romney1$")));
}

}  // namespace hoi4