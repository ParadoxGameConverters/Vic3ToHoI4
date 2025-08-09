#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/is_on_continent_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesRequirementsIsOnContinentTriggerTests, IsValidReturnsTrueIfStateIsOnContinent)
{
   const State state(42, {.continent = "test_continent"});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const IsOnContinentTrigger continent_trigger("test_continent");
   EXPECT_TRUE(continent_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesRequirementsIsOnContinentTriggerTests, IsValidReturnsFalseIfStateIsNotOnContinent)
{
   const State state(42, {.continent = "wrong_continent"});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const IsOnContinentTrigger continent_trigger("test_continent");
   EXPECT_FALSE(continent_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesRequirementsIsOnContinentTriggerTests, EquivalentTriggersAreEqual)
{
   const IsOnContinentTrigger is_on_continent_trigger("test_continent");
   const IsOnContinentTrigger is_on_continent_trigger_two("test_continent");

   const IsOnContinentTrigger wrong_continent_trigger("wrong_continent");
   const IsOnContinentTrigger wrong_continent_trigger_two("wrong_continent");

   EXPECT_EQ(is_on_continent_trigger, is_on_continent_trigger_two);
   EXPECT_EQ(wrong_continent_trigger, wrong_continent_trigger_two);
   EXPECT_NE(is_on_continent_trigger, wrong_continent_trigger);
}


TEST(Hoi4worldRolesRequirementsIsOnContinentTriggerTests, CopyReturnsACopy)
{
   const IsOnContinentTrigger is_on_continent_trigger("test_continent");
   const std::unique_ptr<Trigger> true_copy = is_on_continent_trigger.Copy();

   const IsOnContinentTrigger wrong_continent_trigger("wrong_continent");

   EXPECT_NE(true_copy.get(), &is_on_continent_trigger);
   EXPECT_EQ(*true_copy, is_on_continent_trigger);
   EXPECT_NE(*true_copy, wrong_continent_trigger);
}

}  // namespace hoi4