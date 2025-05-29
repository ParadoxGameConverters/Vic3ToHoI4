#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/requirements/tag_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesRequirementsTagTriggerTests, TriggerReturnsTrueIfCountryTagMatches)
{
   const TagTrigger tag_trigger("TAG");

   const Country country({.tag = "TAG"});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({});

   EXPECT_TRUE(tag_trigger.IsValid(scope, world));
}


TEST(Hoi4worldRolesRequirementsTagTriggerTests, TriggerReturnsFalseIfCountryTagNotMatches)
{
   const TagTrigger tag_trigger("TAG");

   const Country country({.tag = "TWO"});
   const Scope scope = CountryScope{ .country = country };
   const hoi4::World world({});

   EXPECT_FALSE(tag_trigger.IsValid(scope, world));
}


TEST(Hoi4worldRolesRequirementsTagTriggerTests, TriggerReturnsFalseIfScopeNotCountry)
{
   const TagTrigger tag_trigger("TAG");

   const State state(1, {});
   const Scope scope = StateScope{ .state = state };
   const hoi4::World world({});

   EXPECT_FALSE(tag_trigger.IsValid(scope, world));
}

}  // namespace hoi4