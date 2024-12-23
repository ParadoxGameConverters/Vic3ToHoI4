#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/map/strategic_regions.h"
#include "src/out_hoi4/map/out_strategic_regions.h"



namespace out
{


TEST(Outhoi4MapStrategicregionsTests, ExceptionForBadPath)
{
   EXPECT_THROW(OutputStrategicRegions("ExceptionForBadPath", hoi4::StrategicRegions()), std::runtime_error);
}


TEST(Outhoi4MapStrategicregionsTests, FolderIsCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/FolderIsCreated");
   commonItems::TryCreateFolder("output/FolderIsCreated/map");

   const hoi4::StrategicRegions strategic_regions;

   OutputStrategicRegions("FolderIsCreated", strategic_regions);

   EXPECT_TRUE(commonItems::DoesFolderExist("output/FolderIsCreated/map/strategicregions"));
}


TEST(Outhoi4MapStrategicregionsTests, StrategicRegionsAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/StrategicRegionsAreOutput");
   commonItems::TryCreateFolder("output/StrategicRegionsAreOutput/map");

   const hoi4::StrategicRegions strategic_regions(
       {.strategic_regions = {{1, hoi4::StrategicRegion({.filename = "strategic_region_one.txt", .id = 1})},
            {2, hoi4::StrategicRegion({.filename = "strategic_region_two.txt", .id = 2})}}});

   OutputStrategicRegions("StrategicRegionsAreOutput", strategic_regions);

   EXPECT_TRUE(
       commonItems::DoesFileExist("output/StrategicRegionsAreOutput/map/strategicregions/strategic_region_one.txt"));
   EXPECT_TRUE(
       commonItems::DoesFileExist("output/StrategicRegionsAreOutput/map/strategicregions/strategic_region_two.txt"));
}

}  // namespace out