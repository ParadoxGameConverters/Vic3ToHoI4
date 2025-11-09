#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/has_homeland_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersHasHomelandTriggerTests, IsValidReturnsValueInConstructorIfStateHasHomeland)
{
   const State state(42, {.homelands{"good_homeland"}});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const HasHomelandTrigger good_trigger("good_homeland");
   EXPECT_TRUE(good_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersHasHomelandTriggerTests, IsValidReturnsInverseOfValueInConstructorIfStateLacksHomeland)
{
   const State state(42, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const HasHomelandTrigger good_trigger("good_homeland");
   EXPECT_FALSE(good_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersHasHomelandTriggerTests, FindAllValidReturnsEmptyVector)
{
   const State state(42, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const HasHomelandTrigger homeland_trigger("good_homeland");
   EXPECT_TRUE(homeland_trigger.FindAllValid(context, world).empty());
}


TEST(Hoi4worldRolesTriggersHasHomelandTriggerTests, EquivalentTriggersAreEqual)
{
   const HasHomelandTrigger good_trigger("good_homeland");
   const HasHomelandTrigger good_trigger_two("good_homeland");

   const HasHomelandTrigger bad_trigger("bad_homeland");
   const HasHomelandTrigger bad_trigger_two("bad_homeland");

   EXPECT_EQ(good_trigger, good_trigger_two);
   EXPECT_EQ(bad_trigger, bad_trigger_two);
   EXPECT_NE(good_trigger, bad_trigger);
}


TEST(Hoi4worldRolesTriggersHasHomelandTriggerTests, CopyReturnsACopy)
{
   const HasHomelandTrigger good_trigger("good_homeland");
   const std::unique_ptr<Trigger> true_copy = good_trigger.Copy();

   const HasHomelandTrigger bad_trigger("bad_homeland");

   EXPECT_NE(true_copy.get(), &good_trigger);
   EXPECT_EQ(*true_copy, good_trigger);
   EXPECT_NE(*true_copy, bad_trigger);
}

}  // namespace hoi4