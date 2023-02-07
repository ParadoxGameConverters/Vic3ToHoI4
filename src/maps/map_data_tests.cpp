#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/maps/map_data.h"
#include "src/maps/map_data_importer.h"



class MapsMapdata: public ::testing::Test
{
  protected:
   MapsMapdata(): map_data_{{}, {}, {}, {{}, {}, {}, {}}, {}} {}

   void SetUp() override
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

      map_data_ = importer.ImportMapData(mod_filesystem);
   }

   maps::MapData map_data_;
};



TEST_F(MapsMapdata, ExceptionThrownForMissingProvincesBmp)
{
   const commonItems::ModFilesystem mod_filesystem("", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   maps::MapDataImporter importer(province_definitions);

   EXPECT_THROW(importer.ImportMapData(mod_filesystem), std::runtime_error);
}


TEST_F(MapsMapdata, ExceptionThrownForMissingAdjacenciesCsv)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/maps/nocsv", {});
   const maps::ProvinceDefinitions province_definitions({}, {}, {}, {});
   maps::MapDataImporter importer(province_definitions);

   EXPECT_THROW(importer.ImportMapData(mod_filesystem), std::runtime_error);
}


TEST_F(MapsMapdata, NeighborsDefined)
{
   EXPECT_THAT(map_data_.GetNeighbors("42"), testing::UnorderedElementsAre());        // non-existent province
   EXPECT_THAT(map_data_.GetNeighbors("0x000042"), testing::UnorderedElementsAre());  // non-existent province
   EXPECT_THAT(map_data_.GetNeighbors("1"), testing::UnorderedElementsAre("3"));      // defined from above
   EXPECT_THAT(map_data_.GetNeighbors("2"), testing::UnorderedElementsAre("3"));      // defined from right
   EXPECT_THAT(map_data_.GetNeighbors("4"), testing::UnorderedElementsAre("3"));      // defined from left
   EXPECT_THAT(map_data_.GetNeighbors("5"), testing::UnorderedElementsAre("3"));      // defined from above
   EXPECT_THAT(map_data_.GetNeighbors("101"), testing::UnorderedElementsAre("102"));  // non-impassable adjacency
   EXPECT_THAT(map_data_.GetNeighbors("xD00000"),
       testing::UnorderedElementsAre("x8CC57E"));                              // non-impassable adjacency
   EXPECT_THAT(map_data_.GetNeighbors("6"), testing::UnorderedElementsAre());  // impossible adjacency removes neighbor
}


TEST_F(MapsMapdata, SpecifiedBordersCanBeLookedUp)
{
   EXPECT_EQ(map_data_.GetSpecifiedBorderCenter("42", "0x000001"), std::nullopt);  // non-existent province
   EXPECT_EQ(map_data_.GetSpecifiedBorderCenter("1", "0x000005"), std::nullopt);   // non-bordering provinces

   // Bordering provinces
   const auto border_point = map_data_.GetSpecifiedBorderCenter("1", "3");
   ASSERT_TRUE(border_point);
   constexpr maps::Point expected_point{13, 591};  // y-axis is from the bottom
   EXPECT_EQ(*border_point, expected_point);

   // Impassable border for bordering provinces
   const auto impassable_border_point = map_data_.GetSpecifiedBorderCenter("6", "7");
   ASSERT_TRUE(impassable_border_point);

   constexpr maps::Point expected_impassable_point{44, 586};  // y-axis is from the bottom
   EXPECT_EQ(*impassable_border_point, expected_impassable_point);
}


TEST_F(MapsMapdata, AnyBordersCanBeLookedUp)
{
   EXPECT_EQ(map_data_.GetAnyBorderCenter("42"), std::nullopt);  // nonexistent province
   EXPECT_EQ(map_data_.GetAnyBorderCenter("8"), std::nullopt);   // province with no borders

   // bordering provinces
   const auto border_point = map_data_.GetAnyBorderCenter("3");
   ASSERT_TRUE(border_point);

   constexpr maps::Point expected_point{13, 590};  // y-axis is from the bottom
   EXPECT_EQ(*border_point, expected_point);
}


TEST_F(MapsMapdata, ProvinceNamesCanBeLookedUp)
{
   EXPECT_EQ(map_data_.GetProvinceName({0, 0}), std::nullopt);  // undefined points

   // defined points
   const auto province_name = map_data_.GetProvinceName({13, 595});
   ASSERT_TRUE(province_name);
   EXPECT_EQ(*province_name, "1");
}


TEST_F(MapsMapdata, ProvincePointsCanBeLookedUp)
{
   EXPECT_EQ(map_data_.GetProvincePoints("42"), std::nullopt);  // undefined province

   // defined province
   const auto provincePoints = map_data_.GetProvincePoints("1");
   ASSERT_TRUE(provincePoints);

   constexpr maps::Point expected_point{13, 595};
   EXPECT_EQ(provincePoints->GetCentermostPoint(), expected_point);
}