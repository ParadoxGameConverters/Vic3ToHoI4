#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/interest_groups/interest_groups_importer.h"

namespace vic3
{


TEST(Vic3WorldInterestGroupsInterestGroupsImporter, NoInterestGroupsOnEmptyInput)
{
   std::stringstream input;
   const std::map<int, InterestGroup> igs = ImportInterestGroups(input);

   EXPECT_TRUE(igs.empty());
}


TEST(Vic3WorldInterestGroupsInterestGroupsImporter, InterestGroupsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0={\n";
   input << "\tcountry = 2\n";
   input << "\tdefinition = \"ig_example0\"\n";
   input << "\tleader = 1\n";
   input << "\tclout = 0.25447\n";
   input << "\tin_government = yes\n";
   input << "}\n";
   input << "1={\n";
   input << "\tcountry = 4\n";
   input << "\tdefinition = \"ig_example1\"\n";
   input << "\tleader = 3\n";
   input << "\tclout = 0.36117\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, InterestGroup> igs = ImportInterestGroups(input);

   EXPECT_THAT(igs,
       testing::UnorderedElementsAre(testing::Pair(0, InterestGroup("ig_example0", 2, 1, 0.25447F, true, {})),
           testing::Pair(1, InterestGroup("ig_example1", 4, 3, 0.36117F, false, {}))));
}


TEST(Vic3WorldInterestGroupsInterestGroupsImporter, InterestGroupsSetAsNoneAreSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = none\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, InterestGroup> interest_groups = ImportInterestGroups(input);
   EXPECT_TRUE(interest_groups.empty());
}


TEST(Vic3WorldInterestGroupsInterestGroupsImporter, InterestGroupsImportsAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0={\n";
   input << "\tcountry = 2\n";
   input << "\tdefinition = \"ig_example0\"\n";
   input << "\tleader = 1\n";
   input << "\tclout = 0.25447\n";
   input << "\tin_government = yes\n";
   input << "}\n";
   input << "1={\n";
   input << "\tcountry = 4\n";
   input << "\tdefinition = \"ig_example1\"\n";
   input << "\tleader = 3\n";
   input << "\tclout = 0.36117\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::map<int, InterestGroup> _ = ImportInterestGroups(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 2 IGs.\n"));
}
}  // namespace vic3