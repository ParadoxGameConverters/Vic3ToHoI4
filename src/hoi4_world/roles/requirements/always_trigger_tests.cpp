#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/requirements/always_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesRequirementsAlwaysTriggerTests, IsValidReturnsValueInConstructor)
{
   const Country country({.tag = "TAG"});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({});

   const AlwaysTrigger true_trigger(true);
   EXPECT_TRUE(true_trigger.IsValid(scope, world));

   const AlwaysTrigger false_trigger(false);
   EXPECT_FALSE(false_trigger.IsValid(scope, world));
}

}  // namespace hoi4