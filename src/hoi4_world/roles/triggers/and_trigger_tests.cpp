#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/and_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesRequirementsAndTriggerTests, IsValidReturnsTrueIfAllChildrenTrue)
{
   std::unique_ptr<Trigger> trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> trigger_two = std::make_unique<AlwaysTrigger>(true);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(trigger_one));
   children.push_back(std::move(trigger_two));
   const AndTrigger and_trigger(std::move(children));

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({});
   const Context context{ .root = scope, .this_scope = scope, .prev = scope, .from = scope };
   EXPECT_TRUE(and_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesRequirementsAndTriggerTests, IsValidReturnsTrueIfAnyChildrenFalse)
{
   std::unique_ptr<Trigger> trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(trigger_one));
   children.push_back(std::move(trigger_two));
   const AndTrigger and_trigger(std::move(children));

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({});
   const Context context{ .root = scope, .this_scope = scope, .prev = scope, .from = scope };
   EXPECT_FALSE(and_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesRequirementsAndTriggerTests, EquavalentTriggersAreEqual)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const AndTrigger and_trigger_one(std::move(children_one));

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(true_trigger_two));
   children_two.push_back(std::move(false_trigger_two));
   const AndTrigger and_trigger_two(std::move(children_two));

   std::unique_ptr<Trigger> false_trigger_three = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> true_trigger_three = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_three;
   children_three.push_back(std::move(false_trigger_three));
   children_three.push_back(std::move(true_trigger_three));
   const AndTrigger and_trigger_three(std::move(children_three));

   const AndTrigger and_trigger_four({});

   EXPECT_EQ(and_trigger_one, and_trigger_two);
   EXPECT_NE(and_trigger_one, and_trigger_three);
   EXPECT_NE(and_trigger_two, and_trigger_three);
   EXPECT_NE(and_trigger_one, and_trigger_four);
   EXPECT_NE(and_trigger_two, and_trigger_four);
   EXPECT_NE(and_trigger_three, and_trigger_four);
}


TEST(Hoi4worldRolesRequirementsAndTriggerTests, CopyReturnsACopy)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const AndTrigger and_trigger_one(std::move(children_one));

   const auto and_copy = and_trigger_one.Copy();
   EXPECT_NE(and_copy.get(), &and_trigger_one);
   EXPECT_EQ(*and_copy.get(), and_trigger_one);

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(false_trigger_two));
   children_two.push_back(std::move(true_trigger_two));
   const AndTrigger and_trigger_two(std::move(children_two));
   EXPECT_NE(*and_copy.get(), and_trigger_two);
}

}  // namespace hoi4