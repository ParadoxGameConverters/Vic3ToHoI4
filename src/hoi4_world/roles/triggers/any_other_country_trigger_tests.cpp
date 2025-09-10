#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "or_trigger.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/any_other_country_trigger.h"
#include "src/hoi4_world/roles/triggers/tag_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersAnyOtherCountryTriggerTests, IsValidReturnsTrueIfAnyOtherCountryTrueForChildren)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<TagTrigger>("ONE");

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   const AnyOtherCountryTrigger any_other_country_trigger(std::move(children));

   const Scope from_scope = CountryScope{.country = Country({.tag = "FRO"})};
   const Scope this_scope = CountryScope{.country = Country({.tag = "ONE"})};
   const hoi4::World world({
       .countries =
           {
               {"ONE", Country(CountryOptions{.tag = "ONE"})},
               {"FRO", Country(CountryOptions{.tag = "FRO"})},
           },
   });
   const Context context{.root = from_scope, .this_scope = this_scope, .prev = from_scope, .from = from_scope};
   EXPECT_TRUE(any_other_country_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersAnyOtherCountryTriggerTests, IsValidReturnsFalseIfNoOtherCountryTrueForChildren)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<TagTrigger>("ONE");

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   const AnyOtherCountryTrigger any_other_country_trigger(std::move(children));

   const Country country({.tag = "BAD"});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({.countries = {{"BAD", Country(CountryOptions{})}}});
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   EXPECT_FALSE(any_other_country_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersAnyOtherCountryTriggerTests, IsValidReturnsFalseIfOnlySelfTrueForChildren)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<TagTrigger>("ONE");

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   const AnyOtherCountryTrigger any_other_country_trigger(std::move(children));

   const Country country({.tag = "ONE"});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({.countries = {{"BAD", Country(CountryOptions{})}, {"ONE", Country(CountryOptions{})}}});
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   EXPECT_FALSE(any_other_country_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersAnyOtherCountryTriggerTests, FindAllValidReturnsCountriesWithTrueCondition)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<TagTrigger>("ONE");
   std::unique_ptr<Trigger> two_trigger = std::make_unique<TagTrigger>("TWO");

   std::vector<std::unique_ptr<Trigger>> or_children;
   or_children.push_back(std::move(one_trigger));
   or_children.push_back(std::move(two_trigger));
   std::unique_ptr<Trigger> or_trigger = std::make_unique<OrTrigger>(std::move(or_children));

   std::vector<std::unique_ptr<Trigger>> any_other_children;
   any_other_children.push_back(std::move(or_trigger));
   const AnyOtherCountryTrigger any_other_country_trigger(std::move(any_other_children));

   const Scope from_scope = CountryScope{.country = Country({.tag = "FRO"})};
   const hoi4::World world({
       .countries =
           {
               {"ONE", Country(CountryOptions{.tag = "ONE"})},
               {"FRO", Country(CountryOptions{.tag = "FRO"})},
               {"TWO", Country(CountryOptions{.tag = "TWO"})},
           },
   });
   const Context context{.root = from_scope, .this_scope = from_scope, .prev = from_scope, .from = from_scope};
   const Scope expected_one = CountryScope{.country = world.GetCountries().at("ONE")};
   const Scope expected_two = CountryScope{.country = world.GetCountries().at("TWO")};

   EXPECT_THAT(any_other_country_trigger.FindAllValid(context, world),
       testing::ElementsAre(expected_one, expected_two));
}


TEST(Hoi4worldRolesTriggersAnyOtherCountryTriggerTests, EquivalentTriggersAreEqual)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const AnyOtherCountryTrigger and_trigger_one(std::move(children_one));

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(true_trigger_two));
   children_two.push_back(std::move(false_trigger_two));
   const AnyOtherCountryTrigger and_trigger_two(std::move(children_two));

   std::unique_ptr<Trigger> false_trigger_three = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> true_trigger_three = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_three;
   children_three.push_back(std::move(false_trigger_three));
   children_three.push_back(std::move(true_trigger_three));
   const AnyOtherCountryTrigger and_trigger_three(std::move(children_three));

   const AnyOtherCountryTrigger and_trigger_four({});

   EXPECT_EQ(and_trigger_one, and_trigger_two);
   EXPECT_NE(and_trigger_one, and_trigger_three);
   EXPECT_NE(and_trigger_two, and_trigger_three);
   EXPECT_NE(and_trigger_one, and_trigger_four);
   EXPECT_NE(and_trigger_two, and_trigger_four);
   EXPECT_NE(and_trigger_three, and_trigger_four);
}


TEST(Hoi4worldRolesTriggersAnyOtherCountryTriggerTests, CopyReturnsACopy)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const AnyOtherCountryTrigger and_trigger_one(std::move(children_one));

   const auto and_copy = and_trigger_one.Copy();
   EXPECT_NE(and_copy.get(), &and_trigger_one);
   EXPECT_EQ(*and_copy.get(), and_trigger_one);

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(false_trigger_two));
   children_two.push_back(std::move(true_trigger_two));
   const AnyOtherCountryTrigger and_trigger_two(std::move(children_two));
   EXPECT_NE(*and_copy.get(), and_trigger_two);
}

}  // namespace hoi4