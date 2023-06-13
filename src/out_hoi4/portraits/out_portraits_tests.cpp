#include <filesystem>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/out_hoi4/portraits/out_portrait_paths.h"



namespace out
{

TEST(Outhoi4PortraitTests, PortraitIsOutput)
{
   const mappers::GraphicsBlock block({.army = {"g0", "g1"},
       .navy = {"a0", "a1"},
       .leader =
           {.communism = {"c0", "c1"}, .democratic = {"d0", "d1"}, .fascism = {"f0", "f1"}, .neutrality = {"n0", "n1"}},
       .female_leader = {"f_lead0", "f_lead1"},
       .male_operative = {"m_spy0", "m_spy1"},
       .female_operative = {"f_spy0", "f_spy1"}});

   std::stringstream out;
   out << block.portrait_paths;

   EXPECT_EQ(out.str(),
       "\tarmy = {\n"
       "\t\tmale = {\n"
       "\t\t\tg0\n"
       "\t\t\tg1\n"
       "\t\t}\n"
       "\t}\n"
       "\tnavy = {\n"
       "\t\tmale = {\n"
       "\t\t\ta0\n"
       "\t\t\ta1\n"
       "\t\t}\n"
       "\t}\n"
       "\tpolitical = {\n"
       "\t\tcommunism = {\n"
       "\t\tmale = {\n"
       "\t\t\tc0\n"
       "\t\t\tc1\n"
       "\t\t}\n"
       "\t\t}\n"
       "\t\tdemocratic = {\n"
       "\t\tmale = {\n"
       "\t\t\td0\n"
       "\t\t\td1\n"
       "\t\t}\n"
       "\t\t}\n"
       "\t\tfascism = {\n"
       "\t\tmale = {\n"
       "\t\t\tf0\n"
       "\t\t\tf1\n"
       "\t\t}\n"
       "\t\t}\n"
       "\t\tneutrality = {\n"
       "\t\tmale = {\n"
       "\t\t\tn0\n"
       "\t\t\tn1\n"
       "\t\t}\n"
       "\t\t}\n"
       "\t}\n"
       "\toperative = {\n"
       "\t\tmale = {\n"
       "\t\t\tm_spy0\n"
       "\t\t\tm_spy1\n"
       "\t\t}\n"
       "\t\tfemale = {\n"
       "\t\t\tf_spy0\n"
       "\t\t\tf_spy1\n"
       "\t\t}\n"
       "\t}\n"
       "\t\tfemale = {\n"
       "\t\t\tf_lead0\n"
       "\t\t\tf_lead1\n"
       "\t\t}\n");
}

}  // namespace out