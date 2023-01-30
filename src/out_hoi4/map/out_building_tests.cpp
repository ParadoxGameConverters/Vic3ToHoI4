#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/hoi4_building.h"
#include "src/out_hoi4/map/out_building.h"



namespace out
{

TEST(Outhoi4MapBuildingTests, BuildingIsOutput)
{
   std::stringstream output;
   hoi4::Building building(42,
       "test_type",
       {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25},
       144);

   output << building;

   EXPECT_EQ(output.str(), "42;test_type;4.25;9.25;16.25;25.25;144\n");
}


TEST(Outhoi4MapBuildingTests, PositionhasTwoDigitPrecision)
{
   std::stringstream output;
   hoi4::Building building(42,
       "test_type",
       {.x_coordinate = 4.25, .y_coordinate = 9.3333, .z_coordinate = 16.125, .rotation = 25},
       144);

   output << building;

   EXPECT_EQ(output.str(), "42;test_type;4.25;9.33;16.12;25.00;144\n");
}


TEST(Outhoi4MapBuildingTests, MissingConnectingSeaProvinceOutputAsZero)
{
   std::stringstream output;
   hoi4::Building building(42, "test_type", {.x_coordinate = 4, .y_coordinate = 9, .z_coordinate = 16, .rotation = 25});

   output << building;

   EXPECT_EQ(output.str(), "42;test_type;4.00;9.00;16.00;25.00;0\n");
}

}  // namespace out