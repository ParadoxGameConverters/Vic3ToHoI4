#include <gmock/gmock-matchers.h>

#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/possible_path.h"



namespace hoi4
{


TEST(Hoi4worldMapPossiblePathTests, DefaultsAreSet)
{
   const PossiblePath possible_path;

   EXPECT_TRUE(possible_path.GetProvinces().empty());
   EXPECT_EQ(possible_path.GetDevelopment(), 0.F);
   EXPECT_EQ(possible_path.GetCost(), 0);
}

TEST(Hoi4worldMapPossiblePathTests, InitialProvinceCanBeSet)
{
   const PossiblePath possible_path(1);

   EXPECT_THAT(possible_path.GetProvinces(), testing::ElementsAre(1));
   EXPECT_EQ(possible_path.GetDevelopment(), 0.F);
   EXPECT_EQ(possible_path.GetCost(), 0);
}


TEST(Hoi4worldMapPossiblePathTests, ProvincesCanBeAdded)
{
   PossiblePath possible_path(0);
   possible_path.AddProvince(1, 0);
   possible_path.AddProvince(2, 0);

   EXPECT_THAT(possible_path.GetProvinces(), testing::ElementsAre(0, 1, 2));
}


TEST(Hoi4worldMapPossiblePathTests, AddedProvincesIncreaseCost)
{
   PossiblePath possible_path(0);
   possible_path.AddProvince(1, 2);
   possible_path.AddProvince(2, 3);

   EXPECT_EQ(possible_path.GetCost(), 5);
}


TEST(Hoi4worldMapPossiblePathTests, ProvinceCanBeReplaced)
{
   PossiblePath possible_path(1);
   possible_path.ReplaceProvinces({3, 4});

   EXPECT_THAT(possible_path.GetProvinces(), testing::ElementsAre(3, 4));
}


TEST(Hoi4worldMapPossiblePathTests, LevelCanBeSet)
{
   PossiblePath possible_path(0);
   possible_path.SetDevelopment(4.2F);

   EXPECT_NEAR(possible_path.GetDevelopment(), 4.2F, 0.0001);
}


TEST(Hoi4worldMapPossiblePathTests, FirstProvinceCanBeRetrieved)
{
   PossiblePath possible_path(1);
   possible_path.ReplaceProvinces({3, 4});

   EXPECT_EQ(possible_path.GetFirstProvince(), 3);
}


TEST(Hoi4worldMapPossiblePathTests, LastProvinceCanBeRetrieved)
{
   PossiblePath possible_path(1);
   possible_path.ReplaceProvinces({3, 4});

   EXPECT_EQ(possible_path.GetLastProvince(), 4);
}


TEST(Hoi4worldMapPossiblePathTests, LessThanIsBasedOnCostAndReversed)
{
   PossiblePath possible_path_one(0);
   possible_path_one.AddProvince(1, 2);

   PossiblePath possible_path_two(0);
   possible_path_two.AddProvince(1, 3);

   EXPECT_LT(possible_path_two, possible_path_one);
}

}  // namespace hoi4