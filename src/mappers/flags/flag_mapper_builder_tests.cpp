#include <filesystem>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
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

}  // namespace mappers
