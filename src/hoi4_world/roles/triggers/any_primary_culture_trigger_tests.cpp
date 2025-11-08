#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/any_primary_culture_trigger.h"
#include "src/hoi4_world/roles/triggers/is_capital_trigger.h"
#include "src/hoi4_world/roles/triggers/not_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersAnyPrimaryCultureTriggerTests, IsValidReturnsTrueIfAnyPrimaryCultureTrueForChildren)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<AlwaysTrigger>(true);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   const AnyPrimaryCultureTrigger any_primary_culture_trigger(std::move(children));

   const Country country(CountryOptions{.tag = "ONE", .primary_cultures = {"culture_one", "culture_two"}});
   const Scope this_scope = CountryScope{.country = country};
   const hoi4::World world({
       .countries =
           {
               {"ONE", country},
           },
       .culture_definitions =
           {
               {"culture_one", vic3::CultureDefinition({.name = "culture_one"})},
               {"culture_two", vic3::CultureDefinition({.name = "culture_two"})},
           },
   });
   const Context context{.root = this_scope, .this_scope = this_scope, .prev = this_scope, .from = this_scope};
   EXPECT_TRUE(any_primary_culture_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersAnyPrimaryCultureTriggerTests, IsValidReturnsFalseIfNoPrimaryCultureTrueForChildren)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   const AnyPrimaryCultureTrigger any_primary_culture_trigger(std::move(children));

   const Country country({.tag = "ONE", .primary_cultures = {"culture"}});
   const Scope this_scope = CountryScope{.country = country};
   const hoi4::World world({
       .countries =
           {
               {"ONE", country},
           },
       .culture_definitions =
           {
               {"culture_one", vic3::CultureDefinition({.name = "culture_one"})},
           },
   });
   const Context context{.root = this_scope, .this_scope = this_scope, .prev = this_scope, .from = this_scope};
   EXPECT_FALSE(any_primary_culture_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersAnyPrimaryCultureTriggerTests, IsValidReturnsFalseIfNoPrimaryCultures)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<AlwaysTrigger>(true);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   const AnyPrimaryCultureTrigger any_primary_culture_trigger(std::move(children));

   const Country country({.tag = "ONE"});
   const Scope this_scope = CountryScope{.country = country};
   const hoi4::World world({
       .countries =
           {
               {"ONE", country},
           },
       .culture_definitions =
           {
               {"culture_one", vic3::CultureDefinition({.name = "culture_one"})},
           },
   });
   const Context context{.root = this_scope, .this_scope = this_scope, .prev = this_scope, .from = this_scope};
   EXPECT_FALSE(any_primary_culture_trigger.IsValid(context, world));
}


// update this test with a more specific trigger
TEST(Hoi4worldRolesTriggersAnyPrimaryCultureTriggerTests, FindAllValidReturnsCulturesWithTrueCondition)
{
   std::unique_ptr<Trigger> capital_trigger = std::make_unique<AlwaysTrigger>(true);

   std::vector<std::unique_ptr<Trigger>> any_children;
   any_children.push_back(std::move(capital_trigger));
   const AnyPrimaryCultureTrigger any_primary_culture_trigger(std::move(any_children));

   const Country country(CountryOptions{.tag = "ONE", .primary_cultures = {"culture_one", "culture_two"}});
   const hoi4::World world({
       .countries =
           {
               {"ONE", country},
           },
       .culture_definitions =
           {
               {"culture_one", vic3::CultureDefinition({.name = "culture_one"})},
               {"culture_two", vic3::CultureDefinition({.name = "culture_two"})},
           },
   });

   const Scope this_scope = CountryScope{.country = country};
   const Context context{.root = this_scope, .this_scope = this_scope, .prev = this_scope, .from = this_scope};

   const Scope expected_one = CultureScope{.culture = vic3::CultureDefinition({.name = "culture_one"})};
   const Scope expected_two = CultureScope{.culture = vic3::CultureDefinition({.name = "culture_two"})};
   EXPECT_THAT(any_primary_culture_trigger.FindAllValid(context, world),
       testing::ElementsAre(expected_one, expected_two));
}


TEST(Hoi4worldRolesTriggersAnyPrimaryCultureTriggerTests, EquivalentTriggersAreEqual)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const AnyPrimaryCultureTrigger and_trigger_one(std::move(children_one));

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(true_trigger_two));
   children_two.push_back(std::move(false_trigger_two));
   const AnyPrimaryCultureTrigger and_trigger_two(std::move(children_two));

   std::unique_ptr<Trigger> false_trigger_three = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> true_trigger_three = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_three;
   children_three.push_back(std::move(false_trigger_three));
   children_three.push_back(std::move(true_trigger_three));
   const AnyPrimaryCultureTrigger and_trigger_three(std::move(children_three));

   const AnyPrimaryCultureTrigger and_trigger_four({});

   EXPECT_EQ(and_trigger_one, and_trigger_two);
   EXPECT_NE(and_trigger_one, and_trigger_three);
   EXPECT_NE(and_trigger_two, and_trigger_three);
   EXPECT_NE(and_trigger_one, and_trigger_four);
   EXPECT_NE(and_trigger_two, and_trigger_four);
   EXPECT_NE(and_trigger_three, and_trigger_four);
}


TEST(Hoi4worldRolesTriggersAnyPrimaryCultureTriggerTests, CopyReturnsACopy)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const AnyPrimaryCultureTrigger and_trigger_one(std::move(children_one));

   const auto and_copy = and_trigger_one.Copy();
   EXPECT_NE(and_copy.get(), &and_trigger_one);
   EXPECT_EQ(*and_copy.get(), and_trigger_one);

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(false_trigger_two));
   children_two.push_back(std::move(true_trigger_two));
   const AnyPrimaryCultureTrigger and_trigger_two(std::move(children_two));
   EXPECT_NE(*and_copy.get(), and_trigger_two);
}

}  // namespace hoi4