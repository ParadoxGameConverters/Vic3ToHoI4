#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "and_trigger.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/requirements/always_trigger.h"
#include "src/hoi4_world/roles/requirements/or_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesRequirementsOrTriggerTests, TriggerReturnFalseIfAllChildrenFalse)
{
   std::unique_ptr<Trigger> trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(trigger_one));
   children.push_back(std::move(trigger_two));
   const OrTrigger and_trigger(std::move(children));

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({});
   EXPECT_FALSE(and_trigger.IsValid(scope, world));
}


TEST(Hoi4worldRolesRequirementsOrTriggerTests, TriggerReturnTrueIfAnyChildrenTrue)
{
   std::unique_ptr<Trigger> trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(trigger_one));
   children.push_back(std::move(trigger_two));
   const OrTrigger and_trigger(std::move(children));

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({});
   EXPECT_TRUE(and_trigger.IsValid(scope, world));
}

}  // namespace hoi4