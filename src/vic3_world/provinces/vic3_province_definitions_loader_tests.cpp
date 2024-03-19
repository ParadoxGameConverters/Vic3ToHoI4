#include <sstream>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"



namespace vic3
{

TEST(Vic3WorldProvincesVic3ProvinceDefinitions, DefinitionsCanBeLoaded)
{
   const auto province_definitions = LoadProvinceDefinitions();

   EXPECT_THAT(province_definitions.GetProvinceDefinitions(),
       testing::ElementsAre("x000001", "x030000", "xABCDEF", "x000200"));
}


TEST(Vic3WorldProvincesVic3ProvinceDefinitions, BadDefinitionsAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto province_definitions = LoadProvinceDefinitions();

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Ignoring keyword: 0x000004)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Ignoring keyword: x12345)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Ignoring keyword: x1234567)"));

   std::cout.rdbuf(cout_buffer);
}

}  // namespace vic3