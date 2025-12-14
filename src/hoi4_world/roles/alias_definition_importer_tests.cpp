#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/roles/alias_definition.h"
#include "src/hoi4_world/roles/alias_definition_importer.h"



namespace hoi4
{

TEST(Hoi4worldRolesAliasdefinitionimporterTests, DefaultsAreDefaulted)
{
   std::stringstream input;

   AliasDefinitionImporter importer;
   const AliasDefinition alias_definition = importer.ImportAliasDefinition(input);

   EXPECT_TRUE(alias_definition.country_flag.empty());
   EXPECT_TRUE(alias_definition.alias_identifier.empty());
}


TEST(Hoi4worldRolesAliasdefinitionimporterTests, ItemsCanBeInput)
{
   std::stringstream input;
   input << "country_flag = $TAG$_test_flag\n";
   input << "alias_identifier  = $TAG$_identifier\n";

   AliasDefinitionImporter importer;
   const AliasDefinition alias_definition = importer.ImportAliasDefinition(input);

   EXPECT_EQ(alias_definition.country_flag, "$TAG$_test_flag");
   EXPECT_EQ(alias_definition.alias_identifier, "$TAG$_identifier");
}

}  // namespace hoi4