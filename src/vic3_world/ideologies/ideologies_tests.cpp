#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/ideologies/ideologies.h"



namespace vic3
{

TEST(Vic3worldIdeologiesIdeologiesTests, NoIdeologiesNoApprovals)
{
   const Ideologies ideologies;

   const std::map<Law, ApprovalAmount> approvals = ideologies.CalculateLawApprovals({});

   EXPECT_TRUE(approvals.empty());
}


TEST(Vic3worldIdeologiesIdeologiesTests, MissingIdeologiesNoApprovals)
{
   const Ideologies ideologies;

   const std::map<Law, ApprovalAmount> approvals =
       ideologies.CalculateLawApprovals({"test_ideology_one", "test_ideology_two"});

   EXPECT_TRUE(approvals.empty());
}


TEST(Vic3worldIdeologiesIdeologiesTests, ApprovalsAreCalculated)
{
   const Ideologies ideologies({
       {"test_ideology_one", Ideology({{"test_law_one", 2}, {"test_law_two", 3}})},
       {"test_ideology_two", Ideology({{"test_law_one", 5}})},
   });
   const std::map<Law, ApprovalAmount> approvals =
       ideologies.CalculateLawApprovals({"test_ideology_one", "test_ideology_two"});

   EXPECT_THAT(approvals,
       testing::UnorderedElementsAre(testing::Pair("test_law_one", 7), testing::Pair("test_law_two", 3)));
}

}  // namespace vic3
