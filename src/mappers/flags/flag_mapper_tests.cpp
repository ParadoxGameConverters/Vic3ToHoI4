#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>

#include "src/mappers/flags/flag_mapper.h"



using std::filesystem::path;



namespace
{

path CreateTestFolders(std::string_view test_name)
{
   const path test_path(test_name);

   path flag_folder = "output" / test_path / "gfx/flags";
   remove_all(flag_folder);
   create_directories(flag_folder);
   create_directories(flag_folder / "small");
   create_directories(flag_folder / "medium");
   return flag_folder;
}

}  // namespace



namespace mappers
{

TEST(MappersFlagsFlagMapper, SingleFlagsAreCopied)
{
   const path base_folder = CreateTestFolders("SingleFlagsAreCopied");
   FlagMapper flag_mapper(base_folder,
       std::map<std::string, path>{
           {"ABC", "test_files/hoi4_world/flags/gfx/flags/ABC.tga"},
           {"DEF", "test_files/hoi4_world/flags/gfx/flags/DEF.tga"},
       },
       {});
   EXPECT_TRUE(flag_mapper.CopyFlag("TAG"));
   // Won't override existing flag.
   EXPECT_FALSE(flag_mapper.CopyFlag("DEF"));
   // Refuses to use DEF since it exists, so it's now out of flags.
   EXPECT_FALSE(flag_mapper.CopyFlag("Z00"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "small/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "medium/TAG.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder / "Z00.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder / "DEF.tga"));
}


TEST(MappersFlagsFlagMapper, MultipleFlagsAreCopied)
{
   const path base_folder = CreateTestFolders("MultipleFlagsAreCopied");
   FlagMapper flag_mapper(base_folder,
       std::map<std::string, path>{
           {"ABC", "test_files/hoi4_world/flags/gfx/flags/ABC.tga"},
           {"DEF", "test_files/hoi4_world/flags/gfx/flags/DEF.tga"},
       },
       {});
   auto tags = std::vector<std::string>{"TAG", "Z00"};
   EXPECT_TRUE(flag_mapper.CopyFlags(tags));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "small/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "medium/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "Z00.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "small/Z00.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "medium/Z00.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder / "DEF.tga"));
}


TEST(MappersFlagsFlagMapper, ExistingFlagsAreNotCopied)
{
   const path base_folder = CreateTestFolders("ExistingFlagsAreNotCopied");
   FlagMapper flag_mapper(base_folder,
       std::map<std::string, path>{
           {"ABC", "test_files/hoi4_world/flags/gfx/flags/ABC.tga"},
           {"DEF", "test_files/hoi4_world/flags/gfx/flags/DEF.tga"},
       },
       {});
   auto tags = std::vector<std::string>{"TAG", "Z00", "DEF"};
   // Will return false because there aren't enough flags for Z00.
   EXPECT_FALSE(flag_mapper.CopyFlags(tags));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "small/TAG.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "medium/TAG.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder / "Z00.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder / "DEF.tga"));
}


TEST(MappersFlagsFlagMapper, CustomFlagsAreNotCopied)
{
   const path base_folder = CreateTestFolders("CustomFlagsAreNotCopied");
   FlagMapper flag_mapper(base_folder,
       std::map<std::string, path>{
           {"ABC", "test_files/hoi4_world/flags/gfx/flags/ABC.tga"},
           {"DEF", "test_files/hoi4_world/flags/gfx/flags/DEF.tga"},
       },
       {"TAG"});
   auto tags = std::vector<std::string>{"TAG", "Z00", "DEF"};
   // TAG has a custom flag so Z00 can be assigned one.
   EXPECT_TRUE(flag_mapper.CopyFlags(tags));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "Z00.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "small/Z00.tga"));
   EXPECT_TRUE(commonItems::DoesFileExist(base_folder / "medium/Z00.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder / "TAG.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder / "ABC.tga"));
   EXPECT_FALSE(commonItems::DoesFileExist(base_folder / "DEF.tga"));
}

}  // namespace mappers
