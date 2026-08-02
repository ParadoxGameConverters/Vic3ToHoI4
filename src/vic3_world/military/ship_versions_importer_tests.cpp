#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <optional>
#include <sstream>

#include "src/vic3_world/military/ship_versions_importer.hpp"



namespace vic3
{

TEST(Vic3worldMilitaryShipversionsimporterImporter, EmptyManagerGivesNoVersions)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "\t}\n";
   input << "\tversions={\n";
   input << "\t\tdatabase={\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "}";

   const std::map<int64_t, std::string> ship_versions = ImportShipVersions(input);

   EXPECT_TRUE(ship_versions.empty());
}


TEST(Vic3worldMilitaryShipversionsimporterImporter, VersionsCanBeInput)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "\t}\n";
   input << "\tversions={\n";
   input << "\t\tdatabase={\n";
   input << "\t\t\t0={\n";
   input << "\t\t\t\tinfo={\n";
   input << "\t\t\t\t\ttype=\"test_template_one\"\n";
   input << "\t\t\t\t}\n";
   input << "\t\t\t}\n";
   input << "\t\t\t10={\n";
   input << "\t\t\t\tinfo={\n";
   input << "\t\t\t\t\ttype=\"test_template_one\"\n";
   input << "\t\t\t\t}\n";
   input << "\t\t\t}\n";
   input << "\t\t\t2={\n";
   input << "\t\t\t\tinfo={\n";
   input << "\t\t\t\t\ttype=\"test_template_two\"\n";
   input << "\t\t\t\t}\n";
   input << "\t\t\t}\n";
   input << "\t\t\t20={\n";
   input << "\t\t\t\tinfo={\n";
   input << "\t\t\t\t\ttype=\"test_template_two\"\n";
   input << "\t\t\t\t}\n";
   input << "\t\t\t}\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "}";

   const std::map<int64_t, std::string> ship_versions = ImportShipVersions(input);

   EXPECT_THAT(ship_versions,
       testing::UnorderedElementsAre(testing::Pair(0, "test_template_one"),
           testing::Pair(10, "test_template_one"),
           testing::Pair(2, "test_template_two"),
           testing::Pair(20, "test_template_two")));
}

}  // namespace vic3