#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/coastal_provinces_creator.h"
#include "src/maps/map_data.h"
#include "src/maps/province_definitions.h"



TEST(Hoi4worldMapCoastalProvincesCreatorTests, CoastalProvincesDefaultToEmpty)
{
   const maps::ProvinceDefinitions province_definitions;
   const maps::MapData map_data({.province_definitions = province_definitions});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_TRUE(coastal_provinces.GetCoastalProvinces().empty());
}


TEST(Hoi4worldMapCoastalProvincesCreatorTests, CoastalProvincesAreDetected)
{
   const maps::ProvinceDefinitions province_definitions({.land_provinces = {"1"}, .sea_provinces = {"2"}});
   const maps::MapData map_data({.province_neighbors = {{"1", {"2"}}}, .province_definitions = province_definitions});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_THAT(coastal_provinces.GetCoastalProvinces(),
       testing::UnorderedElementsAre(testing::Pair(1, std::vector{2})));
}


TEST(Hoi4worldMapCoastalProvincesCreatorTests, MultipleConnectingSeaProvincesAreDetected)
{
   const maps::ProvinceDefinitions province_definitions({.land_provinces = {"1"}, .sea_provinces = {"2", "3"}});
   const maps::MapData map_data(
       {.province_neighbors = {{"1", {"2", "3"}}}, .province_definitions = province_definitions});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_THAT(coastal_provinces.GetCoastalProvinces(),
       testing::UnorderedElementsAre(testing::Pair(1, std::vector{2, 3})));
}


TEST(Hoi4worldMapCoastalProvincesCreatorTests, NeighboringLandProvincesDoNotMakeForCoasts)
{
   const maps::ProvinceDefinitions province_definitions({.land_provinces = {"1", "2"}});
   const maps::MapData map_data({.province_neighbors = {{"1", {"2"}}}, .province_definitions = province_definitions});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_TRUE(coastal_provinces.GetCoastalProvinces().empty());
}


TEST(Hoi4worldMapCoastalProvincesCreatorTests, BadProvinceNamesAreSkipped)
{
   const maps::ProvinceDefinitions province_definitions({.land_provinces = {"1", "a"}, .sea_provinces = {"2", "b"}});
   const maps::MapData map_data(
       {.province_neighbors = {{"1", {"2", "b"}}, {"a", {"2", "b"}}}, .province_definitions = province_definitions});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_THAT(coastal_provinces.GetCoastalProvinces(),
       testing::UnorderedElementsAre(testing::Pair(1, std::vector{2})));
}