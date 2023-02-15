#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/technology/technologies.h"
#include "src/out_hoi4/technology/out_technologies.h"



namespace out
{

TEST(Outhoi4TechnologyOutTechnologiesTests, InitialCommentIsOutput)
{
   const hoi4::Technologies technologies(std::map<std::optional<std::string>, std::set<std::string>>{});

   std::stringstream output;
   output << technologies;

   EXPECT_EQ(output.str(), "# Starting tech\n");
}


TEST(Outhoi4TechnologyOutTechnologiesTests, NoLimitTechnologiesAreOutput)
{
   const hoi4::Technologies technologies({{std::nullopt, {"test_tech_one", "test_tech_two"}}});

   std::stringstream output;
   output << technologies;

   std::stringstream expected;
   expected << "# Starting tech\n";
   expected << "set_technology = {\n";
   expected << "\ttest_tech_one = 1\n";
   expected << "\ttest_tech_two = 1\n";
   expected << "}\n";
   expected << "\n";

   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4TechnologyOutTechnologiesTests, LimitTechnologiesAreOutput)
{
   const hoi4::Technologies technologies({
       {"limit_one", {"test_tech_one", "test_tech_two"}},
       {"limit_two", {"test_tech_three", "test_tech_four"}},
   });

   std::stringstream output;
   output << technologies;

   std::stringstream expected;
   expected << "# Starting tech\n";
   expected << "if = {\n";
   expected << "\tlimit = { limit_one }\n";
   expected << "\tset_technology = {\n";
   expected << "\t\ttest_tech_one = 1\n";
   expected << "\t\ttest_tech_two = 1\n";
   expected << "\t}\n";
   expected << "}\n";
   expected << "\n";
   expected << "if = {\n";
   expected << "\tlimit = { limit_two }\n";
   expected << "\tset_technology = {\n";
   expected << "\t\ttest_tech_four = 1\n";
   expected << "\t\ttest_tech_three = 1\n";
   expected << "\t}\n";
   expected << "}\n";
   expected << "\n";

   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4TechnologyOutTechnologiesTests, EmptyLimitTechnologiesAreNotOutput)
{
   const hoi4::Technologies technologies({
       {"limit_one", {"test_tech_one", "test_tech_two"}},
       {"limit_two", {}},
   });

   std::stringstream output;
   output << technologies;

   std::stringstream expected;
   expected << "# Starting tech\n";
   expected << "if = {\n";
   expected << "\tlimit = { limit_one }\n";
   expected << "\tset_technology = {\n";
   expected << "\t\ttest_tech_one = 1\n";
   expected << "\t\ttest_tech_two = 1\n";
   expected << "\t}\n";
   expected << "}\n";
   expected << "\n";

   EXPECT_EQ(output.str(), expected.str());
}


TEST(Outhoi4TechnologyOutTechnologiesTests, NoLimitTechnologiesAreNotRepeated)
{
   const hoi4::Technologies technologies({
       {std::nullopt, {"test_tech_one", "test_tech_two"}},
       {"limit_one", {"test_tech_three", "test_tech_four"}},
   });

   std::stringstream output;
   output << technologies;

   std::stringstream expected;
   expected << "# Starting tech\n";
   expected << "set_technology = {\n";
   expected << "\ttest_tech_one = 1\n";
   expected << "\ttest_tech_two = 1\n";
   expected << "}\n";
   expected << "\n";
   expected << "if = {\n";
   expected << "\tlimit = { limit_one }\n";
   expected << "\tset_technology = {\n";
   expected << "\t\ttest_tech_four = 1\n";
   expected << "\t\ttest_tech_three = 1\n";
   expected << "\t}\n";
   expected << "}\n";
   expected << "\n";

   EXPECT_EQ(output.str(), expected.str());
}

}  // namespace out