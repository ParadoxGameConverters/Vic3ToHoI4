#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <ostream>
#include <sstream>

#include "src/out_hoi4/decisions/out_decisions.h"


namespace out
{

// Remove this test when decisions are implemented!
TEST(OutHoI4DecisionsOutDecisions, UnimplementedFeatureIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());
   OutputDecisions("dummy", {});
   std::cout.rdbuf(cout_buffer);
   EXPECT_THAT(log.str(), testing::HasSubstr("Decisions not implemented yet"));
}

}  // namespace out
