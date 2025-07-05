#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/roles/role.h"
#include "src/hoi4_world/roles/role_importer.h"
#include "src/hoi4_world/roles/triggers/always_trigger.h"
#include "src/hoi4_world/roles/triggers/and_trigger.h"
#include "src/hoi4_world/roles/triggers/tag_trigger.h"



namespace hoi4
{

TEST(Hoi4worldRolesRoleimporterTests, DefaultsAreDefaulted)
{
   std::stringstream input;

   RoleImporter importer;
   const Role role = importer.ImportRole("", input);

   EXPECT_TRUE(role.GetName().empty());
   EXPECT_TRUE(role.GetCategory().empty());
   EXPECT_EQ(role.GetTrigger(), AlwaysTrigger(false));
   EXPECT_EQ(role.GetScore(), 0.0F);
   EXPECT_TRUE(role.GetBlockers().empty());
   EXPECT_TRUE(role.GetSharedFocuses().empty());
   EXPECT_TRUE(role.GetFocuses().empty());
   EXPECT_TRUE(role.GetRepeatFocuses().empty());
   EXPECT_TRUE(role.GetRemovedFocuses().empty());
   EXPECT_TRUE(role.GetDecisionsCategories().empty());
   EXPECT_TRUE(role.GetDecisionsInCategories().empty());
   EXPECT_TRUE(role.GetEvents().empty());
}


TEST(Hoi4worldRolesRoleimporterTests, ItemsCanBeImported)
{
   std::stringstream input;
   input << " = {\n";
   input << "\tcategory=unification\n";
   input << "\ttrigger={\n";
   input << "\t\ttag = ITA\n";
   input << "\t\ttag = SIC\n";
   input << "\t}\n";
   input << "\n";
   input << "\tscore=100\n";
   input << "\n";
   input << "\tblock_role=unification_role\n";
   input << "\tblock_category=unification_category\n";
   input << "\n";
   input << "\tshared_focus=army_effort\n";
   input << "\tshared_focus=aviation_effort\n";
   input << "\tfocus={\n";
   input << "\t\tid = $TAG$_italia_irredenta #start of tree for an italian unifier\n";
   input << "\t}\n";
   input << "\tfocus={\n";
   input << "\t\tid = $TAG$_italia_irredenta_2 #start of tree for an italian unifier\n";
   input << "\t}\n";
   input << "\n";
   input << "\trepeat_focus={ #creates wargoal and ai strategy vs another country that owns a core or claim of ITA\n";
   input << "\t\ttrigger={\n";
   input << "\t\t\ttag = ITA\n";
   input << "\t\t\ttag = SIC\n";
   input << "\t\t}\n";
   input << "\t\tfocus={\n";
   input << "\t\t\tid = $TAG$_invade_$TARGET_TAG$\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "\trepeat_focus={ #creates wargoal and ai strategy vs another country that owns a core or claim of ITA\n";
   input << "\t\ttrigger={\n";
   input << "\t\t\ttag = ITA\n";
   input << "\t\t\ttag = SIC\n";
   input << "\t\t}\n";
   input << "\t\tfocus={\n";
   input << "\t\t\tid = $TAG$_invade_$TARGET_TAG$_2\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "\tremoved_focus={\n";
   input << "\t\tid = $TAG$_remove_me\n";
   input << "\t}\n";
   input << "\tremoved_focus={\n";
   input << "\t\tid = $TAG$_remove_me_2\n";
   input << "\t}\n";
   input << "\tdecisions_categories={\n";
   input << "\t\t$TAG$_a_decisions_category={\n";
   input << "\t\t}\n";
   input << "\t\t$TAG$_another_decisions_category={\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "\tdecisions={\n";
   input << "\t\t$TAG$_a_decisions_category = {\n";
   input << "\t\t\t$TAG$_a_decision={\n";
   input << "\t\t\t}\n";
   input << "\t\t\t$TAG$_another_decision={\n";
   input << "\t\t\t}\n";
   input << "\t\t}\n";
   input << "\t\t$TAG$_another_decisions_category = {\n";
   input << "\t\t\t$TAG$_a_third_decision={\n";
   input << "\t\t\t}\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "\tevents={\n";
   input << "\t\tcountry_event={\n";
   input << "\t\t\tid = $TAG$_an_event\n";
   input << "\t\t}\n";
   input << "\t\ttest_event_type={\n";
   input << "\t\t\tid = $TAG$_another_event\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "\t\n";
   input << "}\n";

   RoleImporter importer;
   const Role role = importer.ImportRole("unification_italy", input);

   EXPECT_EQ(role.GetName(), "unification_italy");
   EXPECT_EQ(role.GetCategory(), "unification");

   std::unique_ptr<Trigger> tag_trigger_ita_one = std::make_unique<TagTrigger>("ITA");
   std::unique_ptr<Trigger> tag_trigger_sic_one = std::make_unique<TagTrigger>("SIC");
   std::vector<std::unique_ptr<Trigger>> children_one;
   children_one.push_back(std::move(tag_trigger_ita_one));
   children_one.push_back(std::move(tag_trigger_sic_one));
   const AndTrigger and_trigger_one(std::move(children_one));
   EXPECT_EQ(role.GetTrigger(), and_trigger_one);
   EXPECT_FLOAT_EQ(role.GetScore(), 100.0F);
   EXPECT_THAT(role.GetBlockers(), testing::ElementsAre("unification_role", "unification_category"));
   EXPECT_THAT(role.GetSharedFocuses(), testing::ElementsAre("army_effort", "aviation_effort"));
   EXPECT_THAT(role.GetFocuses(),
       testing::ElementsAre(Focus{.id = "$TAG$_italia_irredenta"}, Focus{.id = "$TAG$_italia_irredenta_2"}));

   std::unique_ptr<Trigger> tag_trigger_ita_two = std::make_unique<TagTrigger>("ITA");
   std::unique_ptr<Trigger> tag_trigger_sic_two = std::make_unique<TagTrigger>("SIC");
   std::vector<std::unique_ptr<Trigger>> children_two;
   children_two.push_back(std::move(tag_trigger_ita_two));
   children_two.push_back(std::move(tag_trigger_sic_two));
   std::unique_ptr<AndTrigger> and_trigger_two = std::make_unique<AndTrigger>(std::move(children_two));

   std::unique_ptr<Trigger> tag_trigger_ita_three = std::make_unique<TagTrigger>("ITA");
   std::unique_ptr<Trigger> tag_trigger_sic_three = std::make_unique<TagTrigger>("SIC");
   std::vector<std::unique_ptr<Trigger>> children_three;
   children_three.push_back(std::move(tag_trigger_ita_three));
   children_three.push_back(std::move(tag_trigger_sic_three));
   std::unique_ptr<AndTrigger> and_trigger_three = std::make_unique<AndTrigger>(std::move(children_three));
   EXPECT_THAT(role.GetRepeatFocuses(),
       testing::ElementsAre(RepeatFocus(std::move(and_trigger_two), {Focus{.id = "$TAG$_invade_$TARGET_TAG$"}}),
           RepeatFocus(std::move(and_trigger_three), {Focus{.id = "$TAG$_invade_$TARGET_TAG$_2"}})));
   EXPECT_THAT(role.GetRemovedFocuses(),
       testing::ElementsAre("= {\n"
                            "\t\tid = $TAG$_remove_me\n"
                            "\t}",
           "= {\n"
           "\t\tid = $TAG$_remove_me_2\n"
           "\t}"));
   EXPECT_THAT(role.GetDecisionsCategories(),
       testing::ElementsAre(DecisionsCategory{.name = "$TAG$_a_decisions_category"},
           DecisionsCategory{.name = "$TAG$_another_decisions_category"}));
   EXPECT_THAT(role.GetDecisionsInCategories(),
       testing::ElementsAre(
           testing::Pair("$TAG$_a_decisions_category",
               std::vector<Decision>{Decision{.name = "$TAG$_a_decision"}, Decision{.name = "$TAG$_another_decision"}}),
           testing::Pair("$TAG$_another_decisions_category",
               std::vector<Decision>{Decision{.name = "$TAG$_a_third_decision"}})));
   EXPECT_THAT(role.GetEvents(),
       testing::ElementsAre(Event{.type = "country_event", .id = "$TAG$_an_event"},
           Event{.type = "test_event_type", .id = "$TAG$_another_event"}));
}

}  // namespace hoi4