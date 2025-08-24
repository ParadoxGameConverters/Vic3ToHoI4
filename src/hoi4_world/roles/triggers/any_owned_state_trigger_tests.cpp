#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/any_owned_state_trigger.h"
#include "src/hoi4_world/roles/triggers/is_capital_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersAnyOwnedStateTriggerTests, IsValidReturnsTrueIfAnyOwnedStateTrueForChildren)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<IsCapitalTrigger>(true);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   const AnyOwnedStateTrigger any_other_country_trigger(std::move(children));

   const Scope this_scope = CountryScope{.country = Country({.tag = "ONE"})};
   const hoi4::World world({
       .countries =
           {
               {"ONE", Country(CountryOptions{.tag = "ONE"})},
           },
       .states = States{.states =
                            {
                                State(1, StateOptions{.owner = "ONE", .is_capital = true}),
                                State(2, StateOptions{.owner = "ONE", .is_capital = false}),
                            }},
   });
   const Context context{.root = this_scope, .this_scope = this_scope, .prev = this_scope, .from = this_scope};
   EXPECT_TRUE(any_other_country_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersAnyOwnedStateTriggerTests, IsValidReturnsFalseIfNoOwnedStateTrueForChildren)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<IsCapitalTrigger>(true);
   std::unique_ptr<Trigger> two_trigger = std::make_unique<AlwaysTrigger>(true);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   children.push_back(std::move(two_trigger));
   const AnyOwnedStateTrigger any_other_country_trigger(std::move(children));

   const Scope this_scope = CountryScope{.country = Country({.tag = "ONE"})};
   const hoi4::World world({
       .countries =
           {
               {"ONE", Country(CountryOptions{.tag = "ONE"})},
           },
       .states = States{.states = {State(1, StateOptions{.owner = "ONE", .is_capital = false})}},
   });
   const Context context{.root = this_scope, .this_scope = this_scope, .prev = this_scope, .from = this_scope};
   EXPECT_FALSE(any_other_country_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersAnyOwnedStateTriggerTests, IsValidReturnsFalseIfNoOwnedStates)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<AlwaysTrigger>(true);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(one_trigger));
   const AnyOwnedStateTrigger any_other_country_trigger(std::move(children));

   const Scope this_scope = CountryScope{.country = Country({.tag = "ONE"})};
   const hoi4::World world({
       .countries =
           {
               {"ONE", Country(CountryOptions{.tag = "ONE"})},
           },
       .states = States{.states = {State(1, StateOptions{.owner = "TWO"})}},
   });
   const Context context{.root = this_scope, .this_scope = this_scope, .prev = this_scope, .from = this_scope};
   EXPECT_FALSE(any_other_country_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersAnyOwnedStateTriggerTests, EquivalentTriggersAreEqual)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const AnyOwnedStateTrigger and_trigger_one(std::move(children_one));

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(true_trigger_two));
   children_two.push_back(std::move(false_trigger_two));
   const AnyOwnedStateTrigger and_trigger_two(std::move(children_two));

   std::unique_ptr<Trigger> false_trigger_three = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> true_trigger_three = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_three;
   children_three.push_back(std::move(false_trigger_three));
   children_three.push_back(std::move(true_trigger_three));
   const AnyOwnedStateTrigger and_trigger_three(std::move(children_three));

   const AnyOwnedStateTrigger and_trigger_four({});

   EXPECT_EQ(and_trigger_one, and_trigger_two);
   EXPECT_NE(and_trigger_one, and_trigger_three);
   EXPECT_NE(and_trigger_two, and_trigger_three);
   EXPECT_NE(and_trigger_one, and_trigger_four);
   EXPECT_NE(and_trigger_two, and_trigger_four);
   EXPECT_NE(and_trigger_three, and_trigger_four);
}


TEST(Hoi4worldRolesTriggersAnyOwnedStateTriggerTests, CopyReturnsACopy)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const AnyOwnedStateTrigger and_trigger_one(std::move(children_one));

   const auto and_copy = and_trigger_one.Copy();
   EXPECT_NE(and_copy.get(), &and_trigger_one);
   EXPECT_EQ(*and_copy.get(), and_trigger_one);

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(false_trigger_two));
   children_two.push_back(std::move(true_trigger_two));
   const AnyOwnedStateTrigger and_trigger_two(std::move(children_two));
   EXPECT_NE(*and_copy.get(), and_trigger_two);
}

}  // namespace hoi4