#include "src/vic3_world/pacts/pact.h"

#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

namespace vic3
{

TEST(Vic3WorldPact, SubjectRelationshipsAreDetermined)
{
   Pact puppet = Pact(0, 1, "puppet", date(""), std::nullopt);
   EXPECT_TRUE(puppet.isSubjectRelationship());
   Pact alliance = Pact(2, 3, "alliance", date(""), std::nullopt);
   EXPECT_FALSE(alliance.isSubjectRelationship());
}
}  // namespace vic3
