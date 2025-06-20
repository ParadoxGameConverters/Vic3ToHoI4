#include "out_mod.h"

#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>
#include <gmock/gmock-matchers.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>



using std::filesystem::create_directories;
using std::filesystem::remove_all;


namespace out
{

TEST(OutHoi4OutModTest, ModFolderIsCleared)
{
   remove_all("output/test_output");
   ASSERT_TRUE(create_directories("output/test_output"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   ClearOutputFolder("test_output");

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] Removing pre-existing copy of test_output"));
   std::cout.rdbuf(cout_buffer);

   EXPECT_FALSE(commonItems::DoesFolderExist("output/test_output"));
}


TEST(OutHoi4OutModTest, FolderIsNotLoggedWhenNotCleared)
{
   EXPECT_FALSE(commonItems::DoesFolderExist("output/test_output"));

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   ClearOutputFolder("test_output");

   EXPECT_EQ(log.str(), "");

   std::cout.rdbuf(cout_buffer);
}


TEST(OutHoi4OutModTest, StatusIsLoggedWhenWritingMod)
{
   ClearOutputFolder("status_test_output");
   remove("output/status_test_output.mod");

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   OutputMod("status_test_output", GameVersion());

   EXPECT_THAT(log.str(), testing::HasSubstr("[PROGRESS] 80%"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] Outputting mod"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tCopying blank mod"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tCreating .mod files"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[PROGRESS] 85%"));

   std::cout.rdbuf(cout_buffer);
}


TEST(OutHoi4OutModTest, ModFolderIsCreated)
{
   ClearOutputFolder("mod_folder_test_output");
   remove("output/mod_folder_test_output.mod");
   OutputMod("mod_folder_test_output", GameVersion());

   EXPECT_TRUE(commonItems::DoesFolderExist("output/mod_folder_test_output"));
}


TEST(OutHoi4OutModTest, ModFileIsCreated)
{
   ClearOutputFolder("mod_file_test_output");
   remove("output/mod_file_test_output.mod");
   OutputMod("mod_file_test_output", GameVersion());

   ASSERT_TRUE(commonItems::DoesFolderExist("output/mod_file_test_output"));

   std::ifstream mod_file("output/mod_file_test_output.mod");
   ASSERT_TRUE(mod_file.is_open());
   std::stringstream mod_file_stream;
   std::copy(std::istreambuf_iterator<char>(mod_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(mod_file_stream));
   mod_file.close();

   EXPECT_EQ(mod_file_stream.str(),
       fmt::format("name = \"Converted - mod_file_test_output\"\n"
                   "path = \"mod/mod_file_test_output/\"\n"
                   "user_dir = \"mod_file_test_output_user_dir\"\n"
                   "replace_path=\"common/countries\"\n"
                   "replace_path=\"common/national_focus\"\n"
                   "replace_path=\"common/peace_conference/ai_peace\"\n"
                   "replace_path=\"common/peace_conference/cost_modifiers\"\n"
                   "replace_path=\"events\"\n"
                   "replace_path=\"history/countries\"\n"
                   "replace_path=\"history/states\"\n"
                   "replace_path=\"history/units\"\n"
                   "replace_path=\"map/supplyareas\"\n"
                   "replace_path=\"map/strategicregions\"\n"
                   "supported_version=\"*\""));
}


TEST(OutHoi4OutModTest, DescriptorFileIsCreated)
{
   ClearOutputFolder("descriptor_file_test_output");
   remove("output/descriptor_file_test_output.mod");
   OutputMod("descriptor_file_test_output", GameVersion());

   ASSERT_TRUE(commonItems::DoesFolderExist("output/descriptor_file_test_output"));

   std::ifstream descriptor_file("output/descriptor_file_test_output.mod");
   ASSERT_TRUE(descriptor_file.is_open());
   std::stringstream descriptor_file_stream;
   std::copy(std::istreambuf_iterator<char>(descriptor_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(descriptor_file_stream));
   descriptor_file.close();

   EXPECT_EQ(descriptor_file_stream.str(),
       fmt::format("name = \"Converted - descriptor_file_test_output\"\n"
                   "path = \"mod/descriptor_file_test_output/\"\n"
                   "user_dir = \"descriptor_file_test_output_user_dir\"\n"
                   "replace_path=\"common/countries\"\n"
                   "replace_path=\"common/national_focus\"\n"
                   "replace_path=\"common/peace_conference/ai_peace\"\n"
                   "replace_path=\"common/peace_conference/cost_modifiers\"\n"
                   "replace_path=\"events\"\n"
                   "replace_path=\"history/countries\"\n"
                   "replace_path=\"history/states\"\n"
                   "replace_path=\"history/units\"\n"
                   "replace_path=\"map/supplyareas\"\n"
                   "replace_path=\"map/strategicregions\"\n"
                   "supported_version=\"*\""));
}


TEST(OutHoi4OutModTest, SupportedVersionIsFromSuppliedVersion)
{
   ClearOutputFolder("version_test_output");
   remove("output/version_test_output.mod");
   OutputMod("version_test_output", GameVersion("42.13"));

   ASSERT_TRUE(commonItems::DoesFolderExist("output/version_test_output"));

   std::ifstream mod_file("output/version_test_output.mod");
   ASSERT_TRUE(mod_file.is_open());
   std::stringstream mod_file_stream;
   std::copy(std::istreambuf_iterator<char>(mod_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(mod_file_stream));
   mod_file.close();

   EXPECT_THAT(mod_file_stream.str(), testing::HasSubstr("supported_version=\"42.13.*\""));

   std::ifstream descriptor_file("output/version_test_output.mod");
   ASSERT_TRUE(descriptor_file.is_open());
   std::stringstream descriptor_file_stream;
   std::copy(std::istreambuf_iterator<char>(descriptor_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(descriptor_file_stream));
   descriptor_file.close();

   EXPECT_THAT(descriptor_file_stream.str(), testing::HasSubstr("supported_version=\"42.13.*\""));
}

}  // namespace out