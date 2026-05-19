#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/country_has_primary_culture_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersCountryHasPrimaryCultureTriggerTests, IsValidReturnsTrueIfCountryHasMatchingPrimaryCulture)
{
   const CountryHasPrimaryCultureTrigger country_has_primary_culture_trigger("cu:test_culture");

   const Country country({.primary_cultures{"cu:test_culture"}});
   const Scope scope = CountryScope{.country = country};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_TRUE(country_has_primary_culture_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersCountryHasPrimaryCultureTriggerTests,
    IsValidReturnsFalseIfCountryLacksMatchingPrimaryCulture)
{
   const CountryHasPrimaryCultureTrigger country_has_primary_culture_trigger("cu:test_culture");

   const Country country({.primary_cultures{"cu:other_culture"}});
   const Scope scope = CountryScope{.country = country};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_FALSE(country_has_primary_culture_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersCountryHasPrimaryCultureTriggerTests, IsValidReturnsFalseIfScopeNotCountry)
{
   const CountryHasPrimaryCultureTrigger country_has_primary_culture_trigger("cu:test_culture");

   const State state(1, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_FALSE(country_has_primary_culture_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersCountryHasPrimaryCultureTriggerTests, FindAllValidReturnsEmptyVector)
{
   const CountryHasPrimaryCultureTrigger country_has_primary_culture_trigger("cu:test_culture");

   const State state(1, {});
   const Scope scope = StateScope{.state = state};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});

   EXPECT_TRUE(country_has_primary_culture_trigger.FindAllValid(context, world).empty());
}


TEST(Hoi4worldRolesTriggersCountryHasPrimaryCultureTriggerTests, EquivalentTriggersAreEqual)
{
   const CountryHasPrimaryCultureTrigger country_has_primary_culture_trigger("cu:test_culture");
   const CountryHasPrimaryCultureTrigger country_has_primary_culture_trigger_two("cu:test_culture");
   const CountryHasPrimaryCultureTrigger two_trigger("cu:other_test_culture");

   EXPECT_EQ(country_has_primary_culture_trigger, country_has_primary_culture_trigger_two);
   EXPECT_NE(country_has_primary_culture_trigger, two_trigger);
}


TEST(Hoi4worldRolesTriggersCountryHasPrimaryCultureTriggerTests, CopyReturnsACopy)
{
   const CountryHasPrimaryCultureTrigger country_has_primary_culture_trigger("cu:test_culture");
   const std::unique_ptr<Trigger> trigger_copy = country_has_primary_culture_trigger.Copy();

   const CountryHasPrimaryCultureTrigger two_trigger("cu:other_test_culture");

   EXPECT_NE(trigger_copy.get(), &country_has_primary_culture_trigger);
   EXPECT_EQ(*trigger_copy, country_has_primary_culture_trigger);
   EXPECT_NE(*trigger_copy, two_trigger);
}

}  // namespace hoi4