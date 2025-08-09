#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/is_capital_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersIsCapitalTriggerTests, IsValidReturnsValueInConstructorIfStateIsCapital)
{
   const State state(42, {.is_capital = true});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const IsCapitalTrigger true_trigger(true);
   EXPECT_TRUE(true_trigger.IsValid(context, world));

   const IsCapitalTrigger false_trigger(false);
   EXPECT_FALSE(false_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersIsCapitalTriggerTests, IsValidReturnsInverseOfValueInConstructorIfStateIsNotCapital)
{
   const State state(42, {.is_capital = false});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const IsCapitalTrigger true_trigger(false);
   EXPECT_TRUE(true_trigger.IsValid(context, world));

   const IsCapitalTrigger false_trigger(true);
   EXPECT_FALSE(false_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersIsCapitalTriggerTests, EquivalentTriggersAreEqual)
{
   const IsCapitalTrigger true_trigger(true);
   const IsCapitalTrigger true_trigger_two(true);

   const IsCapitalTrigger false_trigger(false);
   const IsCapitalTrigger false_trigger_two(false);

   EXPECT_EQ(true_trigger, true_trigger_two);
   EXPECT_EQ(false_trigger, false_trigger_two);
   EXPECT_NE(true_trigger, false_trigger);
}


TEST(Hoi4worldRolesTriggersIsCapitalTriggerTests, CopyReturnsACopy)
{
   const IsCapitalTrigger true_trigger(true);
   const std::unique_ptr<Trigger> true_copy = true_trigger.Copy();

   const IsCapitalTrigger false_trigger(false);

   EXPECT_NE(true_copy.get(), &true_trigger);
   EXPECT_EQ(*true_copy, true_trigger);
   EXPECT_NE(*true_copy, false_trigger);
}

}  // namespace hoi4