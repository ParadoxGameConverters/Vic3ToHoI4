#include <sstream>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/map/strategic_regions_importer.h"



namespace hoi4
{

TEST(Hoi4WorldMapStrategicRegionsTests, StrategicRegionsCanBeUpdated)
{
   StrategicRegions strategic_regions({
       .strategic_regions =
           {
               {42, StrategicRegion({.id = 42, .old_provinces = {144, 169}})},
               {76, StrategicRegion({.id = 76, .old_provinces = {76, 42}})},
           },
       .original_province_to_strategic_region_map =
           {
               {144, 42},
               {169, 42},
               {76, 76},
               {42, 76},
           },
   });

   strategic_regions.UpdateToMatchNewStates(
       {
           State(1, {.provinces = {144}}),
           State(2, {.provinces = {169, 42, 76}}),
       },
       {});

   const auto the_strategic_regions = strategic_regions.GetStrategicRegions();

   const auto region_42 = the_strategic_regions.find(42);
   ASSERT_NE(region_42, the_strategic_regions.end());
   EXPECT_THAT(region_42->second.GetOldProvinces(), testing::UnorderedElementsAre(144, 169));
   EXPECT_THAT(region_42->second.GetNewProvinces(), testing::UnorderedElementsAre(144));

   const auto region_76 = the_strategic_regions.find(76);
   ASSERT_NE(region_76, the_strategic_regions.end());
   EXPECT_THAT(region_76->second.GetOldProvinces(), testing::UnorderedElementsAre(42, 76));
   EXPECT_THAT(region_76->second.GetNewProvinces(), testing::UnorderedElementsAre(169, 42, 76));

   EXPECT_THAT(strategic_regions.GetProvinceToStrategicRegionMap(),
       testing::UnorderedElementsAre(testing::Pair(42, 76),
           testing::Pair(76, 76),
           testing::Pair(169, 76),
           testing::Pair(144, 42)));
}


TEST(Hoi4WorldMapStrategicRegionsTests, UnassignedSurroundedProvincesAreAddedToSurroundingRegions)
{
   StrategicRegions strategic_regions({
       .strategic_regions =
           {
               {10, StrategicRegion({.id = 10, .old_provinces = {1, 2, 3}})},
               {11, StrategicRegion({.id = 11, .old_provinces = {4, 5}})},
               {12, StrategicRegion({.id = 12, .old_provinces = {99}})},
           },
       .original_province_to_strategic_region_map =
           {
               {1, 10},
               {2, 10},
               {3, 10},
               {4, 11},
               {5, 11},
               {99, 12},
           },
   });

   strategic_regions.UpdateToMatchNewStates(
       {
           State(1, {.provinces = {1, 2, 3, 4, 5}}),
       },
       maps::MapData(maps::MapDataOptions{
           .province_neighbors = {{"99", {"1", "2", "3", "4", "5"}}},
       }));

   EXPECT_THAT(strategic_regions.GetProvinceToStrategicRegionMap(),
       testing::UnorderedElementsAre(testing::Pair(1, 10),
           testing::Pair(2, 10),
           testing::Pair(3, 10),
           testing::Pair(4, 10),
           testing::Pair(5, 10),
           testing::Pair(99, 10)));
}


TEST(Hoi4WorldMapStrategicRegionsTests, LeftoverProvincesAreAddedBackToOriginalRegions)
{
   StrategicRegions strategic_regions({
       .strategic_regions =
           {
               {42, StrategicRegion({.id = 42, .old_provinces = {144, 169}})},
               {76, StrategicRegion({.id = 76, .old_provinces = {76, 42}})},
           },
       .original_province_to_strategic_region_map =
           {
               {144, 42},
               {169, 42},
               {76, 76},
               {42, 76},
           },
   });

   strategic_regions.UpdateToMatchNewStates({}, {});

   const auto original_strategic_regions = strategic_regions.GetStrategicRegions();

   const auto region_42 = original_strategic_regions.find(42);
   ASSERT_NE(region_42, original_strategic_regions.end());
   EXPECT_THAT(region_42->second.GetOldProvinces(), testing::UnorderedElementsAre(144, 169));
   EXPECT_THAT(region_42->second.GetNewProvinces(), testing::UnorderedElementsAre(144, 169));

   const auto region_76 = original_strategic_regions.find(76);
   ASSERT_NE(region_76, original_strategic_regions.end());
   EXPECT_THAT(region_76->second.GetOldProvinces(), testing::UnorderedElementsAre(42, 76));
   EXPECT_THAT(region_76->second.GetNewProvinces(), testing::UnorderedElementsAre(42, 76));
}


TEST(Hoi4WorldMapStrategicRegionsTests, ProvincesInNoRegionAreLogged)
{
   StrategicRegions strategic_regions;

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   strategic_regions.UpdateToMatchNewStates({State(1, {.provinces = {212}})}, {});

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Province 212 had no original strategic region"));

   std::cout.rdbuf(cout_buffer);
}


TEST(Hoi4WorldMapStrategicRegionsTests, DesynchronizedInternalsCauseLogging)
{
   StrategicRegions strategic_regions({
       .original_province_to_strategic_region_map =
           {
               {144, 42},
               {169, 42},
               {76, 76},
               {42, 76},
           },
   });

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   strategic_regions.UpdateToMatchNewStates({State(1, {.provinces = {144}}), State(2, {.provinces = {169, 42, 76}})},
       {});

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Strategic region 42 was not in the list of regions."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Strategic region 76 was not in the list of regions."));

   std::cout.rdbuf(cout_buffer);
}

}  // namespace hoi4