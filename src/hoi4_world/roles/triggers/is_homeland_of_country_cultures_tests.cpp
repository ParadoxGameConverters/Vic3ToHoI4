#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/is_homeland_of_country_cultures.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersIsHomelandOfCountryCultureTests, IsValidReturnsTrueIfStateHasCountryHomeland)
{
   const State state(42, {.homelands = {"test_culture"}});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({.countries = {{"ONE", Country({.primary_cultures = {"test_culture"}})}}});

   const IsHomelandOfCountryCulture trigger("ONE");
   EXPECT_TRUE(trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersIsHomelandOfCountryCultureTests, IsValidReturnsFalseIfCountryMissing)
{
   const State state(42, {.homelands = {"test_culture"}});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   const IsHomelandOfCountryCulture trigger("ONE");
   EXPECT_FALSE(trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersIsHomelandOfCountryCultureTests, IsValidReturnsFalseIfStateHasNotCountryHomeland)
{
   const State state(42, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({.countries = {{"ONE", Country({.primary_cultures = {"test_culture"}})}}});

   const IsHomelandOfCountryCulture trigger("ONE");
   EXPECT_FALSE(trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersIsHomelandOfCountryCultureTests, EquivalentTriggersAreEqual)
{
   const IsHomelandOfCountryCulture trigger("ONE");
   const IsHomelandOfCountryCulture same_trigger("ONE");

   const IsHomelandOfCountryCulture trigger_two("TWO");
   const IsHomelandOfCountryCulture same_trigger_two("TWO");

   EXPECT_EQ(trigger, same_trigger);
   EXPECT_EQ(trigger_two, same_trigger_two);
   EXPECT_NE(trigger, trigger_two);
}


TEST(Hoi4worldRolesTriggersIsHomelandOfCountryCultureTests, CopyReturnsACopy)
{
   const IsHomelandOfCountryCulture trigger("ONE");
   const std::unique_ptr<Trigger> true_copy = trigger.Copy();

   const IsHomelandOfCountryCulture trigger_two("TWO");

   EXPECT_NE(true_copy.get(), &trigger);
   EXPECT_EQ(*true_copy, trigger);
   EXPECT_NE(*true_copy, trigger_two);
}

}  // namespace hoi4