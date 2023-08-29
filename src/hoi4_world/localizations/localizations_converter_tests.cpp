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
   EXPECT_THAT(hoi_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("HOI_DEF");
   EXPECT_THAT(hoi_def_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("HOI_ADJ");
   EXPECT_THAT(hoi_adj_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "testy"), testing::Pair("spanish", "pruebamente")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO");
   EXPECT_THAT(two_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));

   const std::optional<commonItems::LocalizationBlock> two_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO_DEF");
   EXPECT_THAT(two_def_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));

   const std::optional<commonItems::LocalizationBlock> two_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO_ADJ");
   EXPECT_THAT(two_adj_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "testy two"),
           testing::Pair("spanish", "pruebamente dos")));
}


TEST(Hoi4worldLocalizationsLocalizationsconverter, DynamicCountryLocalizationsCanBeConverted)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});

   // country names and localizations
   commonItems::LocalizationBlock block_one("ONE", "english");
   block_one.ModifyLocalization("english", "Test");
   block_one.ModifyLocalization("spanish", "Prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("ONE", block_one);
   commonItems::LocalizationBlock block_one_adj("ONE_ADJ", "english");
   block_one_adj.ModifyLocalization("english", "Testy");
   block_one_adj.ModifyLocalization("spanish", "Pruebamente");
   vic3_localizations.AddOrModifyLocalizationBlock("ONE_ADJ", block_one_adj);
   commonItems::LocalizationBlock block_two("TWO", "english");
   block_two.ModifyLocalization("english", "Test two");
   block_two.ModifyLocalization("spanish", "Prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("TWO", block_two);
   commonItems::LocalizationBlock block_two_adj("TWO_ADJ", "english");
   block_two_adj.ModifyLocalization("english", "Testy two");
   block_two_adj.ModifyLocalization("spanish", "Pruebamente dos");
   vic3_localizations.AddOrModifyLocalizationBlock("TWO_ADJ", block_two_adj);
   commonItems::LocalizationBlock block_three("THR", "english");
   block_three.ModifyLocalization("english", "Test three");
   block_three.ModifyLocalization("spanish", "Prueba tres");
   vic3_localizations.AddOrModifyLocalizationBlock("THR", block_three);
   commonItems::LocalizationBlock block_three_adj("THR_ADJ", "english");
   block_three_adj.ModifyLocalization("english", "Testy three");
   block_three_adj.ModifyLocalization("spanish", "Pruebamente tres");
   vic3_localizations.AddOrModifyLocalizationBlock("THR_ADJ", block_three_adj);

   // dynamic names and localizations
   commonItems::LocalizationBlock dynamic_block_one("one_dynamic_name", "english");
   dynamic_block_one.ModifyLocalization("english", "dynamic test");
   dynamic_block_one.ModifyLocalization("spanish", "din�mica prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("one_dynamic_name", dynamic_block_one);
   commonItems::LocalizationBlock dynamic_block_one_adj("one_dynamic_adj", "english");
   dynamic_block_one_adj.ModifyLocalization("english", "dynamic testy");
   dynamic_block_one_adj.ModifyLocalization("spanish", "din�mica pruebamente");
   vic3_localizations.AddOrModifyLocalizationBlock("one_dynamic_adj", dynamic_block_one_adj);
   commonItems::LocalizationBlock dynamic_block_two("two_dynamic_name", "english");
   dynamic_block_two.ModifyLocalization("english", "[COUNTRY.GetDefinition.GetName] dynamic test two");
   dynamic_block_two.ModifyLocalization("spanish", "[COUNTRY.GetDefinition.GetName|l] din�mica prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("two_dynamic_name", dynamic_block_two);
   commonItems::LocalizationBlock dynamic_block_two_adj("two_dynamic_adj", "english");
   dynamic_block_two_adj.ModifyLocalization("english", "[COUNTRY.GetDefinition.GetName] dynamic testy two");
   dynamic_block_two_adj.ModifyLocalization("spanish", "[COUNTRY.GetDefinition.GetName|l] din�mica pruebamente dos");
   vic3_localizations.AddOrModifyLocalizationBlock("two_dynamic_adj", dynamic_block_two_adj);
   commonItems::LocalizationBlock dynamic_block_three("three_dynamic_name", "english");
   dynamic_block_three.ModifyLocalization("english", "[COUNTRY.GetDefinition.GetAdjective] dynamic test three");
   dynamic_block_three.ModifyLocalization("spanish", "[COUNTRY.GetDefinition.GetAdjective|l] din�mica prueba tres");
   vic3_localizations.AddOrModifyLocalizationBlock("three_dynamic_name", dynamic_block_three);
   commonItems::LocalizationBlock dynamic_block_three_adj("three_dynamic_adj", "english");
   dynamic_block_three_adj.ModifyLocalization("english", "[COUNTRY.GetDefinition.GetAdjective] dynamic testy three");
   dynamic_block_three_adj.ModifyLocalization("spanish",
       "[COUNTRY.GetDefinition.GetAdjective|l] din�mica pruebamente tres");
   vic3_localizations.AddOrModifyLocalizationBlock("three_dynamic_adj", dynamic_block_three_adj);


   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {
           {1, "ONE"},
           {2, "TWO"},
           {3, "THR"},
       },
       {},
       {},
       mappers::ProvinceMapper({}, {}),
       {
           {1,
               vic3::Country(
                   {.tag = "ONE", .dynamic_name = "one_dynamic_name", .dynamic_adjective = "one_dynamic_adj"})},
           {2,
               vic3::Country(
                   {.tag = "TWO", .dynamic_name = "two_dynamic_name", .dynamic_adjective = "two_dynamic_adj"})},
           {3,
               vic3::Country(
                   {.tag = "THR", .dynamic_name = "three_dynamic_name", .dynamic_adjective = "three_dynamic_adj"})},
       },
       {});

   const std::optional<commonItems::LocalizationBlock> hoi_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("ONE");
   EXPECT_THAT(hoi_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "dynamic test"),
           testing::Pair("spanish", "din�mica prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("ONE_DEF");
   EXPECT_THAT(hoi_def_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "dynamic test"),
           testing::Pair("spanish", "din�mica prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("ONE_ADJ");
   EXPECT_THAT(hoi_adj_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "dynamic testy"),
           testing::Pair("spanish", "din�mica pruebamente")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO");
   EXPECT_THAT(two_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Test two dynamic test two"),
           testing::Pair("spanish", "prueba dos din�mica prueba dos")));

   const std::optional<commonItems::LocalizationBlock> two_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO_DEF");
   EXPECT_THAT(two_def_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Test two dynamic test two"),
           testing::Pair("spanish", "prueba dos din�mica prueba dos")));

   const std::optional<commonItems::LocalizationBlock> two_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO_ADJ");
   EXPECT_THAT(two_adj_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Test two dynamic testy two"),
           testing::Pair("spanish", "prueba dos din�mica pruebamente dos")));

   const std::optional<commonItems::LocalizationBlock> three_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("THR");
   EXPECT_THAT(three_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Testy three dynamic test three"),
           testing::Pair("spanish", "pruebamente tres din�mica prueba tres")));

   const std::optional<commonItems::LocalizationBlock> three_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("THR_DEF");
   EXPECT_THAT(three_def_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Testy three dynamic test three"),
           testing::Pair("spanish", "pruebamente tres din�mica prueba tres")));

   const std::optional<commonItems::LocalizationBlock> three_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("THR_ADJ");
   EXPECT_THAT(three_adj_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Testy three dynamic testy three"),
           testing::Pair("spanish", "pruebamente tres din�mica pruebamente tres")));
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
   EXPECT_THAT(one_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetStateLocalizations().GetLocalizationBlock("STATE_TWO");
   EXPECT_THAT(two_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
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
   EXPECT_THAT(one_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetVictoryPointLocalizations().GetLocalizationBlock("VICTORY_POINTS_2");
   EXPECT_THAT(two_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
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
   EXPECT_THAT(one_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Douggie"), testing::Pair("spanish", "Dougesa")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetCharacterLocalizations().GetLocalizationBlock("Romney1");
   EXPECT_THAT(two_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Romney"), testing::Pair("spanish", "Romney")));

   const std::optional<commonItems::LocalizationBlock> three_localization_block =
       hoi4_localizations.GetCharacterLocalizations().GetLocalizationBlock("Doug_Romney1");
   EXPECT_THAT(three_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "$Doug$ $Romney1$"),
           testing::Pair("spanish", "$Doug$ $Romney1$")));
}

}  // namespace hoi4