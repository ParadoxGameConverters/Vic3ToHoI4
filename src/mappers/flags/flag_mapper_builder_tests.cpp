#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/mappers/flags/flag_mapper_builder.h"


namespace mappers
{

TEST(MappersFlagsFlagMapperBuilder, FlagDirsAreCreated)
{
   FlagMapperBuilder builder("");
   EXPECT_TRUE(builder.CreateTargetFolders("FlagDirsAreCreated"));
   std::string base_folder("output/FlagDirsAreCreated");
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/gfx"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/gfx/flags"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/gfx/flags/small"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/gfx/flags/medium"));
}

TEST(MappersFlagsFlagMapperBuilder, ForbidIsRespected)
{
   FlagMapperBuilder builder("");
   builder.ReadConfig("configurables/flag_mapper_config.txt");
   EXPECT_TRUE(builder.CreateTargetFolders("ForbidIsRespected"));
   commonItems::ModFilesystem flag_mod("test_files/hoi4_world/flags", {});
   auto flag_mapper = builder.Build(flag_mod);
   // Forbidding one flag means we cannot find two.
   EXPECT_FALSE(flag_mapper.CopyFlags({"TAG", "Z00"}));
   // We should have copied the non-forbidden flag.
   ASSERT_TRUE(commonItems::DoesFileExist("output/ForbidIsRespected/gfx/flags/TAG.tga"));
   std::ifstream tag_file("output/ForbidIsRespected/gfx/flags/TAG.tga");
   ASSERT_TRUE(tag_file.is_open());
   std::stringstream tag_file_stream;
   std::copy(std::istreambuf_iterator<char>(tag_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(tag_file_stream));
   tag_file.close();
   EXPECT_EQ(tag_file_stream.str(),
       "# Another dummy flag file with different text so we can check the right one was copied.\n");
}

TEST(MappersFlagsFlagMapperBuilder, CustomFlagsAreUsed)
{
   FlagMapperBuilder builder("test_files/mappers/flags/");
   builder.ReadConfig("configurables/flag_mapper_config.txt");
   EXPECT_TRUE(builder.CreateTargetFolders("CustomFlagsAreUsed"));
   commonItems::ModFilesystem flag_mod("test_files/hoi4_world/flags", {});
   auto flag_mapper = builder.Build(flag_mod);
   // There is a custom flag for TAG, so we can find one for Z00.
   EXPECT_TRUE(flag_mapper.CopyFlags({"TAG", "Z00"}));
   // Check that the right flags are used.
   std::ifstream z00_file("output/CustomFlagsAreUsed/gfx/flags/Z00.tga");
   ASSERT_TRUE(z00_file.is_open());
   std::stringstream z00_file_stream;
   std::copy(std::istreambuf_iterator<char>(z00_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(z00_file_stream));
   z00_file.close();
   EXPECT_EQ(z00_file_stream.str(),
       "# Another dummy flag file with different text so we can check the right one was copied.\n");
   // Custom flags are copied by main mod creator.
   EXPECT_FALSE(commonItems::DoesFileExist("output/CustomFlagsAreUsed/gfx/flags/TAG.tga"));
}



}  // namespace mappers
