#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/countries/tag_alias.h"
#include "src/out_hoi4/countries/out_tag_aliases.h"



using std::filesystem::create_directories;
using std::filesystem::path;



namespace out
{

TEST(Outhoi4CountriesOuttagaliasesTests, AliasesFileIsCreated)
{
   std::filesystem::remove_all("output/AliasesFileIsCreated");

   OutputTagAliases("AliasesFileIsCreated",
       {
           {"TAG", "ORI", "test_flag"},
           {"TWO", "OTH", "another_flag"},
       });

   std::ifstream aliases_file("output/AliasesFileIsCreated/common/country_tag_aliases/converter_tag_aliases.txt");
   ASSERT_TRUE(aliases_file.is_open());
   std::stringstream aliases_file_stream;
   std::copy(std::istreambuf_iterator<char>(aliases_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(aliases_file_stream));
   aliases_file.close();
   EXPECT_EQ(aliases_file_stream.str(),
       "TAG = {\n"
       "\toriginal_tag = ORI\n"
       "\thas_country_flag = test_flag\n"
       "}\n"
       "\n"
       "TWO = {\n"
       "\toriginal_tag = OTH\n"
       "\thas_country_flag = another_flag\n"
       "}\n"
       "\n");
}


TEST(Outhoi4CountriesOuttagaliasesTests, ExceptionIfAliasesDirectoryNotCreated)
{
   EXPECT_THROW(OutputTagAliases("/dev/null/COM", {}), std::runtime_error);
}

}  // namespace out
