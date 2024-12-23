#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/map/buildings.h"
#include "src/out_hoi4/map/out_buildings.h"



namespace out
{


TEST(Outhoi4MapBuildingsTests, ExceptionForBadPath)
{
   EXPECT_THROW(OutputBuildings("ExceptionForBadPath", hoi4::Buildings()), std::runtime_error);
}


TEST(Outhoi4MapBuildingsTests, FilesAreCreated)
{
   commonItems::DeleteFolder("output/FilesAreCreated");
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/FilesAreCreated");
   commonItems::TryCreateFolder("output/FilesAreCreated/map");

   OutputBuildings("FilesAreCreated", hoi4::Buildings());

   EXPECT_TRUE(commonItems::DoesFileExist("output/FilesAreCreated/map/buildings.txt"));
}


TEST(Outhoi4MapBuildingsTests, BuildingsAreOutput)
{
   commonItems::DeleteFolder("output/BuildingsAreOutput");
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/BuildingsAreOutput");
   commonItems::TryCreateFolder("output/BuildingsAreOutput/map");

   OutputBuildings("BuildingsAreOutput",
       hoi4::Buildings(
           {.buildings = {
                hoi4::Building({.state_id = 42,
                    .type = "test_type",
                    .position =
                        {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25}}),
                hoi4::Building({.state_id = 144,
                    .type = "second_type",
                    .position = {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25},
                    .connecting_sea_province = 145}),

                hoi4::Building({.state_id = 169,
                    .type = "third_type",
                    .position = {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25},
                    .connecting_sea_province = 170})}}));

   ASSERT_TRUE(commonItems::DoesFileExist("output/BuildingsAreOutput/map/buildings.txt"));
   std::ifstream buildings_file("output/BuildingsAreOutput/map/buildings.txt");
   ASSERT_TRUE(buildings_file.is_open());
   std::stringstream buildings_file_stream;
   std::copy(std::istreambuf_iterator<char>(buildings_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(buildings_file_stream));
   buildings_file.close();
   EXPECT_EQ(buildings_file_stream.str(),
       "42;test_type;4.25;9.25;16.25;25.25;0\n"
       "144;second_type;4.25;9.25;16.25;25.25;145\n"
       "169;third_type;4.25;9.25;16.25;25.25;170\n");
}

}  // namespace out