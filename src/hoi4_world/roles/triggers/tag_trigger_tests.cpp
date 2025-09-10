#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/tag_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersTagTriggerTests, IsValidReturnsTrueIfCountryTagMatches)
{
   const TagTrigger tag_trigger("TAG");

   const Country country({.tag = "TAG"});
   const Scope scope = CountryScope{.country = country};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_TRUE(tag_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersTagTriggerTests, IsValidReturnsFalseIfCountryTagNotMatches)
{
   const TagTrigger tag_trigger("TAG");

   const Country country({.tag = "TWO"});
   const Scope scope = CountryScope{.country = country};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_FALSE(tag_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersTagTriggerTests, IsValidReturnsFalseIfScopeNotCountry)
{
   const TagTrigger tag_trigger("TAG");

   const State state(1, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_FALSE(tag_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersTagTriggerTests, FindAllValidReturnsEmptyVector)
{
   const TagTrigger tag_trigger("TAG");

   const State state(1, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_TRUE(tag_trigger.FindAllValid(context, world).empty());
}


TEST(Hoi4worldRolesTriggersTagTriggerTests, EquivalentTriggersAreEqual)
{
   const TagTrigger tag_trigger("TAG");
   const TagTrigger tag_trigger_two("TAG");
   const TagTrigger two_trigger("TWO");

   EXPECT_EQ(tag_trigger, tag_trigger_two);
   EXPECT_NE(tag_trigger, two_trigger);
}


TEST(Hoi4worldRolesTriggersTagTriggerTests, CopyReturnsACopy)
{
   const TagTrigger tag_trigger("TAG");
   const std::unique_ptr<Trigger> tag_copy = tag_trigger.Copy();

   const TagTrigger two_trigger("TWO");

   EXPECT_NE(tag_copy.get(), &tag_trigger);
   EXPECT_EQ(*tag_copy, tag_trigger);
   EXPECT_NE(*tag_copy, two_trigger);
}

}  // namespace hoi4