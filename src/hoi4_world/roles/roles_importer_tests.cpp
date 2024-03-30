#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/roles/roles_importer.h"



TEST(Hoi4worldRolesRolesimporterTests, RolesCanBeImported)
{
   const std::vector<hoi4::Role> roles = hoi4::ImportRoles();

   EXPECT_THAT(roles,
       testing::ElementsAre(hoi4::Role{{.name = "role_1"}},
           hoi4::Role{{.name = "role_2"}},
           hoi4::Role{{.name = "role_3"}}));
}