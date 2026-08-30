#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/maps/utility.h"



namespace maps
{

TEST(MapsUtility, IntDefaultsToZero)  // NOLINT(cert-err58-cpp)
{
   EXPECT_EQ(GetIntFromColor(commonItems::Color(std::array{0, 0, 0})), 0);
}


TEST(MapsUtility, RedIsLeftShiftedBy16)  // NOLINT(cert-err58-cpp)
{
   EXPECT_EQ(GetIntFromColor(commonItems::Color(std::array{0x42, 0, 0})), 0x42'00'00);
}


TEST(MapsUtility, GreenIsLeftShiftedBy8)  // NOLINT(cert-err58-cpp)
{
   EXPECT_EQ(GetIntFromColor(commonItems::Color(std::array{0, 0x42, 0})), 0x00'42'00);
}


TEST(MapsUtility, BlueIsNotShifted)  // NOLINT(cert-err58-cpp)
{
   EXPECT_EQ(GetIntFromColor(commonItems::Color(std::array{0, 0, 0x42})), 0x00'00'42);
}

}  // namespace maps