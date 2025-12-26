#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/countries/tag_alias.h"
#include "src/out_hoi4/countries/out_tag_alias.h"



using std::filesystem::create_directories;
using std::filesystem::path;
using std::filesystem::remove_all;



namespace out
{

TEST(Outhoi4CounriesOutTagAlias, TagAliasesCanBeOutput)
{
   const hoi4::TagAlias tag_alias = hoi4::TagAlias{.alias = "TAG", .original_tag = "ORG", .flag = "test_flag"};
   std::stringstream output;
   OutputTagAlias(tag_alias, output);
   const std::string expected_output =
       "TAG = {\n"
       "\toriginal_tag = ORG\n"
       "\thas_country_flag = test_flag\n"
       "}\n"
       "\n";
   EXPECT_EQ(output.str(), expected_output);
}

}  // namespace out
