#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/or_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesRequirementsOrTriggerTests, IsValidReturnsFalseIfAllChildrenFalse)
{
   std::unique_ptr<Trigger> trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(trigger_one));
   children.push_back(std::move(trigger_two));
   const OrTrigger or_trigger(std::move(children));

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});
   EXPECT_FALSE(or_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesRequirementsOrTriggerTests, IsValidReturnsTrueIfAnyChildrenTrue)
{
   std::unique_ptr<Trigger> trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(trigger_one));
   children.push_back(std::move(trigger_two));
   const OrTrigger or_trigger(std::move(children));

   const Country country({});
   const Scope scope = CountryScope{.country = country};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({});
   EXPECT_TRUE(or_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesRequirementsOrTriggerTests, EquavalentTriggersAreEqual)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const OrTrigger or_trigger_one(std::move(children_one));

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(true_trigger_two));
   children_two.push_back(std::move(false_trigger_two));
   const OrTrigger or_trigger_two(std::move(children_two));

   std::unique_ptr<Trigger> false_trigger_three = std::make_unique<AlwaysTrigger>(false);
   std::unique_ptr<Trigger> true_trigger_three = std::make_unique<AlwaysTrigger>(false);

   std::vector<std::unique_ptr<Trigger>> children_three;
   children_three.push_back(std::move(false_trigger_three));
   children_three.push_back(std::move(true_trigger_three));
   const OrTrigger or_trigger_three(std::move(children_three));

   const OrTrigger or_trigger_four({});

   EXPECT_EQ(or_trigger_one, or_trigger_two);
   EXPECT_NE(or_trigger_one, or_trigger_three);
   EXPECT_NE(or_trigger_two, or_trigger_three);
   EXPECT_NE(or_trigger_one, or_trigger_four);
   EXPECT_NE(or_trigger_two, or_trigger_four);
   EXPECT_NE(or_trigger_three, or_trigger_four);
}


TEST(Hoi4worldRolesRequirementsOrTriggerTests, CopyReturnsACopy)
{
   std::unique_ptr<Trigger> true_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_one = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(true_trigger_one));
   children_one.push_back(std::move(false_trigger_one));
   const OrTrigger or_trigger_one(std::move(children_one));

   const auto or_copy = or_trigger_one.Copy();
   EXPECT_NE(or_copy.get(), &or_trigger_one);
   EXPECT_EQ(*or_copy.get(), or_trigger_one);

   std::unique_ptr<Trigger> true_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::unique_ptr<Trigger> false_trigger_two = std::make_unique<AlwaysTrigger>(false);
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(false_trigger_two));
   children_two.push_back(std::move(true_trigger_two));
   const OrTrigger or_trigger_two(std::move(children_two));
   EXPECT_NE(*or_copy.get(), or_trigger_two);
}

}  // namespace hoi4