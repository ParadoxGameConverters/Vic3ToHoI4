#include "out_mod.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "external/googletest/googletest/include/gtest/gtest.h"



TEST(OutHoi4OutModTest, ModFolderIsCleared)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/test_output");
   EXPECT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   out::ClearOutputFolder("test_output");
   EXPECT_FALSE(commonItems::DoesFolderExist("output/test_output"));
}


TEST(OutHoi4OutModTest, FolderIsLoggedWhenCleared)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/test_output");

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   out::ClearOutputFolder("test_output");

   EXPECT_EQ(log.str(), "    [INFO] Removing pre-existing copy of test_output\n");

   std::cout.rdbuf(cout_buffer);
}


TEST(OutHoi4OutModTest, NoOperationOnMissingModFolder)
{
   EXPECT_FALSE(commonItems::DoesFolderExist("output/test_output"));

   out::ClearOutputFolder("test_output");
   EXPECT_FALSE(commonItems::DoesFolderExist("output/test_output"));
}


TEST(OutHoi4OutModTest, FolderIsNotLoggedWhenNotCleared)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   out::ClearOutputFolder("test_output");

   EXPECT_EQ(log.str(), "");

   std::cout.rdbuf(cout_buffer);
}


TEST(OutHoi4OutModTest, StatusIsLoggedWhenWritingMod)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   out::Output("test_output", GameVersion());

   EXPECT_EQ(log.str(),
       fmt::format("[PROGRESS] 80%\n"
                   "    [INFO] Outputting mod\n"
                   "    [INFO] \tCopying blank mod\n"
                   "    [INFO] \tCreating .mod files\n"
                   "[PROGRESS] 85%\n"));

   std::cout.rdbuf(cout_buffer);
   out::ClearOutputFolder("test_output");
   std::filesystem::remove("output/test_output.mod");
}


TEST(OutHoi4OutModTest, ModFolderIsCreated)
{
   out::Output("test_output", GameVersion());

   EXPECT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   out::ClearOutputFolder("test_output");
   std::filesystem::remove("output/test_output.mod");
}


TEST(OutHoi4OutModTest, ModFileIsCreated)
{
   out::Output("test_output", GameVersion());

   ASSERT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   std::ifstream mod_file("output/test_output.mod");
   ASSERT_TRUE(mod_file.is_open());
   std::stringstream mod_file_stream;
   std::copy(std::istreambuf_iterator<char>(mod_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(mod_file_stream));
   mod_file.close();

   EXPECT_EQ(mod_file_stream.str(),
       fmt::format("name = \"Converted - test_output\"\n"
                   "path = \"mod/test_output/\"\n"
                   "user_dir = \"test_output_user_dir\"\n"
                   "replace_path=\"common/ideologies\"\n"
                   "replace_path=\"history/countries\"\n"
                   "replace_path=\"history/states\"\n"
                   "supported_version=\"*\""));

   out::ClearOutputFolder("test_output");
   std::filesystem::remove("output/test_output.mod");
}


TEST(OutHoi4OutModTest, DescriptorFileIsCreated)
{
   out::Output("test_output", GameVersion());

   ASSERT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   std::ifstream descriptor_file("output/test_output.mod");
   ASSERT_TRUE(descriptor_file.is_open());
   std::stringstream descriptor_file_stream;
   std::copy(std::istreambuf_iterator<char>(descriptor_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(descriptor_file_stream));
   descriptor_file.close();

   EXPECT_EQ(descriptor_file_stream.str(),
       fmt::format("name = \"Converted - test_output\"\n"
                   "path = \"mod/test_output/\"\n"
                   "user_dir = \"test_output_user_dir\"\n"
                   "replace_path=\"common/ideologies\"\n"
                   "replace_path=\"history/countries\"\n"
                   "replace_path=\"history/states\"\n"
                   "supported_version=\"*\""));

   out::ClearOutputFolder("test_output");
   std::filesystem::remove("output/test_output.mod");
}


TEST(OutHoi4OutModTest, SupportedVersionIsFromSuppliedVersion)
{
   out::Output("test_output", GameVersion("42.13"));

   ASSERT_TRUE(commonItems::DoesFolderExist("output/test_output"));

   std::ifstream mod_file("output/test_output.mod");
   ASSERT_TRUE(mod_file.is_open());
   std::stringstream mod_file_stream;
   std::copy(std::istreambuf_iterator<char>(mod_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(mod_file_stream));
   mod_file.close();

   EXPECT_EQ(mod_file_stream.str(),
       fmt::format("name = \"Converted - test_output\"\n"
                   "path = \"mod/test_output/\"\n"
                   "user_dir = \"test_output_user_dir\"\n"
                   "replace_path=\"common/ideologies\"\n"
                   "replace_path=\"history/countries\"\n"
                   "replace_path=\"history/states\"\n"
                   "supported_version=\"42.13.*\""));

   std::ifstream descriptor_file("output/test_output.mod");
   ASSERT_TRUE(descriptor_file.is_open());
   std::stringstream descriptor_file_stream;
   std::copy(std::istreambuf_iterator<char>(descriptor_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(descriptor_file_stream));
   descriptor_file.close();

   EXPECT_EQ(descriptor_file_stream.str(),
       fmt::format("name = \"Converted - test_output\"\n"
                   "path = \"mod/test_output/\"\n"
                   "user_dir = \"test_output_user_dir\"\n"
                   "replace_path=\"common/ideologies\"\n"
                   "replace_path=\"history/countries\"\n"
                   "replace_path=\"history/states\"\n"
                   "supported_version=\"42.13.*\""));

   out::ClearOutputFolder("test_output");
   std::filesystem::remove("output/test_output.mod");
}