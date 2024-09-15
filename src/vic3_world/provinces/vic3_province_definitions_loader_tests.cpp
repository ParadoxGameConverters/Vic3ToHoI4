#include <sstream>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"



namespace vic3
{

TEST(Vic3WorldProvincesVic3ProvinceDefinitionsLoader, DefinitionsAreOrderedByStrategicRegionFile)
{
   const auto province_definitions = LoadProvinceDefinitions(
       {
           .name_to_region_map =
               {
                   {"STATE_1", StateRegion{{}, {"xFF0000"}}},
                   {"STATE_2", StateRegion{{}, {"x00FF00"}}},
               },
           .region_indexes = {{"STATE_1", 1}, {"STATE_2", 2}},
       },
       commonItems::ModFilesystem{
           "test_files/Vic3WorldProvincesVic3ProvinceDefinitions/DefinitionsAreOrderedByStrategicRegionFile",
           {}});

   EXPECT_THAT(province_definitions.GetProvinceDefinitions(), testing::ElementsAre("x00FF00", "xFF0000"));
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitionsLoader, DefinitionsAreOrderedByRegionOrderInStrategicRegionFile)
{
   const auto province_definitions = LoadProvinceDefinitions(
       {
           .name_to_region_map =
               {
                   {"STATE_1", StateRegion{{}, {"xFF0000"}}},
                   {"STATE_2", StateRegion{{}, {"x00FF00"}}},
               },
           .region_indexes = {{"STATE_1", 1}, {"STATE_2", 2}},
       },
       commonItems::ModFilesystem{"test_files/Vic3WorldProvincesVic3ProvinceDefinitions/"
                                  "DefinitionsAreOrderedByRegionOrderInStrategicRegionFile",
           {}});

   EXPECT_THAT(province_definitions.GetProvinceDefinitions(), testing::ElementsAre("x00FF00", "xFF0000"));
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitionsLoader, DefinitionsAreOrderedByStateRegionIndexingOrder)
{
   // The state region indexing order is from scanning map_data/state_regions in file order and the order within the
   // files
   const auto province_definitions = LoadProvinceDefinitions(
       {
           .name_to_region_map =
               {
                   {"STATE_1", StateRegion{{}, {"xFF0000"}}},
                   {"STATE_2", StateRegion{{}, {"x00FF00"}}},
               },
           .region_indexes = {{"STATE_1", 2}, {"STATE_2", 1}},
       },
       commonItems::ModFilesystem{
           "test_files/Vic3WorldProvincesVic3ProvinceDefinitions/DefinitionsAreOrderedByStateRegionIndexingOrder",
           {}});

   EXPECT_THAT(province_definitions.GetProvinceDefinitions(), testing::ElementsAre("x00FF00", "xFF0000"));
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitionsLoader, DefinitionsAreOrderedByBottomToTopInMapDataProvincesDotPng)
{
   const auto province_definitions = LoadProvinceDefinitions(
       {
           .name_to_region_map =
               {
                   {"STATE", StateRegion{{}, {"xFF0000", "x00FF00"}}},
               },
           .region_indexes = {{"STATE", 1}},
       },
       commonItems::ModFilesystem{"test_files/Vic3WorldProvincesVic3ProvinceDefinitions/"
                                  "DefinitionsAreOrderedByBottomToTopInMapDataProvincesDotPng",
           {}});

   EXPECT_THAT(province_definitions.GetProvinceDefinitions(), testing::ElementsAre("x00FF00", "xFF0000"));
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitionsLoader, DefinitionsAreOrderedByLeftToRightInMapDataProvincesDotPng)
{
   const auto province_definitions = LoadProvinceDefinitions(
       {
           .name_to_region_map =
               {
                   {"STATE", StateRegion{{}, {"xFF0000", "x00FF00"}}},
               },
           .region_indexes = {{"STATE", 1}},
       },
       commonItems::ModFilesystem{"test_files/Vic3WorldProvincesVic3ProvinceDefinitions/"
                                  "DefinitionsAreOrderedByLeftToRightInMapDataProvincesDotPng",
           {}});

   EXPECT_THAT(province_definitions.GetProvinceDefinitions(), testing::ElementsAre("x00FF00", "xFF0000"));
}

}  // namespace vic3