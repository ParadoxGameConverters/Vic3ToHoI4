#include <sstream>

#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/focus_trees/focus.h"
#include "src/out_hoi4/focus_trees/out_focus.h"



namespace out
{

TEST(Outhoi4FocustreesOutfocusTests, DefaultsAreOutput)
{
   std::stringstream out;

   const hoi4::Focus focus;
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, IdIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.id = "test_id"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = test_id\n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, IconIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.icon = "test_icon"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = test_icon\n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, TextIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.text = "test_text"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\ttext = test_text\n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, PrerequisitesAreOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.prerequisites = {"= { prerequisite_one }", "= { prerequisite_two }"}};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tprerequisite = { prerequisite_one }\n";
   expected << "\t\tprerequisite = { prerequisite_two }\n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, MutuallyExclusiveIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.mutually_exclusive = "= { test_me }"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tmutually_exclusive = { test_me }\n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, BypassIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.bypass = "= { test_bypass }"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tbypass = { test_bypass }\n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, PositionsAreOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.x_position = 3, .y_position = 5};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 3\n";
   expected << "\t\ty = 5\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, RelativePositionIdIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.relative_position_id = "test_id"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\trelative_position_id = test_id\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, CostIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.cost = 7};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 7\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, AvailableIfCapitulatedIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.available_if_capitulated = true};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tavailable_if_capitulated = yes\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, AvailableIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.available = "= { test_available }"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tavailable = { test_available }\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, CancelIfInvalidIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.cancel_if_invalid = "test_cii"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcancel_if_invalid = test_cii\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, ContinueIfInvalidIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.continue_if_invalid = "test_cii"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcontinue_if_invalid = test_cii\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, SelectEffectIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.select_effect = "= { test_se }"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tselect_effect = { test_se }\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, CompleteTooltipIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.complete_tooltip = "= { test_ct }"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcomplete_tooltip = { test_ct }\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, CompletionRewardIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.completion_reward = "= { test_cr }"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward = { test_cr }\n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, AiWillDoIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.ai_will_do = "= { test_awd }"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t\tai_will_do = { test_awd }\n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}


TEST(Outhoi4FocustreesOutfocusTests, AllowBranchIsOutput)
{
   std::stringstream out;

   const hoi4::Focus focus{.allow_branch = "= { test_ab }"};
   out << focus;

   std::stringstream expected;
   expected << "\tfocus = {\n";
   expected << "\t\tid = \n";
   expected << "\t\ticon = \n";
   expected << "\t\tx = 0\n";
   expected << "\t\ty = 0\n";
   expected << "\t\tcost = 0\n";
   expected << "\t\tcompletion_reward \n";
   expected << "\t\tallow_branch = { test_ab }\n";
   expected << "\t}\n";

   EXPECT_EQ(out.str(), expected.str());
}

}  // namespace out