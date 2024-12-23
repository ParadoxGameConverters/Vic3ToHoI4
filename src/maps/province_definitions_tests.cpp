#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/maps/province_definitions.h"



namespace maps
{

TEST(MapsProvincedefinitions, LandProvincesDefaultToEmpty)
{
   const ProvinceDefinitions province_definitions;

   EXPECT_TRUE(province_definitions.GetLandProvinces().empty());
}


TEST(MapsProvincedefinitions, LandProvincesAreReturned)
{
   const ProvinceDefinitions province_definitions({.land_provinces = {"1", "2", "3", "4", "0x000005"}});

   EXPECT_THAT(province_definitions.GetLandProvinces(), testing::UnorderedElementsAre("1", "2", "3", "4", "0x000005"));
}


TEST(MapsProvincedefinitions, ProvincesCanBeShownAreNotLandProvinces)
{
   const ProvinceDefinitions province_definitions;

   EXPECT_FALSE(province_definitions.IsLandProvince("1"));
   EXPECT_FALSE(province_definitions.IsLandProvince("0x000002"));
}


TEST(MapsProvincedefinitions, ProvincesCanBeShownAreLandProvinces)
{
   const ProvinceDefinitions province_definitions({.land_provinces = {"1", "0x000002"}});

   EXPECT_TRUE(province_definitions.IsLandProvince("1"));
   EXPECT_TRUE(province_definitions.IsLandProvince("0x000002"));
}


TEST(MapsProvincedefinitions, SeaProvincesAreReturned)
{
   const ProvinceDefinitions province_definitions({.sea_provinces = {"1", "2", "3", "4", "0x000005"}});

   EXPECT_THAT(province_definitions.GetSeaProvinces(), testing::UnorderedElementsAre("1", "2", "3", "4", "0x000005"));
}


TEST(MapsProvincedefinitions, ProvincesCanBeShownAreNotSeaProvinces)
{
   const ProvinceDefinitions province_definitions;

   EXPECT_FALSE(province_definitions.IsSeaProvince("1"));
   EXPECT_FALSE(province_definitions.IsSeaProvince("0x000002"));
}


TEST(MapsProvincedefinitions, ProvincesCanBeShownAreSeaProvinces)
{
   const ProvinceDefinitions province_definitions({.sea_provinces = {"1", "0x000002"}});

   EXPECT_TRUE(province_definitions.IsSeaProvince("1"));
   EXPECT_TRUE(province_definitions.IsSeaProvince("0x000002"));
}


TEST(MapsProvincedefinitions, ColorWithNoProvinceReturnsNullopt)
{
   const ProvinceDefinitions province_definitions;

   EXPECT_EQ(province_definitions.GetProvinceFromColor(commonItems::Color(std::array{0, 0, 0})), std::nullopt);
}


TEST(MapsProvincedefinitions, ProvincesCanBeLookedUpByColor)
{
   const ProvinceDefinitions province_definitions(
       {.color_to_province_map = {{{0x01'02'03, "1"}, {0x10'20'30, "0x000010"}}}});

   EXPECT_EQ(province_definitions.GetProvinceFromColor(commonItems::Color(std::array{1, 2, 3})), "1");
   EXPECT_EQ(province_definitions.GetProvinceFromColor(commonItems::Color(std::array{16, 32, 48})), "0x000010");
}


TEST(MapsProvincedefinitions, TerrainTypeDefaultsToNullopt)
{
   const ProvinceDefinitions province_definitions;

   EXPECT_EQ(province_definitions.GetTerrainType("1"), std::nullopt);
   EXPECT_EQ(province_definitions.GetTerrainType("0x000002"), std::nullopt);
}


TEST(MapsProvincedefinitions, TerrainTypeCanBeLookedUp)
{
   const ProvinceDefinitions province_definitions(
       {.terrain_types = {{"1", "test_terrain"}, {"0x000002", "test_terrain_two"}}});

   EXPECT_EQ(province_definitions.GetTerrainType("1"), "test_terrain");
   EXPECT_EQ(province_definitions.GetTerrainType("0x000002"), "test_terrain_two");
}

}  // namespace maps