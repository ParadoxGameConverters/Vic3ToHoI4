#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/provinces/province_mapper.h"



namespace mappers
{

TEST(Mappers_Provinces_ProvinceMapperTests, Vic3ProvincesCanBeMappedToHoi4)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoI4_province_map{{"0x000001", {1, 10}},
       {"0x000200", {2}},
       {"0x002000", {2}},
       {"0x030000", {}}};
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map;
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x000001"), testing::ElementsAre(1, 10));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x000200"), testing::ElementsAre(2));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x002000"), testing::ElementsAre(2));
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x030000"), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetVic3ToHoi4ProvinceMapping("0x400000"), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, Hoi4ProvincesCanBeMappedToVic3)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoI4_province_map;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {"0x000001"}},
       {10, {"0x000001"}},
       {2, {"0x000200", "0x002000"}},
       {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(1), testing::ElementsAre("0x000001"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(10), testing::ElementsAre("0x000001"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(2), testing::ElementsAre("0x000200", "0x002000"));
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(3), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetHoi4ToVic3ProvinceMapping(4), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, Vic3ProvincesAreRecordedAsMapped)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoI4_province_map{{"0x000001", {}},
       {"0x000200", {}},
       {"0x002000", {}},
       {"0x030000", {}}};
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map;
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped("0x000001"));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped("0x000200"));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped("0x002000"));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped("0x030000"));
   EXPECT_FALSE(province_mappings.IsVic3ProvinceMapped("0x400000"));
}


TEST(Mappers_Provinces_ProvinceMapperTests, NoEquivalentProvincesToNoProvinces)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoI4_province_map;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {"0x000001"}},
       {10, {"0x000001"}},
       {2, {"0x000200", "0x002000"}},
       {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetEquivalentVic3Provinces({}), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, EquivalentProvincesAreReturned)
{
   Vic3ToHoi4ProvinceMapping vic3_to_hoI4_province_map;
   Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {"0x000001"}},
       {10, {"0x000001"}},
       {2, {"0x000200", "0x002000"}},
       {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetEquivalentVic3Provinces({4, 3, 2}), testing::ElementsAre("0x000200", "0x002000"));
}

}  // namespace mappers