#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/mappers/flags/flag_mapper_builder.h"


namespace mappers
{

TEST(MappersFlagsFlagMapperBuilder, FlagDirsAreCreated)
{
   FlagMapperBuilder builder;
   EXPECT_TRUE(builder.CreateTargetFolders("FlagDirsAreCreated"));
   std::string base_folder("output/FlagDirsAreCreated");
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/gfx"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/gfx/flags"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/gfx/flags/small"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/gfx/flags/medium"));
}

TEST(MappersFlagsFlagMapperBuilder, ForbidIsRespectedAndCustomIsUsed)
{
   FlagMapperBuilder builder;
   builder.ReadConfig("configurables/flag_mapper_config.txt");
   EXPECT_TRUE(builder.CreateTargetFolders("ForbidIsRespectedAndCustomIsUsed"));
   commonItems::ModFilesystem flag_mod("test_files/hoi4_world/flags", {});
   auto flag_mapper = builder.Build(flag_mod);
   // There is a custom flag for TAG, so we can find one for Z00.
   EXPECT_TRUE(flag_mapper.CopyFlags({"TAG", "Z00"}));
   // Check that the allowed flag was copied.
   std::ifstream z00_file("output/ForbidIsRespectedAndCustomIsUsed/gfx/flags/Z00.tga");
   ASSERT_TRUE(z00_file.is_open());
   std::stringstream z00_file_stream;
   std::copy(std::istreambuf_iterator<char>(z00_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(z00_file_stream));
   z00_file.close();
   EXPECT_THAT(z00_file_stream.str(),
       testing::StartsWith("# Another dummy flag file with different text so we can check the right one was copied."));
   // Custom flags are copied by main mod creator.
   EXPECT_FALSE(commonItems::DoesFileExist("output/CustomFlagsAreUsed/gfx/flags/TAG.tga"));
}

}  // namespace mappers
