#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/decisions/decisions_category.h"
#include "src/hoi4_world/decisions/decisions_category_importer.h"


namespace hoi4
{

TEST(Hoi4worldDecisionsDecisionscategoryimporterTests, DefaultsAreDefaulted)
{
   std::stringstream input;
   input << "= {\n";
   input << "}";
   const DecisionsCategory decisions_category =
       hoi4::DecisionsCategoryImporter{}.GetDecisionsCategory("test_name", input);

   EXPECT_EQ(decisions_category.name, "test_name");
   EXPECT_TRUE(decisions_category.icon.empty());
   EXPECT_TRUE(decisions_category.picture.empty());
   EXPECT_FALSE(decisions_category.priority.has_value());
   EXPECT_TRUE(decisions_category.allowed.empty());
   EXPECT_TRUE(decisions_category.visible.empty());
   EXPECT_TRUE(decisions_category.visibility_type.empty());
   EXPECT_TRUE(decisions_category.extra_items.empty());
}


TEST(HoI4World_Decisions_DecisionsCategoryTests, ItemsCanBeSet)
{
   std::stringstream input;
   input << "= {\n";
   input << "\ticon = generic_research\n";
   input << "\tpicture = GFX_decision_cat_picture_naval_treaties\n";
   input << "\tpriority = 100\n";
   input << "\tallowed = {\n";
   input << "\t\thas_dlc = \"Man the Guns\"\n";
   input << "\t}\n";
   input << "\tvisible = {\n";
   input << "\t\tis_debug = yes\n";
   input << "\t}\n";
   input << "\tvisibility_type = map_and_decisions_view\n";
   input << "\textra_item_one = {\n";
   input << "\t}\n";
   input << "\textra_item_two = {\n";
   input << "\t\tcan_it_have_stuff_inside = yes\n";
   input << "\t}\n";
   input << "}";
   const DecisionsCategory decisions_category =
       hoi4::DecisionsCategoryImporter{}.GetDecisionsCategory("category_name", input);

   EXPECT_EQ(decisions_category.name, "category_name");
   EXPECT_EQ(decisions_category.icon, "generic_research");
   EXPECT_EQ(decisions_category.picture, "GFX_decision_cat_picture_naval_treaties");
   EXPECT_EQ(decisions_category.priority.value_or(0), 100);
   EXPECT_EQ(decisions_category.allowed,
       "= {\n"
       "\t\thas_dlc = \"Man the Guns\"\n"
       "\t}");
   EXPECT_EQ(decisions_category.visible,
       "= {\n"
       "\t\tis_debug = yes\n"
       "\t}");
   EXPECT_EQ(decisions_category.visibility_type, "= map_and_decisions_view");
   EXPECT_THAT(decisions_category.extra_items,
       testing::ElementsAre(std::pair{"extra_item_one",
                                "= {\n"
                                "\t}"},
           std::pair{"extra_item_two",
               "= {\n"
               "\t\tcan_it_have_stuff_inside = yes\n"
               "\t}"}));
}

}  // namespace hoi4