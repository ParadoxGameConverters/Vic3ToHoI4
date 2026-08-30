#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "src/hoi4_world/map/possible_path.h"



namespace hoi4
{

TEST(Hoi4worldMapPossiblePathTests, DefaultsAreSet)  // NOLINT(cert-err58-cpp)
{
   const PossiblePath possible_path;

   EXPECT_TRUE(possible_path.GetProvinces().empty());
   EXPECT_EQ(possible_path.GetDevelopment(), 0.F);
   EXPECT_EQ(possible_path.GetCost(), 0);
}


TEST(Hoi4worldMapPossiblePathTests, InitialProvinceCanBeSet)  // NOLINT(cert-err58-cpp)
{
   const PossiblePath possible_path(1);

   EXPECT_THAT(possible_path.GetProvinces(), testing::ElementsAre(1));
   EXPECT_EQ(possible_path.GetDevelopment(), 0.F);
   EXPECT_EQ(possible_path.GetCost(), 0);
}


TEST(Hoi4worldMapPossiblePathTests, ProvincesCanBeAdded)  // NOLINT(cert-err58-cpp)
{
   PossiblePath possible_path(0);
   possible_path.AddProvince(PossiblePathProvinceType{1}, PossiblePathCostType{0});
   possible_path.AddProvince(PossiblePathProvinceType{2}, PossiblePathCostType{0});

   EXPECT_THAT(possible_path.GetProvinces(), testing::ElementsAre(0, 1, 2));
}


TEST(Hoi4worldMapPossiblePathTests, AddedProvincesIncreaseCost)  // NOLINT(cert-err58-cpp)
{
   PossiblePath possible_path(0);
   possible_path.AddProvince(PossiblePathProvinceType{1}, PossiblePathCostType{2});
   possible_path.AddProvince(PossiblePathProvinceType{2}, PossiblePathCostType{3});

   EXPECT_EQ(possible_path.GetCost(), 5);
}


TEST(Hoi4worldMapPossiblePathTests, ProvinceCanBeReplaced)  // NOLINT(cert-err58-cpp)
{
   PossiblePath possible_path(1);
   possible_path.ReplaceProvinces({3, 4});

   EXPECT_THAT(possible_path.GetProvinces(), testing::ElementsAre(3, 4));
}


TEST(Hoi4worldMapPossiblePathTests, LevelCanBeSet)  // NOLINT(cert-err58-cpp)
{
   PossiblePath possible_path(0);
   possible_path.SetDevelopment(4.2F);

   EXPECT_NEAR(possible_path.GetDevelopment(), 4.2F, 0.0001);
}


TEST(Hoi4worldMapPossiblePathTests, FirstProvinceCanBeRetrieved)  // NOLINT(cert-err58-cpp)
{
   PossiblePath possible_path(1);
   possible_path.ReplaceProvinces({3, 4});

   EXPECT_EQ(possible_path.GetFirstProvince(), 3);
}


TEST(Hoi4worldMapPossiblePathTests, LastProvinceCanBeRetrieved)  // NOLINT(cert-err58-cpp)
{
   PossiblePath possible_path(1);
   possible_path.ReplaceProvinces({3, 4});

   EXPECT_EQ(possible_path.GetLastProvince(), 4);
}


TEST(Hoi4worldMapPossiblePathTests, LessThanIsBasedOnCostAndReversed)  // NOLINT(cert-err58-cpp)
{
   PossiblePath possible_path_one(0);
   possible_path_one.AddProvince(PossiblePathProvinceType{1}, PossiblePathCostType{2});

   PossiblePath possible_path_two(0);
   possible_path_two.AddProvince(PossiblePathProvinceType{1}, PossiblePathCostType{3});

   EXPECT_LT(possible_path_two, possible_path_one);
}

}  // namespace hoi4