#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/provinces/province_mapper.h"



namespace mappers
{

TEST(Mappers_Provinces_ProvinceMapperTests, Vic3ProvincesCanBeMappedToHoi4)
{
   Vic3ToHoI4ProvinceMapping vic3_to_hoI4_province_map{{1, {1, 10}}, {2, {2}}, {20, {2}}, {3, {}}};
   HoI4ToVic3ProvinceMapping hoi4_to_vic3_province_map;
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetVic3ToHoI4ProvinceMapping(1), testing::ElementsAre(1, 10));
   EXPECT_THAT(province_mappings.GetVic3ToHoI4ProvinceMapping(2), testing::ElementsAre(2));
   EXPECT_THAT(province_mappings.GetVic3ToHoI4ProvinceMapping(20), testing::ElementsAre(2));
   EXPECT_THAT(province_mappings.GetVic3ToHoI4ProvinceMapping(3), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetVic3ToHoI4ProvinceMapping(4), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, Hoi4ProvincesCanBeMappedToVic3)
{
   Vic3ToHoI4ProvinceMapping vic3_to_hoI4_province_map;
   HoI4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {1}}, {10, {1}}, {2, {2, 20}}, {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetHoI4ToVic3ProvinceMapping(1), testing::ElementsAre(1));
   EXPECT_THAT(province_mappings.GetHoI4ToVic3ProvinceMapping(10), testing::ElementsAre(1));
   EXPECT_THAT(province_mappings.GetHoI4ToVic3ProvinceMapping(2), testing::ElementsAre(2, 20));
   EXPECT_THAT(province_mappings.GetHoI4ToVic3ProvinceMapping(3), testing::ElementsAre());
   EXPECT_THAT(province_mappings.GetHoI4ToVic3ProvinceMapping(4), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, Vic3ProvincesAreRecordedAsMapped)
{
   Vic3ToHoI4ProvinceMapping vic3_to_hoI4_province_map{{1, {}}, {2, {}}, {20, {}}, {3, {}}};
   HoI4ToVic3ProvinceMapping hoi4_to_vic3_province_map;
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped(1));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped(2));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped(20));
   EXPECT_TRUE(province_mappings.IsVic3ProvinceMapped(3));
   EXPECT_FALSE(province_mappings.IsVic3ProvinceMapped(4));
}


TEST(Mappers_Provinces_ProvinceMapperTests, NoEquivalentProvincesToNoProvinces)
{
   Vic3ToHoI4ProvinceMapping vic3_to_hoI4_province_map;
   HoI4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {1}}, {10, {1}}, {2, {2, 20}}, {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetEquivalentVic3Provinces({}), testing::ElementsAre());
}


TEST(Mappers_Provinces_ProvinceMapperTests, EquivalentProvincesAreReturned)
{
   Vic3ToHoI4ProvinceMapping vic3_to_hoI4_province_map;
   HoI4ToVic3ProvinceMapping hoi4_to_vic3_province_map{{1, {1}}, {10, {1}}, {2, {2, 20}}, {3, {}}};
   const ProvinceMapper province_mappings(vic3_to_hoI4_province_map, hoi4_to_vic3_province_map);

   EXPECT_THAT(province_mappings.GetEquivalentVic3Provinces({4, 3, 2}), testing::ElementsAre(2, 20));
}

}  // namespace mappers