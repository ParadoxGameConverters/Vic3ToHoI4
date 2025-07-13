#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/provinces/province_mapper.h"



namespace mappers
{

TEST(Mappers_Provinces_ProvinceMapperTests, Vic3ProvincesCanBeMappedToHoi4)
{
   const Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map{{"x000001", {1, 10}},
       {"x000200", {2}},
       {"x002000", {2}},
       {"x030000", {}}};
   const Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map;
   const ProvinceMapper province_mappings(vic3_to_hoi4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("x000001"), testing::ElementsAre(1, 10));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("x000200"), testing::ElementsAre(2));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("x002000"), testing::ElementsAre(2));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("x030000"), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("x400000"), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, Hoi4ProvincesCanBeMappedToVic3)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {"x000001"}},
       {10, {"x000001"}},
       {2, {"x000200", "x002000"}},
       {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoi4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(1), testing::ElementsAre("x000001"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(10), testing::ElementsAre("x000001"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(2), testing::ElementsAre("x000200", "x002000"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(3), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(4), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, NoEquivalentProvincesToNoProvinces)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {"x000001"}},
       {10, {"x000001"}},
       {2, {"x000200", "x002000"}},
       {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoi4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetEquivalentVic3Provinces({}), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, EquivalentProvincesAreReturned)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {"x000001"}},
       {10, {"x000001"}},
       {2, {"x000200", "x002000"}},
       {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoi4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetEquivalentVic3Provinces({4, 3, 2}), testing::ElementsAre("x000200", "x002000"));
}


TEST(Mappers_Provinces_ProvinceMapperTests, MissingVic3ProvinceMappingLogsWarning)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {"x000001"}},
       {10, {"x000001"}},
       {2, {"x000200", "x002000"}},
       {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoi4_province_map, hoi4_to_vic3_province_map);

   std::stringstream log;
   auto std_out_buf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   province_mappings.CheckAllVic3ProvincesMapped({"x000001", "x000200", "x002000", "0x000012"});

   std::cout.rdbuf(std_out_buf);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No mapping for Vic3 province 0x000012"));
}


TEST(Mappers_Provinces_ProvinceMapperTests, NoLoggingWhenAllProvincesMapped)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map{{"x000001", {1, 10}},
       {"x000200", {2}},
       {"x002000", {2}},
       {"x030000", {}}};
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map;
   const ProvinceMapper province_mappings(vic3_to_hoi4_province_map, hoi4_to_vic3_province_map);

   std::stringstream log;
   auto std_out_buf = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   province_mappings.CheckAllVic3ProvincesMapped({"x000001", "x000200", "x002000"});

   std::cout.rdbuf(std_out_buf);

   EXPECT_TRUE(log.str().empty());
}

}  // namespace mappers