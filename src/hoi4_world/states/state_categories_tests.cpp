#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/states/state_categories.h"



namespace hoi4
{

TEST(Hoi4worldStatesStatecategoriesTests, DefaultCategoryIsRural)
{
   const StateCategories categories({});

   EXPECT_EQ(categories.GetBestCategory(0), "rural");
}


TEST(Hoi4worldStatesStatecategoriesTests, CategoryCanBeSelected)
{
   const StateCategories categories({{1, "test_category"}});

   EXPECT_EQ(categories.GetBestCategory(1), "test_category");
}


TEST(Hoi4worldStatesStatecategoriesTests, CategoryLessThanOrEqualToNumSlotsIsChosen)
{
   const StateCategories categories({{1, "test_category_one"}, {3, "test_category_two"}});

   EXPECT_EQ(categories.GetBestCategory(1), "test_category_one");
   EXPECT_EQ(categories.GetBestCategory(2), "test_category_one");
}


TEST(Hoi4worldStatesStatecategoriesTests, LargestCategoryLessThanOrEqualToNumSlotsIsChosen)
{
   const StateCategories categories({{1, "test_category"}, {3, "test_category_two"}, {5, "test_category_three"}});

   EXPECT_EQ(categories.GetBestCategory(3), "test_category_two");
   EXPECT_EQ(categories.GetBestCategory(4), "test_category_two");
}

}  // namespace hoi4