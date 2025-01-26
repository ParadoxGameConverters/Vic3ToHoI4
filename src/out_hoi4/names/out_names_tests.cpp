#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/out_hoi4/names/out_names.h"

namespace out
{
TEST(Outhoi4NamesTests, NamesFileIsCreated)
{
   std::filesystem::create_directories("output/NamesFileIsCreated/common/names");

   OutputNames("NamesFileIsCreated", {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}});

   std::ifstream names_file("output/NamesFileIsCreated/common/names/converter_names.txt");
   ASSERT_TRUE(names_file.is_open());
   std::stringstream names_file_stream;
   std::copy(std::istreambuf_iterator<char>(names_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(names_file_stream));
   names_file.close();
   EXPECT_EQ(names_file_stream.str(),
       "TAG = {\n"
       "\tmale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\n"
       "\t\t}\n"
       "\t}\n"
       "\tfemale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\n"
       "\t\t}\n"
       "\t}\n"
       "\tsurnames = {\n"
       "\t\t\t\n"
       "\t}\n"
       "}\n"
       "TWO = {\n"
       "\tmale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\n"
       "\t\t}\n"
       "\t}\n"
       "\tfemale = {\n"
       "\t\tnames = {\n"
       "\t\t\t\n"
       "\t\t}\n"
       "\t}\n"
       "\tsurnames = {\n"
       "\t\t\t\n"
       "\t}\n"
       "}\n");
}
}  // namespace out