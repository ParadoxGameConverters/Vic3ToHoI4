#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/is_homeland_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersIsHomelandTriggerTests, IsValidReturnsTrueIfStateHasCultureHomeland)
{
   const State state(42, {.homelands = {"test_culture"}});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const IsHomelandTrigger trigger("test_culture");
   EXPECT_TRUE(trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersIsHomelandTriggerTests, IsValidReturnsFalseIfStateHasNotCultureHomeland)
{
   const State state(42, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const IsHomelandTrigger trigger("test_culture");
   EXPECT_FALSE(trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersIsHomelandTriggerTests, EquivalentTriggersAreEqual)
{
   const IsHomelandTrigger trigger("test_culture");
   const IsHomelandTrigger same_trigger("test_culture");

   const IsHomelandTrigger trigger_two("test_culture_two");
   const IsHomelandTrigger same_trigger_two("test_culture_two");

   EXPECT_EQ(trigger, same_trigger);
   EXPECT_EQ(trigger_two, same_trigger_two);
   EXPECT_NE(trigger, trigger_two);
}


TEST(Hoi4worldRolesTriggersIsHomelandTriggerTests, CopyReturnsACopy)
{
   const IsHomelandTrigger trigger("test_culture");
   const std::unique_ptr<Trigger> true_copy = trigger.Copy();

   const IsHomelandTrigger trigger_two("test_culture_two");

   EXPECT_NE(true_copy.get(), &trigger);
   EXPECT_EQ(*true_copy, trigger);
   EXPECT_NE(*true_copy, trigger_two);
}

}  // namespace hoi4