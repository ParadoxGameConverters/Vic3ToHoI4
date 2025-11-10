#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/roles/triggers/shares_heritage_trait_with_culture_trigger.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldRolesTriggersSharesHeritageTraitWithCultureTriggerTests,
    IsValidReturnsTrueIfScopeSharesHeritageTraitWithCulture)
{
   const vic3::CultureDefinition culture({.name = "scope_culture", .heritage = "test_heritage"});
   const Scope scope = CultureScope{.culture = culture};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({
       .culture_definitions =
           {
               {"role_culture", vic3::CultureDefinition({.name = "role_culture", .heritage = "test_heritage"})},
               {"scope_culture", vic3::CultureDefinition({.name = "scope_culture", .heritage = "test_heritage"})},
           },
   });

   const SharesHeritageTraitWithCultureTrigger good_trigger("role_culture");
   EXPECT_TRUE(good_trigger.IsValid(context, world));
}


TEST(Hoi4worldRolesTriggersSharesHeritageTraitWithCultureTriggerTests, FindAllValidReturnsEmptyVector)
{
   const vic3::CultureDefinition culture({.name = "scope_culture", .heritage = "test_heritage"});
   const Scope scope = CultureScope{.culture = culture};
   const Context context{.root = scope, .this_scope = scope, .prev = scope, .from = scope};
   const hoi4::World world({
       .culture_definitions =
           {
               {"role_culture", vic3::CultureDefinition({.name = "role_culture", .heritage = "test_heritage"})},
               {"scope_culture", vic3::CultureDefinition({.name = "scope_culture", .heritage = "test_heritage"})},
           },
   });

   const SharesHeritageTraitWithCultureTrigger trigger("");
   EXPECT_TRUE(trigger.FindAllValid(context, world).empty());
}


TEST(Hoi4worldRolesTriggersSharesHeritageTraitWithCultureTriggerTests, EquivalentTriggersAreEqual)
{
   const SharesHeritageTraitWithCultureTrigger good_trigger("good_homeland");
   const SharesHeritageTraitWithCultureTrigger good_trigger_two("good_homeland");

   const SharesHeritageTraitWithCultureTrigger bad_trigger("bad_homeland");
   const SharesHeritageTraitWithCultureTrigger bad_trigger_two("bad_homeland");

   EXPECT_EQ(good_trigger, good_trigger_two);
   EXPECT_EQ(bad_trigger, bad_trigger_two);
   EXPECT_NE(good_trigger, bad_trigger);
}


TEST(Hoi4worldRolesTriggersSharesHeritageTraitWithCultureTriggerTests, CopyReturnsACopy)
{
   const SharesHeritageTraitWithCultureTrigger good_trigger("good_homeland");
   const std::unique_ptr<Trigger> true_copy = good_trigger.Copy();

   const SharesHeritageTraitWithCultureTrigger bad_trigger("bad_homeland");

   EXPECT_NE(true_copy.get(), &good_trigger);
   EXPECT_EQ(*true_copy, good_trigger);
   EXPECT_NE(*true_copy, bad_trigger);
}

}  // namespace hoi4