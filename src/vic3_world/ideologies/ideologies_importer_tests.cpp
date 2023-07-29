#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/ideologies/ideologies_importer.h"



namespace vic3
{

TEST(Vic3worldIdeologiesIdeologiesimporterTests, IdeologiesAreImported)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/ideologies", {});

   const Ideologies ideologies = IdeologiesImporter{}.ImportIdeologies(mod_filesystem);

   const std::map<Law, ApprovalAmount> approvals = ideologies.CalculateLawApprovals(
       {"ideology_paternalistic", "ideology_laissez_faire", "ideology_traditionalist"});

   EXPECT_THAT(approvals,
       testing::UnorderedElementsAre(testing::Pair("law_monarchy", 2),
           testing::Pair("law_single_party_state", 0),
           testing::Pair("law_laissez_faire", 2),
           testing::Pair("law_no_womens_rights", 1),
           testing::Pair("law_state_religion", 1)));
}

}  // namespace vic3