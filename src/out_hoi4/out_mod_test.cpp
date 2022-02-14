#include "out_mod.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/googletest/googletest/include/gtest/gtest.h"



TEST(OutHoi4OutModTest, ModFolderIsCreatedAndCleared)
{
   Output("test_output");

   EXPECT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   ClearOutputFolder("test_output");
   EXPECT_FALSE(commonItems::DoesFolderExist("output/test_output"));
}


TEST(OutHoi4OutModTest, ModFileIsCreated)
{
   Output("test_output");

   ASSERT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   std::ifstream mod_file("output/test_output.mod");
   ASSERT_TRUE(mod_file.is_open());
   std::stringstream mod_file_stream;
   std::copy(std::istreambuf_iterator<char>(mod_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(mod_file_stream));
   mod_file.close();

   std::stringstream expected;
   expected << "name = \"Converted - test_output\"\n";
   expected << "path = \"mod/test_output/\"\n";
   expected << "user_dir = \"test_output_user_dir\"\n";
   expected << "replace_path=\"common/ideologies\"\n";
   expected << "replace_path=\"history/countries\"\n";
   expected << "replace_path=\"history/states\"\n";
   expected << "supported_version=\"1.11.*\"";

   EXPECT_EQ(mod_file_stream.str(), expected.str());

   ClearOutputFolder("test_output");
}


TEST(OutHoi4OutModTest, DescriptorFileIsCreated)
{
   Output("test_output");

   ASSERT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   std::ifstream descriptor_file("output/test_output.mod");
   ASSERT_TRUE(descriptor_file.is_open());
   std::stringstream descriptor_file_stream;
   std::copy(std::istreambuf_iterator<char>(descriptor_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(descriptor_file_stream));
   descriptor_file.close();

   std::stringstream expected;
   expected << "name = \"Converted - test_output\"\n";
   expected << "path = \"mod/test_output/\"\n";
   expected << "user_dir = \"test_output_user_dir\"\n";
   expected << "replace_path=\"common/ideologies\"\n";
   expected << "replace_path=\"history/countries\"\n";
   expected << "replace_path=\"history/states\"\n";
   expected << "supported_version=\"1.11.*\"";

   EXPECT_EQ(descriptor_file_stream.str(), expected.str());

   ClearOutputFolder("test_output");
}