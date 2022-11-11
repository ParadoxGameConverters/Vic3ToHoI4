#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/provinces/province_mapper_importer.h"



TEST(MappersProvincesProvinceMapperImporterTests, ProvinceMappingsCanBeImported)
{
   commonItems::ModFilesystem mod_filesystem("./test_files/mappers/provinces/empty_definition/", {});
   const auto province_mappings = mappers::ProvinceMapperImporter{mod_filesystem}.ImportProvinceMappings();

   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x000001"), testing::ElementsAre(1, 10));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x000200"), testing::ElementsAre(2));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x002000"), testing::ElementsAre(2));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x030000"), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x400000"), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(1), testing::ElementsAre("0x000001"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(10), testing::ElementsAre("0x000001"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(2), testing::ElementsAre("0x000200", "0x002000"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(3), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(4), testing::ElementsAre());
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped("0x000001"));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped("0x000200"));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped("0x002000"));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped("0x030000"));
   EXPECT_FALSE(province_mappings.IsVic3ProvinceMapped("0x400000"));
}


TEST(MappersProvincesProvinceMapperImporterTests, MissingMapDefinitionThrowsException)
{
   commonItems::ModFilesystem mod_filesystem("./test_files/mappers/provinces/no_definition/", {});
   EXPECT_THROW(const auto province_mappings = mappers::ProvinceMapperImporter{mod_filesystem}.ImportProvinceMappings(),
       std::runtime_error);
}


TEST(MappersProvincesProvinceMapperImporterTests, BadLineInMapDefinitionLogsWarning)
{
   std::stringstream log;
   auto stdOutBuf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   commonItems::ModFilesystem mod_filesystem("./test_files/mappers/provinces/bad_line_definition/", {});
   auto _ = mappers::ProvinceMapperImporter{mod_filesystem}.ImportProvinceMappings();
   std::cout.rdbuf(stdOutBuf);

   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Bad line in /map/definition.csv: bad_line;0;0;0;land;false;unknown;0"));
}


TEST(MappersProvincesProvinceMapperImporterTests, MissingHoi4ProvinceMappingLogsWarning)
{
   std::stringstream log;
   auto stdOutBuf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   commonItems::ModFilesystem mod_filesystem("./test_files/mappers/provinces/missing_definition/", {});
   const auto province_mappings = mappers::ProvinceMapperImporter{mod_filesystem}.ImportProvinceMappings();
   const auto _ = province_mappings.GetHoi4ToVic3ProvinceMapping(12);

   std::cout.rdbuf(stdOutBuf);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No mapping for Hoi4 province 12"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No mapping found for Hoi4 province 12"));
}


TEST(MappersProvincesProvinceMapperImporterTests, MissingVic3ProvinceMappingLogsWarning)
{
   commonItems::ModFilesystem mod_filesystem("./test_files/mappers/provinces/empty_definition/", {});
   const auto province_mappings = mappers::ProvinceMapperImporter{mod_filesystem}.ImportProvinceMappings();

   std::stringstream log;
   auto stdOutBuf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto _ = province_mappings.GetVic3ToHoi4ProvinceMapping("0x000012");

   std::cout.rdbuf(stdOutBuf);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No mapping found for Vic3 province 0x000012"));
}


TEST(MappersProvincesProvinceMapperImporterTests, NoLoggingWhenAllProvincesMapped)
{
   std::stringstream log;
   auto stdOutBuf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   commonItems::ModFilesystem mod_filesystem("./test_files/mappers/provinces/good_definition/", {});
   const auto _ = mappers::ProvinceMapperImporter{mod_filesystem}.ImportProvinceMappings();
   std::cout.rdbuf(stdOutBuf);

   EXPECT_TRUE(log.str().empty());
}