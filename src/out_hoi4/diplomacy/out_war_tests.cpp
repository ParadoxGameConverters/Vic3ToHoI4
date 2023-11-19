#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/out_hoi4/diplomacy/out_war.h"



namespace out
{

TEST(Outhoi4DiplomacyOutWarTests, BasicsAreOutput)
{
   const hoi4::War war({.original_defender = "DEF"});

   std::ostringstream output;
   output << war;

   std::ostringstream expected;
   expected << "declare_war_on = {\n";
   expected << "\ttarget = DEF\n";
   expected << "\ttype = annex_everything\n";
   expected << "}\n";
   expected << "\n";
   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4DiplomacyOutWarTests, ExtraAttackersCanBeAdded)
{
   const hoi4::War war({.original_defender = "DEF", .original_attacker = "ATK", .extra_attackers = {"AT2", "AT3"}});

   std::ostringstream output;
   output << war;

   std::ostringstream expected;
   expected << "declare_war_on = {\n";
   expected << "\ttarget = DEF\n";
   expected << "\ttype = annex_everything\n";
   expected << "}\n";
   expected << "AT2 = {\n";
   expected << "\tadd_to_war = {\n";
   expected << "\t\ttargeted_alliance = ATK\n";
   expected << "\t\tenemy = DEF\n";
   expected << "\t}\n";
   expected << "}\n";
   expected << "AT3 = {\n";
   expected << "\tadd_to_war = {\n";
   expected << "\t\ttargeted_alliance = ATK\n";
   expected << "\t\tenemy = DEF\n";
   expected << "\t}\n";
   expected << "}\n";
   expected << "\n";
   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4DiplomacyOutWarTests, ExtraDefendersCanBeAdded)
{
   const hoi4::War war({.original_defender = "DEF", .extra_defenders = {"DF2", "DF3"}, .original_attacker = "ATK"});

   std::ostringstream output;
   output << war;

   std::ostringstream expected;
   expected << "declare_war_on = {\n";
   expected << "\ttarget = DEF\n";
   expected << "\ttype = annex_everything\n";
   expected << "}\n";
   expected << "DF2 = {\n";
   expected << "\tadd_to_war = {\n";
   expected << "\t\ttargeted_alliance = DEF\n";
   expected << "\t\tenemy = ATK\n";
   expected << "\t}\n";
   expected << "}\n";
   expected << "DF3 = {\n";
   expected << "\tadd_to_war = {\n";
   expected << "\t\ttargeted_alliance = DEF\n";
   expected << "\t\tenemy = ATK\n";
   expected << "\t}\n";
   expected << "}\n";
   expected << "\n";
   EXPECT_EQ(output.str(), expected.str());
}

}  // namespace out