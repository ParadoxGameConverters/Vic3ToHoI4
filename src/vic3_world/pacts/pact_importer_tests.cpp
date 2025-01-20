#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/pacts/pact_importer.h"



namespace vic3
{


TEST(Vic3WorldPactImporter, DefaultsAreDefaulted)
{
   PactImporter importer;
   std::stringstream input;
   input << R""(
{
}
)"";

   const Pact pact = importer.ImportPact(input);

   EXPECT_EQ(pact.GetFirstId(), 0);
   EXPECT_EQ(pact.GetSecondId(), 0);
   EXPECT_TRUE(pact.GetAction().empty());
   EXPECT_EQ(pact.GetStartDate(), date());
   EXPECT_FALSE(pact.GetForcedDuration().has_value());
}


TEST(Vic3WorldPactImporter, PactsCanBeImported)
{
   PactImporter importer;
   std::stringstream input;
   input << R""(
{
	targets={
		first=1053
		second=268
	}
	action=alliance
	start_date=1930.2.27
	forced_duration=120
}
)"";

   const Pact pact = importer.ImportPact(input);

   EXPECT_EQ(pact.GetFirstId(), 1053);
   EXPECT_EQ(pact.GetSecondId(), 268);
   EXPECT_EQ(pact.GetAction(), "alliance");
   EXPECT_EQ(pact.GetStartDate(), date("1930.2.27"));
   EXPECT_TRUE(pact.GetForcedDuration().has_value());
   EXPECT_EQ(pact.GetForcedDuration().value_or(0), 120);
}


TEST(Vic3WorldPactImporter, Pre1_5PactsCanBeImported)
{
   PactImporter importer;
   std::stringstream input;
   input << R""(
{
	first=1053
	second=268
	action=alliance	start_date=1930.2.27
	forced_duration=120
}
)"";

   const Pact pact = importer.ImportPact(input);

   EXPECT_EQ(pact.GetFirstId(), 1053);
   EXPECT_EQ(pact.GetSecondId(), 268);
   EXPECT_EQ(pact.GetAction(), "alliance");
   EXPECT_EQ(pact.GetStartDate(), date("1930.2.27"));
   EXPECT_TRUE(pact.GetForcedDuration().has_value());
   EXPECT_EQ(pact.GetForcedDuration().value_or(0), 120);
}

}  // namespace vic3
