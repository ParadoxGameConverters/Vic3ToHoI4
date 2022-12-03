#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/maps/map_data.h"



TEST(MapsMapdata, ExceptionThrownForMissingProvincesBmp)
{
   const commonItems::ModFilesystem mod_filesystem("", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   EXPECT_THROW(const maps::MapData map_data(province_definitions, mod_filesystem), std::runtime_error);
}


TEST(MapsMapdata, ExceptionThrownForMissingAdjacenciesCsv)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps/nocsv", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   EXPECT_THROW(const maps::MapData map_data(province_definitions, mod_filesystem), std::runtime_error);
}


TEST(MapsMapdata, NoNeighborsForNonexistentProvince)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_THAT(map_data.GetNeighbors("42"), testing::UnorderedElementsAre());
   EXPECT_THAT(map_data.GetNeighbors("0x000042"), testing::UnorderedElementsAre());
}


TEST(MapsMapdata, NeighborDefinedFromBelow)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},         // the dark red one on top
           {0x22'B1'4C, "0x000003"},  // the green one in the middle
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_THAT(map_data.GetNeighbors("1"), testing::UnorderedElementsAre("0x000003"));
}


TEST(MapsMapdata, NeighborDefinedFromRight)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0xED'1C'24, "2"},         // the red red one on the left
           {0x22'B1'4C, "0x000003"},  // the green one in the middle
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_THAT(map_data.GetNeighbors("2"), testing::UnorderedElementsAre("0x000003"));
}


TEST(MapsMapdata, NeighborDefinedFromLeft)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x22'B1'4C, "3"},         // the green one in the middle
           {0xFF'7F'27, "0x000004"},  // the orange one on the right
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_THAT(map_data.GetNeighbors("0x000004"), testing::UnorderedElementsAre("3"));
}


TEST(MapsMapdata, NeighborDefinedFromAbove)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x22'B1'4C, "3"},         // the green one in the middle
           {0xFF'F2'00, "0x000005"},  // the yellow red one below
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_THAT(map_data.GetNeighbors("0x000005"), testing::UnorderedElementsAre("3"));
}


TEST(MapsMapdata, NeighborDefinedFromNonImpassableAdjacency)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_THAT(map_data.GetNeighbors("101"), testing::UnorderedElementsAre("102"));
   EXPECT_THAT(map_data.GetNeighbors("xD00000"), testing::UnorderedElementsAre("x8CC57E"));
}


TEST(MapsMapdata, NeighborRemovedFromImpassableAdjacency)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x3F'48'CC, "6"},  // the indigo one on the far right
           {0xA3'49'A4, "7"},  // the purple one on the far right
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_THAT(map_data.GetNeighbors("6"), testing::UnorderedElementsAre());
}


TEST(MapsMapdata, NoSpecifiedBorderForNonexistentProvince)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_EQ(map_data.GetSpecifiedBorderCenter("42", "0x000001"), std::nullopt);
}


TEST(MapsMapdata, NoBorderForNonBorderingProvinces)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},         // the dark red one on top
           {0xED'1C'24, "2"},         // the red red one on the left
           {0x22'B1'4C, "3"},         // the green one in the middle
           {0xFF'7F'27, "4"},         // the orange one on the right
           {0xFF'F2'00, "0x000005"},  // the yellow red one below
           {0x3F'48'CC, "6"},         // the indigo one on the far right
           {0xA3'49'A4, "7"},         // the purple one on the far right
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_EQ(map_data.GetSpecifiedBorderCenter("1", "0x000005"), std::nullopt);
}


TEST(MapsMapdata, SpecifiedBorderForBorderingProvinces)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},         // the dark red one on top
           {0xED'1C'24, "2"},         // the red red one on the left
           {0x22'B1'4C, "0x000003"},  // the green one in the middle
           {0xFF'7F'27, "4"},         // the orange one on the right
           {0xFF'F2'00, "5"},         // the yellow red one below
           {0x3F'48'CC, "6"},         // the indigo one on the far right
           {0xA3'49'A4, "7"},         // the purple one on the far right
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   const auto border_point = map_data.GetSpecifiedBorderCenter("1", "0x000003");
   ASSERT_TRUE(border_point);

   constexpr maps::Point expected_point{13, 591};  // y-axis is from the bottom
   EXPECT_EQ(*border_point, expected_point);
}


TEST(MapsMapdata, SpecifiedBorderForImpassableBorderProvinces)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},         // the dark red one on top
           {0xED'1C'24, "2"},         // the red red one on the left
           {0x22'B1'4C, "3"},         // the green one in the middle
           {0xFF'7F'27, "4"},         // the orange one on the right
           {0xFF'F2'00, "5"},         // the yellow red one below
           {0x3F'48'CC, "6"},         // the indigo one on the far right
           {0xA3'49'A4, "0x000007"},  // the purple one on the far right
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   const auto border_point = map_data.GetSpecifiedBorderCenter("6", "0x000007");
   ASSERT_TRUE(border_point);

   constexpr maps::Point expected_point{44, 586};  // y-axis is from the bottom
   EXPECT_EQ(*border_point, expected_point);
}


TEST(MapsMapdata, NoBorderForNonexistentProvince)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_EQ(map_data.GetAnyBorderCenter("42"), std::nullopt);
}


TEST(MapsMapdata, NoBorderForProvinceWithNoBorders)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x00'A2'E8, "8"},  // the blue province to the very far right
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_EQ(map_data.GetAnyBorderCenter("8"), std::nullopt);
}


TEST(MapsMapdata, AnyBorderForBorderingProvinces)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},  // the dark red one on top
           {0xED'1C'24, "2"},  // the red red one on the left
           {0x22'B1'4C, "3"},  // the green one in the middle
           {0xFF'7F'27, "4"},  // the orange one on the right
           {0xFF'F2'00, "5"},  // the yellow red one below
           {0x3F'48'CC, "6"},  // the indigo one on the far right
           {0xA3'49'A4, "7"},  // the purple one on the far right
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   const auto border_point = map_data.GetAnyBorderCenter("3");
   ASSERT_TRUE(border_point);

   constexpr maps::Point expected_point{13, 590};  // y-axis is from the bottom
   EXPECT_EQ(*border_point, expected_point);
}


TEST(MapsMapdata, NoProvinceNumberForUndefinedPoint)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_EQ(map_data.GetProvinceName({0, 0}), std::nullopt);
}


TEST(MapsMapdata, ProvinceNumberForDefinedPoint)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},  // the dark red one on top
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   const auto province_number = map_data.GetProvinceName({13, 595});
   ASSERT_TRUE(province_number);
   EXPECT_EQ(*province_number, "1");
}


TEST(MapsMapdata, NoProvincePointsForUndefinedProvince)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   const maps::MapData map_data(province_definitions, mod_filesystem);

   EXPECT_EQ(map_data.GetProvincePoints("42"), std::nullopt);
}


TEST(MapsMapdata, ProvincePointsForDefinedProvince)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},  // the dark red one on top
       });
   const maps::MapData map_data(province_definitions, mod_filesystem);

   const auto provincePoints = map_data.GetProvincePoints("1");
   ASSERT_TRUE(provincePoints);

   constexpr maps::Point expected_point{13, 595};
   EXPECT_EQ(provincePoints->GetCentermostPoint(), expected_point);
}