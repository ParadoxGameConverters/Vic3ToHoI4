#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/interest_groups/interest_group_importer.h"

namespace vic3
{


TEST(Vic3WorldInterestGroupsInterestGroupImporter, DefaultsDefaultToDefault)
{
   InterestGroupImporter interest_group_importer;

   std::stringstream input;
   const auto interest_group = interest_group_importer.ImportInterestGroup(input);

   EXPECT_EQ(interest_group, InterestGroup("", 0, 0, 0.0F, false, {}));
}


TEST(Vic3WorldInterestGroupsInterestGroupImporter, InterestGroupCanBeImported)
{
   InterestGroupImporter interest_group_importer;

   std::stringstream input;
   input << "={\n";
   input << "\tcountry = 2\n";
   input << "\tdefinition = \"ig_example\"\n";
   input << "\tleader = 1\n";
   input << "\tclout = 0.25447\n";
   input << "\tin_government = yes\n";
   input << "\tideologies = { ideology_liberal ideology_anti_clerical ideology_republican ideology_anti_slavery }\n";
   input << "}\n";

   const auto interest_group = interest_group_importer.ImportInterestGroup(input);

   EXPECT_EQ(interest_group,
       InterestGroup("ig_example",
           2,
           1,
           0.25447F,
           true,
           {"ideology_liberal", "ideology_anti_clerical", "ideology_republican", "ideology_anti_slavery"}));
}
}  // namespace vic3