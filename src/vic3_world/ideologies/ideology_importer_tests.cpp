#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/ideologies/ideology_importer.h"



namespace vic3
{

TEST(Vic3worldIdeologiesIdeologyimporterTests, DefaultsAreSet)
{
   std::stringstream input;

   const Ideology ideology = IdeologyImporter{}.ImportIdeology(input);

   EXPECT_TRUE(ideology.GetLawApprovals().empty());
}


TEST(Vic3worldIdeologiesIdeologyimporterTests, ApprovalsAreImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "\ticon = \"gfx/interface/icons/ideology_icons/paternalistic.dds\"\n";
   input << "\t\n";
   input << "\tlawgroup_governance_principles = {\n";
   input << "\t\tlaw_monarchy = strongly_approve\n";
   input << "\t\tlaw_theocracy = approve\t\t\t\n";
   input << "\t\tlaw_presidential_republic = neutral\n";
   input << "\t\tlaw_council_republic = strongly_disapprove\n";
   input << "\t}\n";
   input << "\t\n";
   input << "\tlawgroup_distribution_of_power = {\n";
   input << "\t\tlaw_census_voting = disapprove\n";
   input << "\t}\n";
   input << "}\n";

   const Ideology ideology = IdeologyImporter{}.ImportIdeology(input);

   EXPECT_THAT(ideology.GetLawApprovals(),
       testing::UnorderedElementsAre(testing::Pair("law_monarchy", 2),
           testing::Pair("law_theocracy", 1),
           testing::Pair("law_presidential_republic", 0),
           testing::Pair("law_council_republic", -2),
           testing::Pair("law_census_voting", -1)));
}

}  // namespace vic3