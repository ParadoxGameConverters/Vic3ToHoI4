#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "not_trigger.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/requirements/always_trigger.h"
#include "src/hoi4_world/roles/requirements/not_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesRequirementsNotTriggerTests, IsValidReturnsReturnTrueIfAllChildrenFalse)
{
   std::unique_ptr<Trigger> trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(trigger_one));
   children.push_back(std::move(trigger_two));
   const NotTrigger not_trigger(std::move(children));

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({});
   EXPECT_TRUE(not_trigger.IsValid(scope, world));
}


TEST(Hoi4worldRolesRequirementsNotTriggerTests, IsValidReturnsFalseIfAnyChildrenTrue)
{
   std::unique_ptr<Trigger> trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(trigger_one));
   children.push_back(std::move(trigger_two));
   const NotTrigger not_trigger(std::move(children));

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const hoi4::World world({});
   EXPECT_FALSE(not_trigger.IsValid(scope, world));
}


TEST(Hoi4worldRolesRequirementsNotTriggerTests, EquivalentTriggersAreEqual)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const NotTrigger not_trigger_one(std::move(children_one));

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(true_trigger_two));
   children_two.push_back(std::move(false_trigger_two));
   const NotTrigger not_trigger_two(std::move(children_two));

   std::unique_ptr<Trigger> false_trigger_three = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> true_trigger_three = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_three;
   children_three.push_back(std::move(false_trigger_three));
   children_three.push_back(std::move(true_trigger_three));
   const NotTrigger not_trigger_three(std::move(children_three));

   const NotTrigger not_trigger_four({});

   EXPECT_EQ(not_trigger_one, not_trigger_two);
   EXPECT_NE(not_trigger_one, not_trigger_three);
   EXPECT_NE(not_trigger_two, not_trigger_three);
   EXPECT_NE(not_trigger_one, not_trigger_four);
   EXPECT_NE(not_trigger_two, not_trigger_four);
   EXPECT_NE(not_trigger_three, not_trigger_four);
}


TEST(Hoi4worldRolesRequirementsNotTriggerTests, CopyReturnsACopy)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const NotTrigger not_trigger_one(std::move(children_one));

   const auto not_copy = not_trigger_one.Copy();
   EXPECT_NE(not_copy.get(), &not_trigger_one);
   EXPECT_EQ(*not_copy.get(), not_trigger_one);

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(false_trigger_two));
   children_two.push_back(std::move(true_trigger_two));
   const NotTrigger not_trigger_two(std::move(children_two));
   EXPECT_NE(*not_copy.get(), not_trigger_two);
}

}  // namespace hoi4