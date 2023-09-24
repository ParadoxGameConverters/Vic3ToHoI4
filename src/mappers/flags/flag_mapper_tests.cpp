#include <filesystem>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "src/mappers/flags/flag_mapper.h"


namespace
{

std::string CreateTestFolders(std::string_view test_name)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder(fmt::format("output/{}", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/gfx", test_name));
   return fmt::format("output/{}/gfx/flags", test_name);
}

}  // namespace



namespace mappers
{

TEST(MappersFlagsFlagMapper, FlagDirsAreCreated)
{
   const std::string base_folder = CreateTestFolders("FlagDirsAreCreated");
   FlagMapper flag_mapper(std::filesystem::path(base_folder), std::map<std::string, std::filesystem::path>{});
   EXPECT_TRUE(flag_mapper.CreateFlagFolders());
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/small"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder + "/medium"));
}


TEST(MappersFlagsFlagMapper, FlagFilesAreCopied)
{
   const std::string base_folder = CreateTestFolders("FlagFilesAreCopied");
   FlagMapper flag_mapper(std::filesystem::path(base_folder),
       std::map<std::string, std::filesystem::path>{
           {"ABC", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/ABC.tga")}});
   EXPECT_TRUE(flag_mapper.CreateFlagFolders());
   EXPECT_TRUE(flag_mapper.CopyFlag("TAG"));
   EXPECT_FALSE(flag_mapper.CopyFlag("Z00"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/small/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/medium/TAG.tga"));
}


}  // namespace mappers
