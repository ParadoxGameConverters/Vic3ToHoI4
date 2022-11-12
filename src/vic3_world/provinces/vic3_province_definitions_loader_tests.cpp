#include <sstream>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"



namespace vic3
{

TEST(Vic3WorldProvincesVic3ProvinceDefinitions, DefinitionsCanBeLoaded)
{
   commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/provinces", {});
   const auto province_definitions = ProvinceDefinitionsLoader{}.LoadProvinceDefinitions(mod_filesystem);

   EXPECT_THAT(province_definitions.GetProvinceDefinitions(), testing::ElementsAre("x000001", "x000200"));
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitions, BadDefinitionsAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/provinces", {});
   const auto province_definitions = ProvinceDefinitionsLoader{}.LoadProvinceDefinitions(mod_filesystem);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Ignoring keyword: X030000)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Ignoring keyword: xabcdef)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Ignoring keyword: 0x000004)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Ignoring keyword: x12345)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Ignoring keyword: x1234567)"));

   std::cout.rdbuf(cout_buffer);
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitions, MissingFileThrowsException)
{
   commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/missing_provinces", {});

   EXPECT_THROW(const auto _ = ProvinceDefinitionsLoader{}.LoadProvinceDefinitions(mod_filesystem), std::runtime_error);
}

}  // namespace vic3