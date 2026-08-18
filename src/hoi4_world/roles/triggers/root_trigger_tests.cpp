#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/root_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/vic3_world/cultures/culture.h"


namespace hoi4
{

TEST(Hoi4worldRolesTriggersRootTriggerTests, IsValidReturnsIfCountryMatches)
{
   const Country country({.tag = "TAG"});
   const Scope country_scope = CountryScope{.country = country};
   const Context context{.root = country_scope,
       .this_scope = country_scope,
       .prev = country_scope,
       .from = country_scope};
   const hoi4::World world(WorldOptions{});

   const RootTrigger true_trigger("TAG");
   EXPECT_TRUE(true_trigger.IsValid(context, world));

   const RootTrigger true_trigger_c("c:TAG");
   EXPECT_TRUE(true_trigger_c.IsValid(context, world));

   const RootTrigger false_trigger("NON");
   EXPECT_FALSE(false_trigger.IsValid(context, world));

   const RootTrigger false_trigger_c("c:NON");
   EXPECT_FALSE(false_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersRootTriggerTests, IsValidReturnsFalseIfRootIsState)
{
   const hoi4::World world(WorldOptions{});
   const State state(42, {.owner = "TAG"});
   const Scope state_scope = StateScope{.state = state};

   const RootTrigger trigger("TAG");

   const Country tag_country(CountryOptions{.tag = "TAG"});
   const Scope tag_country_scope = CountryScope{.country = tag_country};
   const Context context{.root = state_scope, .this_scope = state_scope, .prev = state_scope, .from = state_scope};
   EXPECT_FALSE(trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersRootTriggerTests, IsValidReturnsFalseIfRootIsCulture)
{
   const hoi4::World world(WorldOptions{});
   const vic3::CultureDefinition culture({});
   const Scope culture_scope = CultureScope{.culture = culture};

   const RootTrigger true_trigger("TAG");

   const Country tag_country(CountryOptions{.tag = "TAG"});
   const Scope tag_country_scope = CountryScope{.country = tag_country};
   const Context context{.root = culture_scope,
       .this_scope = culture_scope,
       .prev = culture_scope,
       .from = culture_scope};
   EXPECT_FALSE(true_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersRootTriggerTests, FindAllValidReturnsEmptyVector)
{
   const State state(42, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const RootTrigger owner_trigger("");
   EXPECT_TRUE(owner_trigger.FindAllValid(context, world).empty());
}


TEST(Hoi4worldRolesTriggersRootTriggerTests, EquivalentTriggersAreEqual)
{
   const RootTrigger one_trigger("ONE");
   const RootTrigger one_trigger_two("ONE");

   const RootTrigger other_trigger("OTH");
   const RootTrigger other_trigger_two("OTH");

   EXPECT_EQ(one_trigger, one_trigger_two);
   EXPECT_EQ(other_trigger, other_trigger_two);
   EXPECT_NE(one_trigger, other_trigger);
}


TEST(Hoi4worldRolesTriggersRootTriggerTests, CopyReturnsACopy)
{
   const RootTrigger owner_trigger("OWN");
   const std::unique_ptr<Trigger> owner_copy = owner_trigger.Copy();

   const RootTrigger other_trigger("OTH");

   EXPECT_NE(owner_copy.get(), &owner_trigger);
   EXPECT_EQ(*owner_copy, owner_trigger);
   EXPECT_NE(*owner_copy, other_trigger);
}

}  // namespace hoi4