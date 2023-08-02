#include <string>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/mappers/world/world_mapper.h"

namespace mappers
{

TEST(MappersWorldWorldMapperBuilderTests, NullBuilderOutputsNull)
{
   const auto nullMapper = WorldMapperBuilder::NullMapper().Build();

   EXPECT_TRUE(nullMapper.country_mapper.GetCountryMappings().empty());
   EXPECT_TRUE(nullMapper.province_mapper.GetHoi4ToVic3ProvinceMappings().empty());
   EXPECT_TRUE(nullMapper.province_mapper.GetVic3ToHoi4ProvinceMappings().empty());
   EXPECT_TRUE(nullMapper.tech_mapper.empty());
}

TEST(MappersWorldWorldMapperBuilderTests, AddCountryWorks)
{
   const auto worldMapper =
       WorldMapperBuilder::NullMapper().AddCountries({{1, "ONE"}}).AddCountries({{2, "TWO"}}).Build();

   EXPECT_EQ(worldMapper.country_mapper.GetHoiTag(1).value(), "ONE");
   EXPECT_EQ(worldMapper.country_mapper.GetHoiTag(2).value(), "TWO");
}

TEST(MappersWorldWorldMapperBuilderTests, AddProvincesWorks)
{
   const auto worldMapper =
       WorldMapperBuilder::NullMapper().AddProvinces({{"0x00000001", 10}, {"0x00000002", 20}}).Build();

   EXPECT_THAT(worldMapper.province_mapper.GetVic3ToHoi4ProvinceMapping("0x00000002"), testing::ElementsAre(20));
   EXPECT_THAT(worldMapper.province_mapper.GetHoi4ToVic3ProvinceMapping(10), testing::ElementsAre("0x00000001"));
}

TEST(MappersWorldWorldMapperBuilderTests, AddTechsWorks)
{
   const auto worldMapper =
       WorldMapperBuilder::NullMapper().AddTechs({{{"vicTech1"}, std::nullopt, {"hoiTech1", "hoiTech2"}}}).Build();

   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetTechs(), testing::UnorderedElementsAre("hoiTech1", "hoiTech2"));
   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetVic3Requirements(), testing::UnorderedElementsAre("vicTech1"));
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultTechsWorks)
{
   const auto worldMapper =
       WorldMapperBuilder::NullMapper().DefaultTechMapper().Build();

   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetTechs(), testing::UnorderedElementsAre("dest_tech_one", "dest_tech_two"));
   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetVic3Requirements(), testing::UnorderedElementsAre("source_tech"));
}

}  // namespace mappers