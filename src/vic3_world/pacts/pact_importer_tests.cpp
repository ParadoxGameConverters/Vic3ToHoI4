#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/pacts/pact_importer.h"

namespace vic3
{

TEST(Vic3WorldPactImporter, PactsCanBeImported)
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
   Pact pact = importer.ImportPact(0, input);
   EXPECT_EQ(pact, Pact(1053, 268, "alliance", date("1930.2.27"), 120));
   std::stringstream input2;
   input2 << R""(
{
	first=36
	second=176
	action=puppet	start_date=1836.1.1
}
)"";
   pact = importer.ImportPact(1, input2);
   EXPECT_EQ(pact, Pact(36, 176, "puppet", date("1836.1.1"), std::nullopt));
}
}  // namespace vic3
