#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/characters/hoi4_characters_converter.h"
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
       ConvertLocalizations(vic3_localizations, {}, {}, {}, mappers::ProvinceMapper({}, {}), {}, {}, {}, {});

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
       {},
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
       {},
       {},
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
   dynamic_block_one.ModifyLocalization("spanish", "dinámica prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("one_dynamic_name", dynamic_block_one);
   commonItems::LocalizationBlock dynamic_block_one_adj("one_dynamic_adj", "english");
   dynamic_block_one_adj.ModifyLocalization("english", "dynamic testy");
   dynamic_block_one_adj.ModifyLocalization("spanish", "dinámica pruebamente");
   vic3_localizations.AddOrModifyLocalizationBlock("one_dynamic_adj", dynamic_block_one_adj);
   commonItems::LocalizationBlock dynamic_block_two("two_dynamic_name", "english");
   dynamic_block_two.ModifyLocalization("english", "[COUNTRY.GetDefinition.GetName] dynamic test two");
   dynamic_block_two.ModifyLocalization("spanish", "[COUNTRY.GetDefinition.GetName|l] dinámica prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("two_dynamic_name", dynamic_block_two);
   commonItems::LocalizationBlock dynamic_block_two_adj("two_dynamic_adj", "english");
   dynamic_block_two_adj.ModifyLocalization("english", "[COUNTRY.GetDefinition.GetName] dynamic testy two");
   dynamic_block_two_adj.ModifyLocalization("spanish", "[COUNTRY.GetDefinition.GetName|l] dinámica pruebamente dos");
   vic3_localizations.AddOrModifyLocalizationBlock("two_dynamic_adj", dynamic_block_two_adj);
   commonItems::LocalizationBlock dynamic_block_three("three_dynamic_name", "english");
   dynamic_block_three.ModifyLocalization("english", "[COUNTRY.GetDefinition.GetAdjective] dynamic test three");
   dynamic_block_three.ModifyLocalization("spanish", "[COUNTRY.GetDefinition.GetAdjective|l] dinámica prueba tres");
   vic3_localizations.AddOrModifyLocalizationBlock("three_dynamic_name", dynamic_block_three);
   commonItems::LocalizationBlock dynamic_block_three_adj("three_dynamic_adj", "english");
   dynamic_block_three_adj.ModifyLocalization("english", "[COUNTRY.GetDefinition.GetAdjective] dynamic testy three");
   dynamic_block_three_adj.ModifyLocalization("spanish",
       "[COUNTRY.GetDefinition.GetAdjective|l] dinámica pruebamente tres");
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
       {},
       {},
       {});

   const std::optional<commonItems::LocalizationBlock> hoi_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("ONE");
   EXPECT_THAT(hoi_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "dynamic test"),
           testing::Pair("spanish", "dinámica prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("ONE_DEF");
   EXPECT_THAT(hoi_def_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "dynamic test"),
           testing::Pair("spanish", "dinámica prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("ONE_ADJ");
   EXPECT_THAT(hoi_adj_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "dynamic testy"),
           testing::Pair("spanish", "dinámica pruebamente")));

   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO");
   EXPECT_THAT(two_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Test two dynamic test two"),
           testing::Pair("spanish", "prueba dos dinámica prueba dos")));

   const std::optional<commonItems::LocalizationBlock> two_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO_DEF");
   EXPECT_THAT(two_def_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Test two dynamic test two"),
           testing::Pair("spanish", "prueba dos dinámica prueba dos")));

   const std::optional<commonItems::LocalizationBlock> two_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("TWO_ADJ");
   EXPECT_THAT(two_adj_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Test two dynamic testy two"),
           testing::Pair("spanish", "prueba dos dinámica pruebamente dos")));

   const std::optional<commonItems::LocalizationBlock> three_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("THR");
   EXPECT_THAT(three_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Testy three dynamic test three"),
           testing::Pair("spanish", "pruebamente tres dinámica prueba tres")));

   const std::optional<commonItems::LocalizationBlock> three_def_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("THR_DEF");
   EXPECT_THAT(three_def_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Testy three dynamic test three"),
           testing::Pair("spanish", "pruebamente tres dinámica prueba tres")));

   const std::optional<commonItems::LocalizationBlock> three_adj_localization_block =
       hoi4_localizations.GetCountryLocalizations().GetLocalizationBlock("THR_ADJ");
   EXPECT_THAT(three_adj_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Testy three dynamic testy three"),
           testing::Pair("spanish", "pruebamente tres dinámica pruebamente tres")));
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
       ConvertLocalizations(vic3_localizations, {}, {}, {}, mappers::ProvinceMapper({}, {}), {}, {}, {}, {});

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
       {},
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
       {},
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
       {},
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
       {},
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
       {},
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
       {},
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
       },
       {},
       {});


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


TEST(Hoi4worldLocalizationsLocalizationsconverter, IdeaLocalizationsCanBeConverted)
{
   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("male", "english");
   block_one.ModifyLocalization("english", "MaleName");
   block_one.ModifyLocalization("spanish", "MaleNombre");
   vic3_localizations.AddOrModifyLocalizationBlock("male", block_one);
   commonItems::LocalizationBlock block_two("female", "english");
   block_two.ModifyLocalization("english", "FemaleName");
   block_two.ModifyLocalization("spanish", "FemaleNombre");
   vic3_localizations.AddOrModifyLocalizationBlock("female", block_two);
   commonItems::LocalizationBlock block_three("monarch", "english");
   block_three.ModifyLocalization("english", "Surname");
   block_three.ModifyLocalization("spanish", "Surnombre");
   vic3_localizations.AddOrModifyLocalizationBlock("monarch", block_three);


   const Character character_one({.first_name = "male", .last_name = "monarch", .is_female = false});
   const Character character_two({.first_name = "female", .last_name = "monarch", .is_female = true});

   const Localizations hoi4_localizations = ConvertLocalizations(vic3_localizations,
       {},
       {},
       {},
       mappers::ProvinceMapper({}, {}),
       {},
       {},
       {
           {"ONE", Country({.monarch_idea_id = 1})},
           {"TWO", Country({.monarch_idea_id = 2})},
           {"NON", Country({.monarch_idea_id = std::nullopt})},
           {"MIS", Country({.monarch_idea_id = 3})},
       },
       {
           {1, character_one},
           {2, character_two},
       });

   EXPECT_EQ(hoi4_localizations.GetIdeaLocalizations().size(), 4);

   const std::string idea_one_name = hoi4::GetMonarchIdeaName("ONE", character_one);
   const std::string idea_one_desc = fmt::format("{}_desc", idea_one_name);
   const std::optional<commonItems::LocalizationBlock> one_localization_block =
       hoi4_localizations.GetIdeaLocalizations().GetLocalizationBlock(idea_one_name);
   EXPECT_THAT(one_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "King MaleName Surname"),
           testing::Pair("braz_por", "Rei MaleName Surname"),
           testing::Pair("french", "Roi MaleName Surname"),
           testing::Pair("german", "König MaleName Surname"),
           testing::Pair("japanese", "MaleName Surname 王"),
           testing::Pair("polish", "Król MaleName Surname"),
           testing::Pair("russian", "Король MaleName Surname"),
           testing::Pair("spanish", "Rey MaleNombre Surnombre")));

   const std::optional<commonItems::LocalizationBlock> one_localization_desc_block =
       hoi4_localizations.GetIdeaLocalizations().GetLocalizationBlock(idea_one_desc);
   EXPECT_THAT(one_localization_desc_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(
           testing::Pair("english",
               "Rallying around the King of [ONE.GetName] and the [ONE.GetAdjective] Dominions, the [ONE.GetAdjective] "
               "people stand united and proud of their imperial legacy."),
           testing::Pair("braz_por",
               "Reunindo-se em torno do Rei da [ONE.GetName] e dos Domínios [ONE.GetAdjective], o povo "
               "[ONE.GetAdjective] se mantém unido e orgulhoso de seu legado imperial."),
           testing::Pair("french",
               "Rassemblant autour du Roi de [ONE.GetName] et des colonies [ONE.GetAdjective]s, la nation "
               "[ONE.GetAdjective] est unie et fière de son héritage impérial."),
           testing::Pair("german",
               "Das [ONE.GetAdjective]e Volk versammelt sich um den König von [ONE.GetName] und die "
               "[ONE.GetAdjective]e Kolonien und ist vereint und stolz auf sein kaiserliches Erbe."),
           testing::Pair("japanese",
               "[ONE.GetName]の国王と[ONE.GetAdjective]の領土の周りに結集し、[ONE.GetAdjective]"
               "人は団結し、帝国の遺産を誇りに思っています"),
           testing::Pair("polish",
               "Gromadząc się wokół Króla [ONE.GetName] i [ONE.GetAdjective]ich Dominiów, [ONE.GetAdjective] są "
               "zjednoczeni i dumni ze swojego imperialnego dziedzictwa."),
           testing::Pair("russian",
               "Сплотившись вокруг Короля [ONE.GetName] и [ONE.GetAdjective]их Доминионов, [ONE.GetAdjective]ий народ "
               "объединяется и гордится своим имперским наследием."),
           testing::Pair("spanish",
               "Reunida en torno al Rey de [ONE.GetName] y las tierras [ONE.GetAdjective]s, la nación "
               "[ONE.GetAdjective] está unida y orgullosa de su legado imperial.")));

   const std::string idea_two_name = hoi4::GetMonarchIdeaName("TWO", character_two);
   const std::string idea_two_desc = fmt::format("{}_desc", idea_two_name);
   const std::optional<commonItems::LocalizationBlock> two_localization_block =
       hoi4_localizations.GetIdeaLocalizations().GetLocalizationBlock(idea_two_name);
   EXPECT_THAT(two_localization_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "Queen FemaleName Surname"),
           testing::Pair("braz_por", "Rainha FemaleName Surname"),
           testing::Pair("french", "Reine FemaleName Surname"),
           testing::Pair("german", "Königin FemaleName Surname"),
           testing::Pair("japanese", "FemaleName Surname 女王"),
           testing::Pair("polish", "Królowa FemaleName Surname"),
           testing::Pair("russian", "Королева FemaleName Surname"),
           testing::Pair("spanish", "Reina FemaleNombre Surnombre")));

   const std::optional<commonItems::LocalizationBlock> two_localization_desc_block =
       hoi4_localizations.GetIdeaLocalizations().GetLocalizationBlock(idea_two_desc);
   EXPECT_THAT(two_localization_desc_block.value_or(commonItems::LocalizationBlock{"", ""}).GetLocalizations(),
       testing::UnorderedElementsAre(
           testing::Pair("english",
               "Rallying around the Queen of [TWO.GetName] and the [TWO.GetAdjective] Dominions, the "
               "[TWO.GetAdjective] people stand united and proud of their imperial legacy."),
           testing::Pair("braz_por",
               "Reunindo-se em torno da Rainha da [TWO.GetName] e dos Domínios [TWO.GetAdjective], o povo "
               "[TWO.GetAdjective] se mantém unido e orgulhoso de seu legado imperial."),
           testing::Pair("french",
               "Rassemblant autour de la Reine de [TWO.GetName] et des colonies [TWO.GetAdjective]s, la nation "
               "[TWO.GetAdjective] est unie et fière de son héritage impérial."),
           testing::Pair("german",
               "Das [TWO.GetAdjective]e Volk versammelt sich um die Königin von [TWO.GetName] und die "
               "[TWO.GetAdjective]e Kolonien und ist vereint und stolz auf sein kaiserliches Erbe."),
           testing::Pair("japanese",
               "[TWO.GetName]の女王と[TWO.GetAdjective]の領土の周りに結集し、[TWO.GetAdjective]"
               "人は団結し、帝国の遺産を誇りに思っています"),
           testing::Pair("polish",
               "Gromadząc się wokół Królowej [TWO.GetName] i [TWO.GetAdjective]ich Dominiów, [TWO.GetAdjective] są "
               "zjednoczeni i dumni ze swojego imperialnego dziedzictwa."),
           testing::Pair("russian",
               "Сплотившись вокруг Королевы [TWO.GetName] и [TWO.GetAdjective]их Доминионов, [TWO.GetAdjective]ий "
               "народ объединяется и гордится своим имперским наследием."),
           testing::Pair("spanish",
               "Reunida en torno a la Reina de [TWO.GetName] y las tierras [TWO.GetAdjective]s, la nación "
               "[TWO.GetAdjective] está unida y orgullosa de su legado imperial.")));
}

}  // namespace hoi4