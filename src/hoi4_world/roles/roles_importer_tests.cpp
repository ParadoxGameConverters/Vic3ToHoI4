#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/roles/roles_importer.h"



namespace hoi4
{

TEST(Hoi4worldRolesRolesimporterTests, RolesCanBeImported)
{
   const std::map<std::string, Role> roles = ImportRoles();

   EXPECT_THAT(roles,
       testing::ElementsAre(testing::Pair("role_1", Role{{.name = "role_1"}}),
           testing::Pair("role_2", Role{{.name = "role_2"}}),
           testing::Pair("role_3", Role{{.name = "role_3"}})));
}

}  // namespace hoi4