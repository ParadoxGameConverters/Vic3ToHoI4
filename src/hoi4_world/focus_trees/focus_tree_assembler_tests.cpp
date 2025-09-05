#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/focus_trees/focus_tree_assembler.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/any_other_country_trigger.h"
#include "src/hoi4_world/roles/triggers/any_owned_state_trigger.h"
#include "src/hoi4_world/roles/triggers/or_trigger.h"
#include "src/hoi4_world/roles/triggers/tag_trigger.h"



namespace hoi4
{

TEST(Hoi4worldFocustreesFocustreeassemblerTests, TreeIsEmptyByDefault)
{
   const FocusTree focus_tree = AssembleTree({}, "", World({}));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_TRUE(focus_tree.focuses.empty());
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, SharedFocusesAreAddedToTree)
{
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.shared_focuses = {"focus_one", "focus_two"}}},
           Role{{.shared_focuses = {"focus_three", "focus_four"}}},
       },
       "",
       World({}));

   EXPECT_THAT(focus_tree.shared_focuses, testing::ElementsAre("focus_one", "focus_two", "focus_three", "focus_four"));
   EXPECT_TRUE(focus_tree.focuses.empty());
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, FocusesAreAddedToTree)
{
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.focuses = {Focus{.id = "focus_one"}, Focus{.id = "focus_two"}}}},
           Role{{.focuses = {Focus{.id = "focus_three"}, Focus{.id = "focus_four"}}}},
       },
       "",
       World({}));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{.id = "focus_one"},
           Focus{.id = "focus_two"},
           Focus{.id = "focus_three"},
           Focus{.id = "focus_four"}));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, FocusesAreToTheRightOfSharedFocuses)
{
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.focuses =
                     {
                         Focus{.id = "focus_one", .tree_starter = true},
                         Focus{.id = "focus_two", .tree_starter = true},
                     }}},
           Role{{.shared_focuses = {"focus_three", "focus_four"}}},
       },
       "",
       World({}));

   EXPECT_THAT(focus_tree.shared_focuses, testing::ElementsAre("focus_three", "focus_four"));
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{.id = "focus_one", .tree_starter = true, .x_position = 20},
           Focus{.id = "focus_two", .tree_starter = true, .x_position = 30}));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, FocusesHaveTagSubstitutionApplied)
{
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{
               .focuses =
                   {
                       Focus{.id = "$TAG$_focus",
                           .relative_position_id = "$TAG$_focus",
                           .ai_will_do = "$TAG$_ai_will_do"},
                   },
           }},
       },
       "REP",
       World({}));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{.id = "REP_focus",
           .x_position = 0,
           .relative_position_id = "REP_focus",
           .ai_will_do = "REP_ai_will_do"}));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, RepeatFocusesAreAddedToTree)
{
   std::unique_ptr<Trigger> always_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(always_trigger_one));
   std::unique_ptr<Trigger> any_other_country_trigger =
       std::make_unique<AnyOtherCountryTrigger>(std::move(children_one));

   std::unique_ptr<Trigger> always_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(always_trigger_two));
   std::unique_ptr<Trigger> any_owned_state_trigger = std::make_unique<AnyOwnedStateTrigger>(std::move(children_two));

   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.repeat_focuses =
                     {
                         RepeatFocus(std::move(any_other_country_trigger),
                             {
                                 Focus{.id = "$TARGET_ID$_focus_one"},
                                 Focus{.id = "$TARGET_ID$_focus_two"},
                             }),
                         RepeatFocus(std::move(any_owned_state_trigger),
                             {
                                 Focus{.id = "$TARGET_ID$_focus_three"},
                                 Focus{.id = "$TARGET_ID$_focus_four"},
                             }),
                     }}},
       },
       "THR",
       World({
           .countries =
               {
                   {"ONE", Country({.tag = "ONE"})},
                   {"TWO", Country({.tag = "TWO"})},
                   {"THR", Country({.tag = "THR", .owned_states = {1, 2, 3}})},
               },
           .states = {.states = {State(1, {}), State(2, {}), State(3, {})}},
       }));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{.id = "ONE_focus_one", .x_position = -1},
           Focus{.id = "TWO_focus_one", .x_position = 1},
           Focus{.id = "ONE_focus_two", .x_position = -1},
           Focus{.id = "TWO_focus_two", .x_position = 1},
           Focus{.id = "1_focus_four", .x_position = -2},
           Focus{.id = "2_focus_four", .x_position = 0},
           Focus{.id = "3_focus_four", .x_position = 2},
           Focus{.id = "1_focus_three", .x_position = -2},
           Focus{.id = "2_focus_three", .x_position = 0},
           Focus{.id = "3_focus_three", .x_position = 2}));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, NoRepeatFocusesAreAddedForInvalidTag)
{
   std::unique_ptr<Trigger> always_trigger_one = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(always_trigger_one));
   std::unique_ptr<Trigger> any_other_country_trigger =
       std::make_unique<AnyOtherCountryTrigger>(std::move(children_one));

   std::unique_ptr<Trigger> always_trigger_two = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(always_trigger_two));
   std::unique_ptr<Trigger> any_owned_state_trigger = std::make_unique<AnyOwnedStateTrigger>(std::move(children_two));

   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.repeat_focuses =
                     {
                         RepeatFocus(std::move(any_other_country_trigger),
                             {
                                 Focus{.id = "$TARGET_ID$_focus_one"},
                                 Focus{.id = "$TARGET_ID$_focus_two"},
                             }),
                         RepeatFocus(std::move(any_owned_state_trigger),
                             {
                                 Focus{.id = "$TARGET_ID$_focus_three"},
                                 Focus{.id = "$TARGET_ID$_focus_four"},
                             }),
                     }}},
       },
       "BAD",
       World({
           .countries =
               {
                   {"ONE", Country({.tag = "ONE"})},
                   {"TWO", Country({.tag = "TWO"})},
                   {"THR", Country({.tag = "THR", .owned_states = {1, 2, 3}})},
               },
           .states = {.states = {State(1, {}), State(2, {}), State(3, {})}},
       }));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_TRUE(focus_tree.focuses.empty());
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, RepeatFocusesHaveTargetTagSubstitutionApplied)
{
   std::unique_ptr<Trigger> always_trigger = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> children;
   children.push_back(std::move(always_trigger));
   std::unique_ptr<Trigger> any_other_country_trigger = std::make_unique<AnyOtherCountryTrigger>(std::move(children));

   const FocusTree focus_tree = AssembleTree(
       {
           Role{{
               .repeat_focuses =
                   {
                       RepeatFocus(std::move(any_other_country_trigger),
                           {
                               Focus{
                                   .id = "$TARGET_ID$_focus_one",
                                   .prerequisites = {"$TARGET_ID$_prerequisite"},
                                   .relative_position_id = "$TARGET_ID$_focus",
                                   .available = "$TARGET_ID$_available",
                                   .select_effect = "$TARGET_ID$_select_effect",
                                   .completion_reward = "$TARGET_ID$_completion_reward",
                                   .ai_will_do = "$TARGET_ID$_ai_will_do",
                               },
                           }),
                   },
           }},
       },
       "TWO",
       World({.countries = {{"ONE", Country({.tag = "ONE"})}, {"TWO", Country({.tag = "TWO"})}}}));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{
           .id = "ONE_focus_one",
           .prerequisites = {"ONE_prerequisite"},
           .x_position = 0,
           .relative_position_id = "ONE_focus",
           .available = "ONE_available",
           .select_effect = "ONE_select_effect",
           .completion_reward = "ONE_completion_reward",
           .ai_will_do = "ONE_ai_will_do",
       }));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, RepeatFocusesAreBalancedInPosition)
{
   std::unique_ptr<Trigger> one_trigger = std::make_unique<TagTrigger>("ONE");
   std::unique_ptr<Trigger> three_trigger = std::make_unique<TagTrigger>("THR");
   std::vector<std::unique_ptr<Trigger>> countries;
   countries.push_back(std::move(one_trigger));
   countries.push_back(std::move(three_trigger));
   std::unique_ptr<Trigger> or_trigger = std::make_unique<OrTrigger>(std::move(countries));
   std::vector<std::unique_ptr<Trigger>> or_trigger_list;
   or_trigger_list.push_back(std::move(or_trigger));
   std::unique_ptr<Trigger> any_other_country_trigger_one =
       std::make_unique<AnyOtherCountryTrigger>(std::move(or_trigger_list));

   std::unique_ptr<Trigger> always_trigger = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> always_trigger_list;
   always_trigger_list.push_back(std::move(always_trigger));
   std::unique_ptr<Trigger> any_other_country_trigger_two =
       std::make_unique<AnyOtherCountryTrigger>(std::move(always_trigger_list));

   const FocusTree focus_tree = AssembleTree(
       {
           Role{{
               .repeat_focuses =
                   {
                       RepeatFocus(std::move(any_other_country_trigger_one),
                           {
                               Focus{.id = "$TARGET_ID$_focus_one"},
                               Focus{.id = "$TARGET_ID$_focus_two"},
                           }),
                       RepeatFocus(std::move(any_other_country_trigger_two),
                           {
                               Focus{.id = "$TARGET_ID$_focus_three"},
                               Focus{.id = "$TARGET_ID$_focus_four"},
                           }),
                   },
           }},
       },
       "FOR",
       World({.countries = {
                  {"ONE", Country({.tag = "ONE"})},
                  {"TWO", Country({.tag = "TWO"})},
                  {"THR", Country({.tag = "THR"})},
                  {"FOR", Country({.tag = "FOR"})},
              }}));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{.id = "ONE_focus_one", .x_position = -1},
           Focus{.id = "THR_focus_one", .x_position = 1},
           Focus{.id = "ONE_focus_two", .x_position = -1},
           Focus{.id = "THR_focus_two", .x_position = 1},
           Focus{.id = "ONE_focus_four", .x_position = -2},
           Focus{.id = "THR_focus_four", .x_position = 0},
           Focus{.id = "TWO_focus_four", .x_position = 2},
           Focus{.id = "ONE_focus_three", .x_position = -2},
           Focus{.id = "THR_focus_three", .x_position = 0},
           Focus{.id = "TWO_focus_three", .x_position = 2}));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, PrerequisitesWithRepeatFocusesAreExpanded)
{
   std::unique_ptr<Trigger> always_trigger = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> always_trigger_list;
   always_trigger_list.push_back(std::move(always_trigger));
   std::unique_ptr<Trigger> any_other_country_trigger =
       std::make_unique<AnyOtherCountryTrigger>(std::move(always_trigger_list));

   const FocusTree focus_tree = AssembleTree(
       {
           Role{{
               .focuses =
                   {
                       Focus{.id = "$TAG$_focus_one", .tree_starter = true},
                       Focus{.id = "$TAG$_focus_three", .prerequisites = {"repeat_focus = $TARGET_ID$_focus_two"}},
                   },
               .repeat_focuses =
                   {
                       RepeatFocus(std::move(any_other_country_trigger),
                           {
                               Focus{
                                   .id = "$TARGET_ID$_focus_two",
                                   .prerequisites = {"$TAG$_focus_one"},
                                   .relative_position_id = "$TAG$_focus_one",
                               },
                           }),
                   },
           }},
       },
       "TAG",
       World({.countries = {
                  {"ONE", Country({.tag = "ONE"})},
                  {"TWO", Country({.tag = "TWO"})},
                  {"TAG", Country({.tag = "TAG"})},
              }}));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(
           Focus{
               .id = "TAG_focus_one",
               .tree_starter = true,
           },
           Focus{
               .id = "TAG_focus_three",
               .prerequisites = {"focus = ONE_focus_two focus = TWO_focus_two"},
               .x_position = 1,
               .relative_position_id = "ONE_focus_two",
           },
           Focus{
               .id = "ONE_focus_two",
               .prerequisites = {"TAG_focus_one"},
               .x_position = -1,
               .relative_position_id = "TAG_focus_one",
           },
           Focus{
               .id = "TWO_focus_two",
               .prerequisites = {"TAG_focus_one"},
               .x_position = 1,
               .relative_position_id = "TAG_focus_one",
           }));
}


// The previous test implicitly covers even numbers of repeat focuses, this one does odd
TEST(Hoi4worldFocustreesFocustreeassemblerTests, FocusesAfterRepeatFocusesAreBalanced)
{
   std::unique_ptr<Trigger> always_trigger = std::make_unique<AlwaysTrigger>(true);
   std::vector<std::unique_ptr<Trigger>> always_trigger_list;
   always_trigger_list.push_back(std::move(always_trigger));
   std::unique_ptr<Trigger> any_other_country_trigger =
       std::make_unique<AnyOtherCountryTrigger>(std::move(always_trigger_list));

   const FocusTree focus_tree = AssembleTree(
       {
           Role{{
               .focuses =
                   {
                       Focus{.id = "$TAG$_focus_one", .tree_starter = true},
                       Focus{.id = "$TAG$_focus_three", .prerequisites = {"repeat_focus = $TARGET_ID$_focus_two"}},
                   },
               .repeat_focuses =
                   {
                       RepeatFocus(std::move(any_other_country_trigger),
                           {
                               Focus{
                                   .id = "$TARGET_ID$_focus_two",
                                   .prerequisites = {"$TAG$_focus_one"},
                                   .relative_position_id = "$TAG$_focus_one",
                               },
                           }),
                   },
           }},
       },
       "TAG",
       World({.countries = {
                  {"ONE", Country({.tag = "ONE"})},
                  {"TWO", Country({.tag = "TWO"})},
                  {"THR", Country({.tag = "THR"})},
                  {"TAG", Country({.tag = "TAG"})},
              }}));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(
           Focus{
               .id = "TAG_focus_one",
               .tree_starter = true,
           },
           Focus{
               .id = "TAG_focus_three",
               .prerequisites = {"focus = ONE_focus_two focus = THR_focus_two focus = TWO_focus_two"},
               .x_position = 0,
               .relative_position_id = "THR_focus_two",
           },
           Focus{
               .id = "ONE_focus_two",
               .prerequisites = {"TAG_focus_one"},
               .x_position = -2,
               .relative_position_id = "TAG_focus_one",
           },
           Focus{
               .id = "THR_focus_two",
               .prerequisites = {"TAG_focus_one"},
               .x_position = 0,
               .relative_position_id = "TAG_focus_one",
           },
           Focus{
               .id = "TWO_focus_two",
               .prerequisites = {"TAG_focus_one"},
               .x_position = 2,
               .relative_position_id = "TAG_focus_one",
           }));
}

}  // namespace hoi4