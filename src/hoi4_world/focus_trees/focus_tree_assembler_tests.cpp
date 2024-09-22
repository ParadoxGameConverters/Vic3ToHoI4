#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/focus_trees/focus_tree_assembler.h"



namespace hoi4
{

TEST(Hoi4worldFocustreesFocustreeassemblerTests, TreeIsEmptyByDefault)
{
   const FocusTree focus_tree = AssembleTree({}, "");

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
       "");

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
       "");

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{.id = "focus_one", .x_position = 0},
           Focus{.id = "focus_two", .x_position = 10},
           Focus{.id = "focus_three", .x_position = 20},
           Focus{.id = "focus_four", .x_position = 30}));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, FocusesAreToTheRightOfSharedFocuses)
{
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.focuses = {Focus{.id = "focus_one"}, Focus{.id = "focus_two"}}}},
           Role{{.shared_focuses = {"focus_three", "focus_four"}}},
       },
       "");

   EXPECT_THAT(focus_tree.shared_focuses, testing::ElementsAre("focus_three", "focus_four"));
   EXPECT_THAT(focus_tree.focuses,
       testing::ElementsAre(Focus{.id = "focus_one", .x_position = 20}, Focus{.id = "focus_two", .x_position = 30}));
}


TEST(Hoi4worldFocustreesFocustreeassemblerTests, FocusesIdsHaveTagSubstitutionApplied)
{
   const FocusTree focus_tree = AssembleTree(
       {
           Role{{.focuses = {Focus{.id = "$TAG$_focus"}}}},
       },
       "REP");

   EXPECT_TRUE(focus_tree.shared_focuses.empty());
   EXPECT_THAT(focus_tree.focuses, testing::ElementsAre(Focus{.id = "REP_focus", .x_position = 0}));
}

}  // namespace hoi4