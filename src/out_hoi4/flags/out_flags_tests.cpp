#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/out_hoi4/flags/out_flags.h"



using std::filesystem::path;



namespace out
{

TEST(OutHoI4FlagsOutFlags, FlagFilesAreCopied)
{
   std::map<std::string, hoi4::Country> countries{
       {"TAG", hoi4::Country({.tag = "TAG"})},
       {"Z00", hoi4::Country({.tag = "Z00"})},
   };
   commonItems::ModFilesystem flag_mod("test_files/hoi4_world/flags", {});
   OutputFlags("FlagFilesAreCopied", countries, flag_mod);
   path base_folder("output/FlagFilesAreCopied");
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder / "gfx"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder / "gfx/flags"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder / "gfx/flags/small"));
   EXPECT_TRUE(commonItems::DoesFolderExist(base_folder / "gfx/flags/medium"));

   std::ifstream z00_file(base_folder / "gfx/flags/Z00.tga");
   ASSERT_TRUE(z00_file.is_open());
   std::stringstream z00_file_stream;
   std::copy(std::istreambuf_iterator<char>(z00_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(z00_file_stream));
   z00_file.close();
   EXPECT_THAT(z00_file_stream.str(),
       testing::StartsWith("# Another dummy flag file with different text so we can check the right one was copied."));
   EXPECT_FALSE(commonItems::DoesFileExist("output/CustomFlagsAreUsed/gfx/flags/TAG.tga"));
}

}  // namespace out
