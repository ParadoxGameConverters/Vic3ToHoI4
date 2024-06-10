#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/focus_trees/focus_importer.h"



namespace hoi4
{

TEST(Hoi4worldFocustreesFocusimporterTests, DefaultsAreDefaulted)
{
   std::stringstream input;

   FocusImporter importer;
   const Focus focus = importer.ImportFocus(input);

   EXPECT_TRUE(focus.id.empty());
   EXPECT_TRUE(focus.icon.empty());
   EXPECT_FALSE(focus.text.has_value());
   EXPECT_TRUE(focus.prerequisites.empty());
   EXPECT_FALSE(focus.mutually_exclusive.has_value());
   EXPECT_FALSE(focus.bypass.has_value());
   EXPECT_EQ(focus.x_position, 0);
   EXPECT_EQ(focus.y_position, 0);
   EXPECT_FALSE(focus.relative_position_id.has_value());
   EXPECT_EQ(focus.cost, 0);
   EXPECT_FALSE(focus.available_if_capitulated);
   EXPECT_FALSE(focus.available.has_value());
   EXPECT_FALSE(focus.cancel_if_invalid.has_value());
   EXPECT_FALSE(focus.continue_if_invalid.has_value());
   EXPECT_FALSE(focus.select_effect.has_value());
   EXPECT_FALSE(focus.complete_tooltip.has_value());
   EXPECT_TRUE(focus.completion_reward.empty());
   EXPECT_FALSE(focus.ai_will_do.has_value());
   EXPECT_FALSE(focus.allow_branch.has_value());
}


TEST(Hoi4worldFocustreesFocusimporterTests, ItemsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tid = test_id\n";
   input << "\ticon = test_icon\n";
   input << "\ttext = test_text\n";
   input << "\tprerequisite = test_prerequisite_1\n";
   input << "\tprerequisite = test_prerequisite_2\n";
   input << "\tmutually_exclusive = test_mutually_exclusive\n";
   input << "\tbypass = test_bypass\n";
   input << "\tx = 3\n";
   input << "\ty = 5\n";
   input << "\trelative_position_id = test_relative_position\n";
   input << "\tcost = 8\n";
   input << "\tavailable_if_capitulated = yes\n";
   input << "\tavailable = test_available\n";
   input << "\tcancel_if_invalid = test_cancel_if_invalid\n";
   input << "\tcontinue_if_invalid = test_continue_if_invalid\n";
   input << "\tselect_effect = test_select_effect\n";
   input << "\tcomplete_tooltip = test_complete_tooltip\n";
   input << "\tcompletion_reward = test_completion_reward\n";
   input << "\tai_will_do = test_ai_will_do\n";
   input << "\tallow_branch = { test_allow_branch }\n";
   input << "\t}\n";
   input << "}";

   FocusImporter importer;
   const Focus focus = importer.ImportFocus(input);

   EXPECT_EQ(focus.id, "test_id");
   EXPECT_EQ(focus.icon, "test_icon");
   EXPECT_TRUE(focus.text.has_value());
   EXPECT_EQ(focus.text.value_or(""), "test_text");
   EXPECT_THAT(focus.prerequisites, testing::ElementsAre("test_prerequisite_1", "test_prerequisite_2"));
   EXPECT_TRUE(focus.mutually_exclusive.has_value());
   EXPECT_EQ(focus.mutually_exclusive.value_or(""), "test_mutually_exclusive");
   EXPECT_EQ(focus.bypass.value_or(""), "test_bypass");
   EXPECT_EQ(focus.x_position, 3);
   EXPECT_EQ(focus.y_position, 5);
   EXPECT_EQ(focus.relative_position_id.value_or(""), "test_relative_position");
   EXPECT_EQ(focus.cost, 8);
   EXPECT_TRUE(focus.available_if_capitulated);
   EXPECT_EQ(focus.available.value_or(""), "test_available");
   EXPECT_EQ(focus.cancel_if_invalid.value_or(""), "test_cancel_if_invalid");
   EXPECT_EQ(focus.continue_if_invalid.value_or(""), "test_continue_if_invalid");
   EXPECT_EQ(focus.select_effect.value_or(""), "test_select_effect");
   EXPECT_EQ(focus.complete_tooltip.value_or(""), "test_complete_tooltip");
   EXPECT_EQ(focus.completion_reward, "test_completion_reward");
   EXPECT_EQ(focus.ai_will_do.value_or(""), "test_ai_will_do");
   EXPECT_EQ(focus.allow_branch.value_or(""), "= { test_allow_branch }");
}

}  // namespace hoi4