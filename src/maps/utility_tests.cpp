#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/maps/utility.h"



TEST(MapsUtility, IntDefaultsToZero)
{
   EXPECT_EQ(maps::GetIntFromColor(commonItems::Color(std::array{0, 0, 0})), 0);
}


TEST(MapsUtility, RedIsLeftShiftedBy16)
{
   EXPECT_EQ(maps::GetIntFromColor(commonItems::Color(std::array{0x42, 0, 0})), 0x42'00'00);
}


TEST(MapsUtility, GreenIsLeftShiftedBy8)
{
   EXPECT_EQ(maps::GetIntFromColor(commonItems::Color(std::array{0, 0x42, 0})), 0x00'42'00);
}


TEST(MapsUtility, BlueIsNotShifted)
{
   EXPECT_EQ(maps::GetIntFromColor(commonItems::Color(std::array{0, 0, 0x42})), 0x00'00'42);
}