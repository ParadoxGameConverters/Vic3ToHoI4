#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/coastal_provinces_creator.h"
#include "src/maps/map_data.h"
#include "src/maps/province_definitions.h"



TEST(Hoi4worldMapCoastalProvincesCreatorTests, CoastalProvincesDefaultToEmpty)
{
   maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   maps::MapData map_data({}, {}, {}, province_definitions, {});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_TRUE(coastal_provinces.GetCoastalProvinces().empty());
}


TEST(Hoi4worldMapCoastalProvincesCreatorTests, CoastalProvincesAreDetected)
{
   maps::ProvinceDefinitions province_definitions({"1"}, {"2"}, {}, {});
   maps::MapData map_data({{"1", {"2"}}}, {}, {}, province_definitions, {});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_THAT(coastal_provinces.GetCoastalProvinces(),
       testing::UnorderedElementsAre(testing::Pair(1, std::vector{2})));
}


TEST(Hoi4worldMapCoastalProvincesCreatorTests, MultipleConnectingSeaProvincesAreDetected)
{
    maps::ProvinceDefinitions province_definitions({ "1" }, { "2", "3" }, {}, {});
    maps::MapData map_data({ {"1", {"2", "3"}} }, {}, {}, province_definitions, {});
    const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
        province_definitions.GetLandProvinces(),
        province_definitions.GetSeaProvinces());

    EXPECT_THAT(coastal_provinces.GetCoastalProvinces(),
        testing::UnorderedElementsAre(testing::Pair(1, std::vector{ 2, 3 })));
}


TEST(Hoi4worldMapCoastalProvincesCreatorTests, NeighboringLandProvincesDoNotMakeForCoasts)
{
   maps::ProvinceDefinitions province_definitions({"1", "2"}, {}, {}, {});
   maps::MapData map_data({{"1", {"2"}}}, {}, {}, province_definitions, {});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_TRUE(coastal_provinces.GetCoastalProvinces().empty());
}


TEST(Hoi4worldMapCoastalProvincesCreatorTests, BadProvinceNamesAreSkipped)
{
   maps::ProvinceDefinitions province_definitions({"1", "a"}, {"2", "b"}, {}, {});
   maps::MapData map_data({{"1", {"2", "b"}}, {"a", {"2", "b"}}}, {}, {}, province_definitions, {});
   const hoi4::CoastalProvinces coastal_provinces = hoi4::CreateCoastalProvinces(map_data,
       province_definitions.GetLandProvinces(),
       province_definitions.GetSeaProvinces());

   EXPECT_THAT(coastal_provinces.GetCoastalProvinces(),
       testing::UnorderedElementsAre(testing::Pair(1, std::vector{2})));
}