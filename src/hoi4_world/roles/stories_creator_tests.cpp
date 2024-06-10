#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/roles/stories_creator.h"



namespace hoi4
{

TEST(Hoi4worldRolesStoriescreatorTests, AlwaysRolesAreAppliedToAllCountries)
{
   const Role role({
       .name = "test_role",
       .requirements = "always = yes",
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories({{"test_role", role}},
       {
           {"TAG", Country({})},
           {"TWO", Country({})},
       });

   EXPECT_THAT(stories,
       testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TAG", role},
           std::pair<std::string, hoi4::Role>{"TWO", role}));
}


TEST(Hoi4worldRolesStoriescreatorTests, TagRolesAreAppliedToCountriesWithMatchingTag)
{
   const Role role({
       .name = "test_role",
       .requirements = "tag = TAG",
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories({{"test_role", role}},
       {
           {"TAG", Country({})},
           {"TWO", Country({})},
       });

   EXPECT_THAT(stories, testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TAG", role}));
}


TEST(Hoi4worldRolesStoriescreatorTests, PrimaryCultureRolesAreAppliedToCountriesWithMatchingPrimaryCulture)
{
   const Role role({
       .name = "test_role",
       .requirements = "country_has_primary_culture = test_culture",
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories({{"test_role", role}},
       {
           {"TAG", Country({.primary_cultures = {"non_match_culture"}})},
           {"TWO", Country({.primary_cultures = {"test_culture"}})},
       });

   EXPECT_THAT(stories, testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TWO", role}));
}


TEST(Hoi4worldRolesStoriescreatorTests, RolesWithNoRequirementsAreNotApplied)
{
   const Role role({
       .name = "test_role",
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories({{"test_role", role}},
       {
           {"TAG", Country({})},
           {"TWO", Country({})},
       });

   EXPECT_THAT(stories, testing::ElementsAre());
}


TEST(Hoi4worldRolesStoriescreatorTests, NoStoriesIfNoCountries)
{
   const Role role({
       .name = "test_role",
       .requirements = "always = yes",
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories({{"test_role", role}}, {});

   EXPECT_THAT(stories, testing::ElementsAre());
}


TEST(Hoi4worldRolesStoriescreatorTests, NoStoriesIfNoRoles)
{
   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories({},
       {
           {"TAG", Country({})},
           {"TWO", Country({})},
       });

   EXPECT_THAT(stories, testing::ElementsAre());
}


TEST(Hoi4worldRolesStoriescreatorTests, StoriesAreSortedByRoleScore)
{
   const Role role_one({
       .name = "test_role_one",
       .requirements = "tag = TAG",
       .score = 100,
   });
   const Role role_two({
       .name = "test_role_two",
       .requirements = "tag = TWO",
       .score = 200,
   });
   const Role role_three({
       .name = "test_role_three",
       .requirements = "tag = THR",
       .score = 300,
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories(
       {
           {"test_role_one", role_one},
           {"test_role_two", role_two},
           {"test_role_three", role_three},
       },
       {
           {"TAG", Country({})},
           {"TWO", Country({})},
           {"THR", Country({})},
       });

   EXPECT_THAT(stories,
       testing::ElementsAre(std::pair<std::string, hoi4::Role>{"THR", role_three},
           std::pair<std::string, hoi4::Role>{"TWO", role_two},
           std::pair<std::string, hoi4::Role>{"TAG", role_one}));
}


TEST(Hoi4worldRolesStoriescreatorTests, StoriesAreSortedByTagIfScoresAreEqual)
{
   const Role role_one({
       .name = "test_role_one",
       .requirements = "tag = TAG",
       .score = 100,
   });
   const Role role_two({
       .name = "test_role_two",
       .requirements = "tag = TWO",
       .score = 100,
   });
   const Role role_three({
       .name = "test_role_three",
       .requirements = "tag = THR",
       .score = 100,
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories(
       {
           {"test_role_one", role_one},
           {"test_role_two", role_two},
           {"test_role_three", role_three},
       },
       {
           {"TAG", Country({})},
           {"TWO", Country({})},
           {"THR", Country({})},
       });

   EXPECT_THAT(stories,
       testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TAG", role_one},
           std::pair<std::string, hoi4::Role>{"THR", role_three},
           std::pair<std::string, hoi4::Role>{"TWO", role_two}));
}


TEST(Hoi4worldRolesStoriescreatorTests, StoriesAreSortedByRoleNameIfScoresAndTagsAreEqual)
{
   const Role role_one({
       .name = "test_role_second",
       .requirements = "always = yes",
       .score = 100,
   });
   const Role role_two({
       .name = "test_role_first",
       .requirements = "always = yes",
       .score = 100,
   });
   const Role role_three({
       .name = "test_role_zzz",
       .requirements = "always = yes",
       .score = 100,
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories(
       {
           {"test_role_second", role_one},
           {"test_role_first", role_two},
           {"test_role_zzz", role_three},
       },
       {
           {"TAG", Country({})},
       });

   EXPECT_THAT(stories,
       testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TAG", role_two},
           std::pair<std::string, hoi4::Role>{"TAG", role_one},
           std::pair<std::string, hoi4::Role>{"TAG", role_three}));
}


TEST(Hoi4worldRolesStoriescreatorTests, BlockersCanBlockRolesByName)
{
   const Role role({
       .name = "test_role",
       .requirements = "always = yes",
       .blockers = {"test_role_two"},
   });
   const Role role_two({
       .name = "test_role_two",
       .requirements = "always = yes",
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories(
       {
           {"test_role", role},
           {"test_role_two", role_two},
       },
       {
           {"TAG", Country({})},
       });

   EXPECT_THAT(stories, testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TAG", role}));
}


TEST(Hoi4worldRolesStoriescreatorTests, BlockersCanBlockRolesByCategory)
{
   const Role role({
       .name = "test_role",
       .requirements = "always = yes",
       .blockers = {"test_role_two_category"},
   });
   const Role role_two({
       .name = "test_role_two",
       .category = "test_role_two_category",
       .requirements = "always = yes",
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories(
       {
           {"test_role", role},
           {"test_role_two", role_two},
       },
       {
           {"TAG", Country({})},
       });

   EXPECT_THAT(stories, testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TAG", role}));
}


TEST(Hoi4worldRolesStoriescreatorTests, MultipleBlockersCanBlockRoles)
{
   const Role role({
       .name = "test_role",
       .requirements = "always = yes",
       .blockers = {"test_role_two_category", "test_role_three"},
   });
   const Role role_two({
       .name = "test_role_two",
       .category = "test_role_two_category",
       .requirements = "always = yes",
   });
   const Role role_three({
       .name = "test_role_three",
       .requirements = "always = yes",
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories(
       {
           {"test_role", role},
           {"test_role_two", role_two},
           {"test_role_three", role_three},
       },
       {
           {"TAG", Country({})},
       });

   EXPECT_THAT(stories, testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TAG", role}));
}


TEST(Hoi4worldRolesStoriescreatorTests, BlockersOnlyBlockWithSameCountry)
{
   const Role role({
       .name = "test_role",
       .requirements = "tag = TAG",
       .score = 102,
       .blockers = {"test_role_two_category", "test_role_three"},
   });
   const Role role_two({
       .name = "test_role_two",
       .category = "test_role_two_category",
       .requirements = "always = yes",
       .score = 101,
   });
   const Role role_three({
       .name = "test_role_three",
       .requirements = "always = yes",
       .score = 100,
   });

   const std::vector<std::pair<std::string, hoi4::Role>> stories = CreateStories(
       {
           {"test_role", role},
           {"test_role_two", role_two},
           {"test_role_three", role_three},
       },
       {
           {"TAG", Country({})},
           {"TWO", Country({})},
       });

   EXPECT_THAT(stories,
       testing::ElementsAre(std::pair<std::string, hoi4::Role>{"TAG", role},
           std::pair<std::string, hoi4::Role>{"TWO", role_two},
           std::pair<std::string, hoi4::Role>{"TWO", role_three}));
}

}  // namespace hoi4