#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/roles/requirements/always_trigger.h"
#include "src/hoi4_world/roles/requirements/tag_trigger.h"
#include "src/hoi4_world/roles/stories_creator.h"



namespace hoi4
{

TEST(Hoi4worldRolesStoriescreatorTests, AlwaysRolesAreAppliedToAllCountries)
{
   const Role role(RoleOptions{
       .name = "test_role",
       .requirement = std::make_unique<AlwaysTrigger>(true),
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories({{"test_role", role}},
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
           {"TWO", Country({.tag = "TWO"})},
       });

   EXPECT_THAT(stories,
       testing::UnorderedElementsAre(std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role}},
           std::pair<std::string, std::vector<hoi4::Role>>{"TWO", {role}}));
}


TEST(Hoi4worldRolesStoriescreatorTests, TagRolesAreAppliedToCountriesWithMatchingTag)
{
   const Role role(RoleOptions{
       .name = "test_role",
       .requirement = std::make_unique<TagTrigger>("TAG"),
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories({{"test_role", role}},
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
           {"TWO", Country({.tag = "TWO"})},
       });

   EXPECT_THAT(stories, testing::UnorderedElementsAre(std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role}}));
}


TEST(Hoi4worldRolesStoriescreatorTests, RolesWithNoRequirementsAreNotApplied)
{
   const Role role(RoleOptions{
       .name = "test_role",
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories({{"test_role", role}},
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
           {"TWO", Country({.tag = "TWO"})},
       });

   EXPECT_THAT(stories, testing::UnorderedElementsAre());
}


TEST(Hoi4worldRolesStoriescreatorTests, NoStoriesIfNoCountries)
{
   const Role role(RoleOptions{
       .name = "test_role",
       .requirement = std::make_unique<AlwaysTrigger>(true),
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories({{"test_role", role}}, World({}), {});

   EXPECT_THAT(stories, testing::UnorderedElementsAre());
}


TEST(Hoi4worldRolesStoriescreatorTests, NoStoriesIfNoRoles)
{
   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories({},
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
           {"TWO", Country({.tag = "TWO"})},
       });

   EXPECT_THAT(stories, testing::UnorderedElementsAre());
}


TEST(Hoi4worldRolesStoriescreatorTests, StoriesAreSortedByRoleScore)
{
   const Role role_one(RoleOptions{
       .name = "test_role_one",
       .requirement = std::make_unique<TagTrigger>("TAG"),
       .score = 100,
   });
   const Role role_two(RoleOptions{
       .name = "test_role_two",
       .requirement = std::make_unique<TagTrigger>("TWO"),
       .score = 200,
   });
   const Role role_three(RoleOptions{
       .name = "test_role_three",
       .requirement = std::make_unique<TagTrigger>("THR"),
       .score = 300,
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories(
       {
           {"test_role_one", role_one},
           {"test_role_two", role_two},
           {"test_role_three", role_three},
       },
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
           {"TWO", Country({.tag = "TWO"})},
           {"THR", Country({.tag = "THR"})},
       });

   EXPECT_THAT(stories,
       testing::UnorderedElementsAre(std::pair<std::string, std::vector<hoi4::Role>>{"THR", {role_three}},
           std::pair<std::string, std::vector<hoi4::Role>>{"TWO", {role_two}},
           std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role_one}}));
}


TEST(Hoi4worldRolesStoriescreatorTests, StoriesAreSortedByTagIfScoresAreEqual)
{
   const Role role_one(RoleOptions{
       .name = "test_role_one",
       .requirement = std::make_unique<TagTrigger>("TAG"),
       .score = 100,
   });
   const Role role_two(RoleOptions{
       .name = "test_role_two",
       .requirement = std::make_unique<TagTrigger>("TWO"),
       .score = 200,
   });
   const Role role_three(RoleOptions{
       .name = "test_role_three",
       .requirement = std::make_unique<TagTrigger>("THR"),
       .score = 300,
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories(
       {
           {"test_role_one", role_one},
           {"test_role_two", role_two},
           {"test_role_three", role_three},
       },
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
           {"TWO", Country({.tag = "TWO"})},
           {"THR", Country({.tag = "THR"})},
       });

   EXPECT_THAT(stories,
       testing::UnorderedElementsAre(std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role_one}},
           std::pair<std::string, std::vector<hoi4::Role>>{"THR", {role_three}},
           std::pair<std::string, std::vector<hoi4::Role>>{"TWO", {role_two}}));
}


TEST(Hoi4worldRolesStoriescreatorTests, StoriesAreSortedByRoleNameIfScoresAndTagsAreEqual)
{
   const Role role_one(RoleOptions{
       .name = "test_role_second",
       .requirement = std::make_unique<AlwaysTrigger>(true),
       .score = 100,
   });
   const Role role_two(RoleOptions{
       .name = "test_role_first",
       .requirement = std::make_unique<AlwaysTrigger>(true),
       .score = 100,
   });
   const Role role_three(RoleOptions{
       .name = "test_role_zzz",
       .requirement = std::make_unique<AlwaysTrigger>(true),
       .score = 100,
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories(
       {
           {"test_role_second", role_one},
           {"test_role_first", role_two},
           {"test_role_zzz", role_three},
       },
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
       });

   EXPECT_THAT(stories,
       testing::UnorderedElementsAre(
           std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role_two, role_one, role_three}}));
}


TEST(Hoi4worldRolesStoriescreatorTests, BlockersCanBlockRolesByName)
{
   const Role role(RoleOptions{
       .name = "test_role",
       .requirement = std::make_unique<AlwaysTrigger>(true),
       .blockers = {"test_role_two"},
   });
   const Role role_two(RoleOptions{
       .name = "test_role_two",
       .requirement = std::make_unique<AlwaysTrigger>(true),
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories(
       {
           {"test_role", role},
           {"test_role_two", role_two},
       },
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
       });

   EXPECT_THAT(stories, testing::UnorderedElementsAre(std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role}}));
}


TEST(Hoi4worldRolesStoriescreatorTests, BlockersCanBlockRolesByCategory)
{
   const Role role(RoleOptions{
       .name = "test_role",
       .requirement = std::make_unique<AlwaysTrigger>(true),
       .blockers = {"test_role_two_category"},
   });
   const Role role_two(RoleOptions{
       .name = "test_role_two",
       .category = "test_role_two_category",
       .requirement = std::make_unique<AlwaysTrigger>(true),
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories(
       {
           {"test_role", role},
           {"test_role_two", role_two},
       },
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
       });

   EXPECT_THAT(stories, testing::UnorderedElementsAre(std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role}}));
}


TEST(Hoi4worldRolesStoriescreatorTests, MultipleBlockersCanBlockRoles)
{
   const Role role(RoleOptions{
       .name = "test_role",
       .requirement = std::make_unique<AlwaysTrigger>(true),
       .blockers = {"test_role_two_category", "test_role_three"},
   });
   const Role role_two(RoleOptions{
       .name = "test_role_two",
       .category = "test_role_two_category",
       .requirement = std::make_unique<AlwaysTrigger>(true),
   });
   const Role role_three(RoleOptions{
       .name = "test_role_three",
       .requirement = std::make_unique<AlwaysTrigger>(true),
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories(
       {
           {"test_role", role},
           {"test_role_two", role_two},
           {"test_role_three", role_three},
       },
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
       });

   EXPECT_THAT(stories, testing::UnorderedElementsAre(std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role}}));
}


TEST(Hoi4worldRolesStoriescreatorTests, BlockersOnlyBlockWithSameCountry)
{
   const Role role(RoleOptions{
       .name = "test_role",
       .requirement = std::make_unique<TagTrigger>("TAG"),
       .score = 102,
       .blockers = {"test_role_two_category", "test_role_three"},
   });
   const Role role_two(RoleOptions{
       .name = "test_role_two",
       .category = "test_role_two_category",
       .requirement = std::make_unique<AlwaysTrigger>(true),
       .score = 101,
   });
   const Role role_three(RoleOptions{
       .name = "test_role_three",
       .requirement = std::make_unique<AlwaysTrigger>(true),
       .score = 100,
   });

   const std::map<std::string, std::vector<hoi4::Role>> stories = CreateStories(
       {
           {"test_role", role},
           {"test_role_two", role_two},
           {"test_role_three", role_three},
       },
       World({}),
       {
           {"TAG", Country({.tag = "TAG"})},
           {"TWO", Country({.tag = "TWO"})},
       });

   EXPECT_THAT(stories,
       testing::UnorderedElementsAre(std::pair<std::string, std::vector<hoi4::Role>>{"TAG", {role}},
           std::pair<std::string, std::vector<hoi4::Role>>{"TWO", {role_two, role_three}}));
}

}  // namespace hoi4