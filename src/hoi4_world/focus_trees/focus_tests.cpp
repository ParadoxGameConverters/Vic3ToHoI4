#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/focus_trees/focus.h"



namespace hoi4
{

TEST(Hoi4worldFocustreesFocusTests, ReplacementIsApplied)
{
   Focus focus;

   focus.id = "test_id";
   focus.icon = "test_icon";
   focus.text = "test_text";
   focus.prerequisites = {"= { test_prerequisite_1 }", "= { test_prerequisite_2 }"};
   focus.mutually_exclusive = "test_mutually_exclusive";
   focus.bypass = "= { test_bypass }";
   focus.tree_starter = true;
   focus.x_position = 3;
   focus.y_position = 5;
   focus.relative_position_id = "test_relative_position";
   focus.cost = 8;
   focus.available_if_capitulated = true;
   focus.available = "= { test_available }";
   focus.cancel_if_invalid = "test_cancel_if_invalid";
   focus.continue_if_invalid = "test_continue_if_invalid";
   focus.will_lead_to_war_with = "test_will_lead_to_war_with";
   focus.select_effect = "= { test_select_effect }";
   focus.complete_tooltip = "test_complete_tooltip";
   focus.completion_reward = "= { test_completion_reward }";
   focus.ai_will_do = "= { test_ai_will_do }";
   focus.allow_branch = "= { test_allow_branch }";

   focus.ApplyReplacement("test", "result");

   EXPECT_EQ(focus.id, "result_id");
   EXPECT_EQ(focus.icon, "result_icon");
   EXPECT_EQ(focus.text.value_or(""), "result_text");
   EXPECT_THAT(focus.prerequisites, testing::ElementsAre("= { result_prerequisite_1 }", "= { result_prerequisite_2 }"));
   EXPECT_EQ(focus.mutually_exclusive.value_or(""), "result_mutually_exclusive");
   EXPECT_EQ(focus.bypass.value_or(""), "= { result_bypass }");
   EXPECT_TRUE(focus.tree_starter);
   EXPECT_EQ(focus.x_position, 3);
   EXPECT_EQ(focus.y_position, 5);
   EXPECT_EQ(focus.relative_position_id.value_or(""), "result_relative_position");
   EXPECT_EQ(focus.cost, 8);
   EXPECT_TRUE(focus.available_if_capitulated);
   EXPECT_EQ(focus.available.value_or(""), "= { result_available }");
   EXPECT_EQ(focus.cancel_if_invalid.value_or(""), "result_cancel_if_invalid");
   EXPECT_EQ(focus.continue_if_invalid.value_or(""), "result_continue_if_invalid");
   EXPECT_EQ(focus.will_lead_to_war_with.value_or(""), "result_will_lead_to_war_with");
   EXPECT_EQ(focus.select_effect.value_or(""), "= { result_select_effect }");
   EXPECT_EQ(focus.complete_tooltip.value_or(""), "result_complete_tooltip");
   EXPECT_EQ(focus.completion_reward, "= { result_completion_reward }");
   EXPECT_EQ(focus.ai_will_do.value_or(""), "= { result_ai_will_do }");
   EXPECT_EQ(focus.allow_branch.value_or(""), "= { result_allow_branch }");
}

}  // namespace hoi4