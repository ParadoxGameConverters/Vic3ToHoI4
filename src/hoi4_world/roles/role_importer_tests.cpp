#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/roles/role.h"
#include "src/hoi4_world/roles/role_importer.h"



namespace hoi4
{

TEST(Hoi4worldRolesRoleimporterTests, DefaultsAreDefaulted)
{
   std::stringstream input;

   RoleImporter importer;
   const Role role = importer.ImportRole("", input);

   EXPECT_TRUE(role.GetName().empty());
   EXPECT_TRUE(role.GetCategory().empty());
   EXPECT_TRUE(role.GetRequirements().empty());
   EXPECT_EQ(role.GetScore(), 0.0F);
   EXPECT_TRUE(role.GetBlockers().empty());
   EXPECT_TRUE(role.GetSharedFocuses().empty());
   EXPECT_TRUE(role.GetFocuses().empty());
   EXPECT_TRUE(role.GetRepeatFocuses().empty());
   EXPECT_TRUE(role.GetRemovedFocuses().empty());
   EXPECT_TRUE(role.GetDecisions().empty());
   EXPECT_TRUE(role.GetEvents().empty());
}


TEST(Hoi4worldRolesRoleimporterTests, ItemsCanBeImported)
{
   std::stringstream input;
   input << " = {\n";
   input << "\tcategory=unification\n";
   input << "\trequirements={\n";
   input << "\t\tOR = {\n";
   input << "\t\t\ttag=ITA\n";
   input << "\t\t}\n";
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
   input << "\t\trequirement={\n";
   input << "\t\t\tany_other_country = {\n";
   input << "\t\t\t\t\tany_owned_state = {\n";
   input << "\t\t\t\t\t\tOR = {\n";
   input << "\t\t\t\t\t\t\tis_core_of = ITA\n";
   input << "\t\t\t\t\t\t\tis_claimed_by = ITA\n";
   input << "\t\t\t\t\t\t}\n";
   input << "\t\t\t\t\t}\n";
   input << "\t\t\t}\n";
   input << "\t\t}\n";
   input << "\t\tfocus={\n";
   input << "\t\t\tid = $TAG$_invade_$TARGET_TAG$\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "\trepeat_focus={ #creates wargoal and ai strategy vs another country that owns a core or claim of ITA\n";
   input << "\t\trequirement={\n";
   input << "\t\t\tany_other_country = {\n";
   input << "\t\t\t\t\tany_owned_state = {\n";
   input << "\t\t\t\t\t\tOR = {\n";
   input << "\t\t\t\t\t\t\tis_core_of = ITA\n";
   input << "\t\t\t\t\t\t\tis_claimed_by = ITA\n";
   input << "\t\t\t\t\t\t}\n";
   input << "\t\t\t\t\t}\n";
   input << "\t\t\t}\n";
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
   input << "\tdecision={\n";
   input << "\t\tid = $TAG$_a_decision\n";
   input << "\t}\n";
   input << "\tdecision={\n";
   input << "\t\tid = $TAG$_another_decision\n";
   input << "\t}\n";
   input << "\tevent={\n";
   input << "\t\tid = $TAG$_an_event\n";
   input << "\t}\n";
   input << "\tevent={\n";
   input << "\t\tid = $TAG$_another_event\n";
   input << "\t}\n";
   input << "\t\n";
   input << "}\n";

   RoleImporter importer;
   const Role role = importer.ImportRole("unification_italy", input);

   EXPECT_EQ(role.GetName(), "unification_italy");
   EXPECT_EQ(role.GetCategory(), "unification");
   EXPECT_EQ(role.GetRequirements(),
       "= {\n"
       "\t\tOR = {\n"
       "\t\t\ttag=ITA\n"
       "\t\t}\n"
       "\t}");
   EXPECT_FLOAT_EQ(role.GetScore(), 100.0F);
   EXPECT_THAT(role.GetBlockers(), testing::ElementsAre("unification_role", "unification_category"));
   EXPECT_THAT(role.GetSharedFocuses(), testing::ElementsAre("army_effort", "aviation_effort"));
   EXPECT_THAT(role.GetFocuses(),
       testing::ElementsAre(Focus{.id = "$TAG$_italia_irredenta"}, Focus{.id = "$TAG$_italia_irredenta_2"}));
   EXPECT_THAT(role.GetRepeatFocuses(),
       testing::ElementsAre("= { #creates wargoal and ai strategy vs another country that owns a core or claim of ITA\n"
                            "\t\trequirement={\n"
                            "\t\t\tany_other_country = {\n"
                            "\t\t\t\t\tany_owned_state = {\n"
                            "\t\t\t\t\t\tOR = {\n"
                            "\t\t\t\t\t\t\tis_core_of = ITA\n"
                            "\t\t\t\t\t\t\tis_claimed_by = ITA\n"
                            "\t\t\t\t\t\t}\n"
                            "\t\t\t\t\t}\n"
                            "\t\t\t}\n"
                            "\t\t}\n"
                            "\t\tfocus={\n"
                            "\t\t\tid = $TAG$_invade_$TARGET_TAG$\n"
                            "\t\t}\n"
                            "\t}",
           "= { #creates wargoal and ai strategy vs another country that owns a core or claim of ITA\n"
           "\t\trequirement={\n"
           "\t\t\tany_other_country = {\n"
           "\t\t\t\t\tany_owned_state = {\n"
           "\t\t\t\t\t\tOR = {\n"
           "\t\t\t\t\t\t\tis_core_of = ITA\n"
           "\t\t\t\t\t\t\tis_claimed_by = ITA\n"
           "\t\t\t\t\t\t}\n"
           "\t\t\t\t\t}\n"
           "\t\t\t}\n"
           "\t\t}\n"
           "\t\tfocus={\n"
           "\t\t\tid = $TAG$_invade_$TARGET_TAG$_2\n"
           "\t\t}\n"
           "\t}"));
   EXPECT_THAT(role.GetRemovedFocuses(),
       testing::ElementsAre("= {\n"
                            "\t\tid = $TAG$_remove_me\n"
                            "\t}",
           "= {\n"
           "\t\tid = $TAG$_remove_me_2\n"
           "\t}"));
   EXPECT_THAT(role.GetDecisions(),
       testing::ElementsAre("= {\n"
                            "\t\tid = $TAG$_a_decision\n"
                            "\t}",
           "= {\n"
           "\t\tid = $TAG$_another_decision\n"
           "\t}"));
   EXPECT_THAT(role.GetEvents(),
       testing::ElementsAre("= {\n"
                            "\t\tid = $TAG$_an_event\n"
                            "\t}",
           "= {\n"
           "\t\tid = $TAG$_another_event\n"
           "\t}"));
}

}  // namespace hoi4