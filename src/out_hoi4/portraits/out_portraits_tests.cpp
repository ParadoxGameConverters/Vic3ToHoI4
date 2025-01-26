#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/out_hoi4/portraits/out_portraits.h"



namespace out
{
TEST(Outhoi4PortraitsTests, PortraitFileIsCreated)
{
   std::filesystem::create_directories("output/PortraitFileIsCreated/portraits");

   OutputPortraits("PortraitFileIsCreated",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}});

   std::ifstream portraits_file("output/PortraitFileIsCreated/portraits/converter_portraits.txt");
   ASSERT_TRUE(portraits_file.is_open());
   std::stringstream portraits_file_stream;
   std::copy(std::istreambuf_iterator<char>(portraits_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(portraits_file_stream));
   portraits_file.close();
   EXPECT_EQ(portraits_file_stream.str(),
       "TAG = {\n"
       "\tarmy = {\n"
       "\t}\n"
       "\tnavy = {\n"
       "\t}\n"
       "\tpolitical = {\n"
       "\t}\n"
       "\toperative = {\n"
       "\t}\n"
       "\tscientist = {\n"
       "\t}\n"
       "}\n"
       "TWO = {\n"
       "\tarmy = {\n"
       "\t}\n"
       "\tnavy = {\n"
       "\t}\n"
       "\tpolitical = {\n"
       "\t}\n"
       "\toperative = {\n"
       "\t}\n"
       "\tscientist = {\n"
       "\t}\n"
       "}\n");
}
}  // namespace out