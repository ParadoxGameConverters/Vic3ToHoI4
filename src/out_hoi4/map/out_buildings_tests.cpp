#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/map/buildings.h"
#include "src/out_hoi4/map/out_buildings.h"



namespace out
{


TEST(Outhoi4MapBuildingsTests, ExceptionForBadPath)
{
   EXPECT_THROW(OutputBuildings("ExceptionForBadPath", hoi4::Buildings({}, {})), std::runtime_error);
}


TEST(Outhoi4MapBuildingsTests, FilesAreCreated)
{
   commonItems::DeleteFolder("output/FilesAreCreated");
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/FilesAreCreated");
   commonItems::TryCreateFolder("output/FilesAreCreated/map");

   OutputBuildings("FilesAreCreated", hoi4::Buildings({}, {}));

   EXPECT_TRUE(commonItems::DoesFileExist("output/FilesAreCreated/map/buildings.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/FilesAreCreated/map/airports.txt"));
}


TEST(Outhoi4MapBuildingsTests, BuildingsAreOutput)
{
   commonItems::DeleteFolder("output/BuildingsAreOutput");
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/BuildingsAreOutput");
   commonItems::TryCreateFolder("output/BuildingsAreOutput/map");

   OutputBuildings("BuildingsAreOutput",
       hoi4::Buildings({hoi4::Building(42,
                            "test_type",
                            {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25}),

                           hoi4::Building(144,
                               "second_type",
                               {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25},
                               145),

                           hoi4::Building(169,
                               "third_type",
                               {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25},
                               170)},
           {}));

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


TEST(Outhoi4MapBuildingsTests, AirportsAreOutput)
{
   commonItems::DeleteFolder("output/AirportsAreOutput");
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/AirportsAreOutput");
   commonItems::TryCreateFolder("output/AirportsAreOutput/map");

   OutputBuildings("AirportsAreOutput", hoi4::Buildings({}, {{2, 4}, {3, 9}, {4, 16}}));

   ASSERT_TRUE(commonItems::DoesFileExist("output/AirportsAreOutput/map/airports.txt"));
   std::ifstream airports_file("output/AirportsAreOutput/map/airports.txt");
   ASSERT_TRUE(airports_file.is_open());
   std::stringstream airports_file_stream;
   std::copy(std::istreambuf_iterator<char>(airports_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(airports_file_stream));
   airports_file.close();
   EXPECT_EQ(airports_file_stream.str(),
       "2={4 }\n"
       "3={9 }\n"
       "4={16 }\n");
}

}  // namespace out