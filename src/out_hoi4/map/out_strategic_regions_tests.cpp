#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/map/strategic_regions.h"
#include "src/out_hoi4/map/out_strategic_regions.h"



using std::filesystem::create_directories;
using std::filesystem::path;
using std::filesystem::remove_all;



namespace out
{

TEST(Outhoi4MapStrategicregionsTests, FolderIsCreated)
{
   remove_all("output/FolderIsCreated/map");
   create_directories("output/FolderIsCreated/map");

   const hoi4::StrategicRegions strategic_regions;

   OutputStrategicRegions("FolderIsCreated", strategic_regions);

   EXPECT_TRUE(commonItems::DoesFolderExist(path("output/FolderIsCreated/map/strategicregions")));
}


TEST(Outhoi4MapStrategicregionsTests, StrategicRegionsAreOutput)
{
   remove_all("output/StrategicRegionsAreOutput/map");
   create_directories("output/StrategicRegionsAreOutput/map");

   const hoi4::StrategicRegions strategic_regions(
       {.strategic_regions = {{1, hoi4::StrategicRegion({.filename = "strategic_region_one.txt", .id = 1})},
            {2, hoi4::StrategicRegion({.filename = "strategic_region_two.txt", .id = 2})}}});

   OutputStrategicRegions("StrategicRegionsAreOutput", strategic_regions);

   EXPECT_TRUE(commonItems::DoesFileExist(
       path("output/StrategicRegionsAreOutput/map/strategicregions/strategic_region_one.txt")));
   EXPECT_TRUE(commonItems::DoesFileExist(
       path("output/StrategicRegionsAreOutput/map/strategicregions/strategic_region_two.txt")));
}

}  // namespace out