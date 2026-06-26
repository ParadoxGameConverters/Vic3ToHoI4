#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/owner_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersOwnerTriggerTests, IsValidReturnsIfStateOwnerMatches)
{
   const State state(42, {.owner = "TAG"});
   const Scope state_scope = StateScope{.state = state};
   const Context context{.root = state_scope, .this_scope = state_scope, .prev = state_scope, .from = state_scope};
   const hoi4::World world(WorldOptions{});

   const OwnerTrigger true_trigger("TAG");
   EXPECT_TRUE(true_trigger.IsValid(context, world));

   const OwnerTrigger false_trigger("NON");
   EXPECT_FALSE(false_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersOwnerTriggerTests, IfRootSpecifiedIsValidReturnsIfStateOwnerMatchesRoot)
{
   const State state(42, {.owner = "TAG"});
   const Scope state_scope = StateScope{.state = state};
   const hoi4::World world(WorldOptions{});

   const OwnerTrigger owner_trigger("ROOT");

   const Country tag_country(CountryOptions{.tag = "TAG"});
   const Scope tag_country_scope = CountryScope{.country = tag_country};
   const Context good_context{.root = tag_country_scope,
       .this_scope = state_scope,
       .prev = state_scope,
       .from = state_scope};
   EXPECT_TRUE(owner_trigger.IsValid(good_context, world));

   const Country other_country(CountryOptions{.tag = "OTH"});
   const Scope other_country_scope = CountryScope{.country = other_country};
   const Context bad_context{.root = other_country_scope,
       .this_scope = state_scope,
       .prev = state_scope,
       .from = state_scope};
   EXPECT_FALSE(owner_trigger.IsValid(bad_context, world));
}


TEST(Hoi4worldRolesTriggersOwnerTriggerTests, FindAllValidReturnsEmptyVector)
{
   const State state(42, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const OwnerTrigger owner_trigger("");
   EXPECT_TRUE(owner_trigger.FindAllValid(context, world).empty());
}


TEST(Hoi4worldRolesTriggersOwnerTriggerTests, EquivalentTriggersAreEqual)
{
   const OwnerTrigger one_trigger("ONE");
   const OwnerTrigger one_trigger_two("ONE");

   const OwnerTrigger other_trigger("OTH");
   const OwnerTrigger other_trigger_two("OTH");

   EXPECT_EQ(one_trigger, one_trigger_two);
   EXPECT_EQ(other_trigger, other_trigger_two);
   EXPECT_NE(one_trigger, other_trigger);
}


TEST(Hoi4worldRolesTriggersOwnerTriggerTests, CopyReturnsACopy)
{
   const OwnerTrigger owner_trigger("OWN");
   const std::unique_ptr<Trigger> owner_copy = owner_trigger.Copy();

   const OwnerTrigger other_trigger("OTH");

   EXPECT_NE(owner_copy.get(), &owner_trigger);
   EXPECT_EQ(*owner_copy, owner_trigger);
   EXPECT_NE(*owner_copy, other_trigger);
}

}  // namespace hoi4