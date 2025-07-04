#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
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


TEST(Hoi4worldRolesRequirementsAlwaysTriggerTests, EquavalentTriggersAreEqual)
{
   const AlwaysTrigger true_trigger(true);
   const AlwaysTrigger true_trigger_two(true);

   const AlwaysTrigger false_trigger(false);
   const AlwaysTrigger false_trigger_two(false);

   EXPECT_EQ(true_trigger, true_trigger_two);
   EXPECT_EQ(false_trigger, false_trigger_two);
   EXPECT_NE(true_trigger, false_trigger);
}


TEST(Hoi4worldRolesRequirementsAlwaysTriggerTests, CopyReturnsACopy)
{
   const AlwaysTrigger true_trigger(true);
   const std::unique_ptr<Trigger> true_copy = true_trigger.Copy();

   const AlwaysTrigger false_trigger(false);

   EXPECT_NE(true_copy.get(), &true_trigger);
   EXPECT_EQ(*true_copy, true_trigger);
   EXPECT_NE(*true_copy, false_trigger);
}

}  // namespace hoi4