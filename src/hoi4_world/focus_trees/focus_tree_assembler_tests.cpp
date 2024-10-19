#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/focus_trees/focus_tree_assembler.h"



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
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.repeat_focuses =
                     {
                         RepeatFocus{.requirement =
                                         [](const Country&, const World&) {
                                            static int num_matched = 0;
                                            ++num_matched;
                                            return num_matched <= 2;
                                         },
                             .focuses = {Focus{.id = "$TARGET_TAG$_focus_one"}, Focus{.id = "$TARGET_TAG$_focus_two"}}},
                         RepeatFocus{.requirement =
                                         [](const Country&, const World&) {
                                            static int num_matched = 0;
                                            ++num_matched;
                                            return num_matched <= 2;
                                         },
                             .focuses = {Focus{.id = "$TARGET_TAG$_focus_three"},
                                 Focus{.id = "$TARGET_TAG$_focus_four"}}},
                     }}},
       },
       "",
       World({.countries = {
                  {"ONE", Country({})},
                  {"TWO", Country({})},
                  {"THR", Country({})},
              }}));

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{.id = "ONE_focus_one", .x_position = -1},
           Focus{.id = "THR_focus_one", .x_position = 1},
           Focus{.id = "ONE_focus_two", .x_position = -1},
           Focus{.id = "THR_focus_two", .x_position = 1},
           Focus{.id = "ONE_focus_four", .x_position = -1},
           Focus{.id = "THR_focus_four", .x_position = 1},
           Focus{.id = "ONE_focus_three", .x_position = -1},
           Focus{.id = "THR_focus_three", .x_position = 1}));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, RepeatFocusesHaveTargetTagSubstitutionApplied)
{
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{
               .repeat_focuses =
                   {
                       RepeatFocus{
                           .requirement =
                               [](const Country&, const World&) {
                                  return true;
                               },
                           .focuses =
                               {
                                   Focus{
                                       .id = "$TARGET_TAG$_focus_one",
                                       .prerequisites = {"$TARGET_TAG$_prerequisite"},
                                       .relative_position_id = "$TARGET_TAG$_focus",
                                       .available = "$TARGET_TAG$_available",
                                       .select_effect = "$TARGET_TAG$_select_effect",
                                       .completion_reward = "$TARGET_TAG$_completion_reward",
                                       .ai_will_do = "$TARGET_TAG$_ai_will_do",
                                   },
                               },
                       },
                   },
           }},
       },
       "",
       World({.countries = {{"ONE", Country({})}}}));

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
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.repeat_focuses =
                     {
                         RepeatFocus{.requirement =
                                         [](const Country&, const World&) {
                                            static int num_matched = 0;
                                            ++num_matched;
                                            return num_matched <= 2;
                                         },
                             .focuses = {Focus{.id = "$TARGET_TAG$_focus_one"}, Focus{.id = "$TARGET_TAG$_focus_two"}}},
                         RepeatFocus{.requirement =
                                         [](const Country&, const World&) {
                                            static int num_matched = 0;
                                            ++num_matched;
                                            return num_matched <= 3;
                                         },
                             .focuses = {Focus{.id = "$TARGET_TAG$_focus_three"},
                                 Focus{.id = "$TARGET_TAG$_focus_four"}}},
                     }}},
       },
       "",
       World({.countries = {
                  {"ONE", Country({})},
                  {"TWO", Country({})},
                  {"THR", Country({})},
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


TEST(Hoi4worldFocustreesFocustreeassemblerTests, PrerequisitesEithRepeatFocusesAreExpanded)
{
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{
               .focuses =
                   {
                       Focus{.id = "$TAG$_focus_one", .tree_starter = true},
                       Focus{.id = "$TAG$_focus_three", .prerequisites = {"repeat_focus = $TARGET_TAG$_focus_two"}},
                   },
               .repeat_focuses =
                   {
                       RepeatFocus{
                           .requirement =
                               [](const Country&, const World&) {
                                  return true;
                               },
                           .focuses =
                               {
                                   Focus{
                                       .id = "$TARGET_TAG$_focus_two",
                                       .prerequisites = {"$TAG$_focus_one"},
                                       .relative_position_id = "$TAG$_focus_one",
                                   },
                               },
                       },
                   },
           }},
       },
       "TAG",
       World({.countries = {
                  {"ONE", Country({})},
                  {"TWO", Country({})},
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

}  // namespace hoi4