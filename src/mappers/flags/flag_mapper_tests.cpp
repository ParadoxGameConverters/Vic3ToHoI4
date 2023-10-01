#include <filesystem>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/mappers/flags/flag_mapper.h"


namespace
{

std::string CreateTestFolders(std::string_view test_name)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder(fmt::format("output/{}", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/gfx", test_name));
   auto flag_folder = fmt::format("output/{}/gfx/flags", test_name);
   commonItems::TryCreateFolder(flag_folder);
   commonItems::TryCreateFolder(flag_folder + "/small");
   commonItems::TryCreateFolder(flag_folder + "/medium");
   return flag_folder;
}

}  // namespace



namespace mappers
{

TEST(MappersFlagsFlagMapper, SingleFlagsAreCopied)
{
   const std::string base_folder = CreateTestFolders("SingleFlagsAreCopied");
   FlagMapper flag_mapper(std::filesystem::path(base_folder),
       std::map<std::string, std::filesystem::path>{
           {"ABC", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/ABC.tga")},
           {"DEF", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/DEF.tga")},
       },
       {});
   EXPECT_TRUE(flag_mapper.CopyFlag("TAG"));
   // Won't override existing flag.
   EXPECT_FALSE(flag_mapper.CopyFlag("DEF"));
   // Refuses to use DEF since it exists, so it's now out of flags.
   EXPECT_FALSE(flag_mapper.CopyFlag("Z00"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/small/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/medium/TAG.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder + "/Z00.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder + "/DEF.tga"));
}

TEST(MappersFlagsFlagMapper, MultipleFlagsAreCopied)
{
   const std::string base_folder = CreateTestFolders("MultipleFlagsAreCopied");
   FlagMapper flag_mapper(std::filesystem::path(base_folder),
       std::map<std::string, std::filesystem::path>{
           {"ABC", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/ABC.tga")},
           {"DEF", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/DEF.tga")},
       },
       {});
   auto tags = std::vector<std::string>{"TAG", "Z00"};
   EXPECT_TRUE(flag_mapper.CopyFlags(tags));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/small/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/medium/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/Z00.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/small/Z00.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/medium/Z00.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder + "/DEF.tga"));
}

TEST(MappersFlagsFlagMapper, ExistingFlagsAreNotCopied)
{
   const std::string base_folder = CreateTestFolders("ExistingFlagsAreNotCopied");
   FlagMapper flag_mapper(std::filesystem::path(base_folder),
       std::map<std::string, std::filesystem::path>{
           {"ABC", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/ABC.tga")},
           {"DEF", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/DEF.tga")},
       },
       {});
   auto tags = std::vector<std::string>{"TAG", "Z00", "DEF"};
   // Will return false because there aren't enough flags for Z00.
   EXPECT_FALSE(flag_mapper.CopyFlags(tags));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/small/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/medium/TAG.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder + "/Z00.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder + "/DEF.tga"));
}

TEST(MappersFlagsFlagMapper, CustomFlagsAreNotCopied)
{
   const std::string base_folder = CreateTestFolders("CustomFlagsAreNotCopied");
   FlagMapper flag_mapper(std::filesystem::path(base_folder),
       std::map<std::string, std::filesystem::path>{
           {"ABC", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/ABC.tga")},
           {"DEF", std::filesystem::path("test_files/hoi4_world/flags/gfx/flags/DEF.tga")}},
       {"TAG"});
   auto tags = std::vector<std::string>{"TAG", "Z00", "DEF"};
   // TAG has a custom flag so Z00 can be assigned one.
   EXPECT_TRUE(flag_mapper.CopyFlags(tags));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/Z00.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/small/Z00.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder + "/medium/Z00.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder + "/TAG.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder + "/ABC.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder + "/DEF.tga"));
}


}  // namespace mappers
