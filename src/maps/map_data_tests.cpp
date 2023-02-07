#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/maps/map_data.h"
#include "src/maps/map_data_importer.h"



TEST(MapsMapdata, ExceptionThrownForMissingProvincesBmp)
{
   const commonItems::ModFilesystem mod_filesystem("", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   maps::MapDataImporter importer(province_definitions);

   EXPECT_THROW(importer.ImportMapData(mod_filesystem), std::runtime_error);
}


TEST(MapsMapdata, ExceptionThrownForMissingAdjacenciesCsv)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps/nocsv", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   maps::MapDataImporter importer(province_definitions);

   EXPECT_THROW(importer.ImportMapData(mod_filesystem), std::runtime_error);
}


TEST(MapsMapdata, NeighborsDefined)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},         // the dark red one on top
           {0xED'1C'24, "2"},         // the red red one on the left
           {0x22'B1'4C, "0x000003"},  // the green one in the middle
           {0xFF'7F'27, "0x000004"},  // the orange one on the right
           {0xFF'F2'00, "0x000005"},  // the yellow red one below
           {0x3F'48'CC, "6"},         // the indigo one on the far right
           {0xA3'49'A4, "7"},         // the purple one on the far right
       });
   maps::MapDataImporter importer(province_definitions);

   const maps::MapData map_data = importer.ImportMapData(mod_filesystem);

   EXPECT_THAT(map_data.GetNeighbors("42"), testing::UnorderedElementsAre());                  // non-existent province
   EXPECT_THAT(map_data.GetNeighbors("0x000042"), testing::UnorderedElementsAre());            // non-existent province
   EXPECT_THAT(map_data.GetNeighbors("1"), testing::UnorderedElementsAre("0x000003"));         // defined from above
   EXPECT_THAT(map_data.GetNeighbors("2"), testing::UnorderedElementsAre("0x000003"));         // defined from right
   EXPECT_THAT(map_data.GetNeighbors("0x000004"), testing::UnorderedElementsAre("0x000003"));  // defined from left
   EXPECT_THAT(map_data.GetNeighbors("0x000005"), testing::UnorderedElementsAre("0x000003"));  // defined from above
   EXPECT_THAT(map_data.GetNeighbors("101"), testing::UnorderedElementsAre("102"));          // non-impassable adjacency
   EXPECT_THAT(map_data.GetNeighbors("xD00000"), testing::UnorderedElementsAre("x8CC57E"));  // non-impassable adjacency
   EXPECT_THAT(map_data.GetNeighbors("6"), testing::UnorderedElementsAre());  // impossible adjacency removes neighbor
}


TEST(MapsMapdata, SpecifiedBordersCanBeLookedUp)
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
           {0xFF'F2'00, "0x000005"},  // the yellow red one below
           {0x3F'48'CC, "6"},         // the indigo one on the far right
           {0xA3'49'A4, "0x000007"},  // the purple one on the far right
       });
   maps::MapDataImporter importer(province_definitions);

   const maps::MapData map_data = importer.ImportMapData(mod_filesystem);

   EXPECT_EQ(map_data.GetSpecifiedBorderCenter("42", "0x000001"), std::nullopt);  // non-existent province
   EXPECT_EQ(map_data.GetSpecifiedBorderCenter("1", "0x000005"), std::nullopt);   // non-bordering provinces

   // Bordering provinces
   const auto border_point = map_data.GetSpecifiedBorderCenter("1", "0x000003");
   ASSERT_TRUE(border_point);
   constexpr maps::Point expected_point{13, 591};  // y-axis is from the bottom
   EXPECT_EQ(*border_point, expected_point);

   // Impassable border for bordering provinces
   const auto impassable_border_point = map_data.GetSpecifiedBorderCenter("6", "0x000007");
   ASSERT_TRUE(impassable_border_point);

   constexpr maps::Point expected_impassable_point{44, 586};  // y-axis is from the bottom
   EXPECT_EQ(*impassable_border_point, expected_impassable_point);
}


TEST(MapsMapdata, AnyBordersCanBeLookedUp)
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
   maps::MapDataImporter importer(province_definitions);

   const maps::MapData map_data = importer.ImportMapData(mod_filesystem);


   EXPECT_EQ(map_data.GetAnyBorderCenter("42"), std::nullopt);  // nonexistent province
   EXPECT_EQ(map_data.GetAnyBorderCenter("8"), std::nullopt);   // province with no borders

   // bordering provinces
   const auto border_point = map_data.GetAnyBorderCenter("3");
   ASSERT_TRUE(border_point);

   constexpr maps::Point expected_point{13, 590};  // y-axis is from the bottom
   EXPECT_EQ(*border_point, expected_point);
}


TEST(MapsMapdata, ProvinceNamesCanBeLookedUp)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},  // the dark red one on top
       });
   maps::MapDataImporter importer(province_definitions);

   const maps::MapData map_data = importer.ImportMapData(mod_filesystem);

   EXPECT_EQ(map_data.GetProvinceName({0, 0}), std::nullopt);  // undefined points

   // defined points
   const auto province_name = map_data.GetProvinceName({13, 595});
   ASSERT_TRUE(province_name);
   EXPECT_EQ(*province_name, "1");
}


TEST(MapsMapdata, ProvincePointsCanBeLookedUp)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps", {});
   const maps::ProvinceDefinitions province_definitions({},
       {},
       {},
       {
           {0x88'00'15, "1"},  // the dark red one on top
       });
   maps::MapDataImporter importer(province_definitions);

   const maps::MapData map_data = importer.ImportMapData(mod_filesystem);


   EXPECT_EQ(map_data.GetProvincePoints("42"), std::nullopt);  // undefined province

   // defined province
   const auto provincePoints = map_data.GetProvincePoints("1");
   ASSERT_TRUE(provincePoints);

   constexpr maps::Point expected_point{13, 595};
   EXPECT_EQ(provincePoints->GetCentermostPoint(), expected_point);
}