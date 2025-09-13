#include "src/vic3_world/pacts/pact.h"

#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

namespace vic3
{

TEST(Vic3WorldPact, SubjectRelationshipsAreDetermined)
{
   const Pact puppet = Pact(PactPartners{.first = 0, .second = 1}, "puppet", date(""), std::nullopt);
   EXPECT_TRUE(puppet.IsSubjectRelationship());
   const Pact alliance = Pact(PactPartners{.first = 2, .second = 3}, "alliance", date(""), std::nullopt);
   EXPECT_FALSE(alliance.IsSubjectRelationship());
}
}  // namespace vic3
