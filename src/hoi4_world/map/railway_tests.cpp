#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "src/hoi4_world/map/railway.h"



namespace hoi4
{

TEST(Hoi4worldMapRailwayTests, LevelIsAsSet)  // NOLINT(cert-err58-cpp)
{
   const Railway railway(2, {1, 2});

   EXPECT_EQ(railway.GetLevel(), 2);
}


TEST(Hoi4worldMapRailwayTests, ExceptionForTooFewProvinces)  // NOLINT(cert-err58-cpp)
{
   EXPECT_THROW(Railway railway(42, {}), std::runtime_error);
}


TEST(Hoi4worldMapRailwayTests, ProvincesAreAsSet)  // NOLINT(cert-err58-cpp)
{
   const Railway railway(42, {1, 2, 3, 4, 5});
   EXPECT_THAT(railway.GetProvinces(), testing::ElementsAre(1, 2, 3, 4, 5));
}


TEST(Hoi4worldMapRailwayTests, LengthIsEqualToNumberOfProvinces)  // NOLINT(cert-err58-cpp)
{
   const Railway railway(42, {1, 2, 3, 4, 5});
   EXPECT_EQ(railway.GetLength(), 5);
}

}  // namespace hoi4