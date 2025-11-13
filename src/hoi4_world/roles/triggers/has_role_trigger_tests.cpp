#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/has_role_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRoleTriggerTests, IsValidReturnsTrueIfCountryHasRole)
{
   const HasRoleTrigger has_role_trigger("test_role");

   Country country({});
   country.SetRoles({"test_role"});
   const Scope scope = CountryScope{.country = country};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_TRUE(has_role_trigger.IsValid(context, world));
}


TEST(Hoi4worldRoleTriggerTests, IsValidReturnsFalseIfCountryLacksRole)
{
   const HasRoleTrigger has_role_trigger("test_role");

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_FALSE(has_role_trigger.IsValid(context, world));
}


TEST(Hoi4worldRoleTriggerTests, IsValidReturnsFalseIfScopeNotCountry)
{
   const HasRoleTrigger has_role_trigger("test_role");

   const State state(1, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_FALSE(has_role_trigger.IsValid(context, world));
}


TEST(Hoi4worldRoleTriggerTests, FindAllValidReturnsEmptyVector)
{
   const HasRoleTrigger has_role_trigger("test_role");

   const State state(1, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_TRUE(has_role_trigger.FindAllValid(context, world).empty());
}


TEST(Hoi4worldRoleTriggerTests, EquivalentTriggersAreEqual)
{
   const HasRoleTrigger has_role_trigger("test_role");
   const HasRoleTrigger has_role_trigger_two("test_role");
   const HasRoleTrigger two_trigger("second_role");

   EXPECT_EQ(has_role_trigger, has_role_trigger_two);
   EXPECT_NE(has_role_trigger, two_trigger);
}


TEST(Hoi4worldRoleTriggerTests, CopyReturnsACopy)
{
   const HasRoleTrigger has_role_trigger("test_role");
   const std::unique_ptr<Trigger> tag_copy = has_role_trigger.Copy();

   const HasRoleTrigger two_trigger("second_role");

   EXPECT_NE(tag_copy.get(), &has_role_trigger);
   EXPECT_EQ(*tag_copy, has_role_trigger);
   EXPECT_NE(*tag_copy, two_trigger);
}

}  // namespace hoi4