#include <string>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/mappers/world/world_mapper_builder.h"


namespace mappers
{

TEST(MappersWorldWorldMapperBuilderTests, NullBuilderOutputsNull)
{
   const auto nullMapper = WorldMapperBuilder::CreateNullMapper().Build();

   EXPECT_TRUE(nullMapper.country_mapper.GetCountryMappings().empty());
   EXPECT_TRUE(nullMapper.province_mapper.GetHoi4ToVic3ProvinceMappings().empty());
   EXPECT_TRUE(nullMapper.province_mapper.GetVic3ToHoi4ProvinceMappings().empty());
   EXPECT_TRUE(nullMapper.tech_mapper.empty());
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultBuilderOutputsDefaults)
{
   const vic3::World world(vic3::WorldOptions({.countries = {
                                                   {1, vic3::Country({.number = 1, .tag = "Z00"})},
                                               }}));
   const auto worldMapper =
       WorldMapperBuilder::CreateDefaultMapper(commonItems::ModFilesystem("test_files/hoi4_world", {}), world).Build();

   EXPECT_EQ(worldMapper.country_mapper.GetHoiTag(1).value(), "Z00");
   EXPECT_EQ(worldMapper.province_mapper.GetVic3ToHoi4ProvinceMapping("x002000").at(0), 2);
   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetTechs(),
       testing::UnorderedElementsAre("dest_tech_one", "dest_tech_two"));
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultCountryWorks)
{
   const vic3::World world(vic3::WorldOptions({.countries = {
                                                   {1, vic3::Country({.number = 1, .tag = "Z00"})},
                                               }}));
   const auto worldMapper = WorldMapperBuilder::CreateNullMapper().DefaultCountryMapper(world).Build();

   EXPECT_EQ(worldMapper.country_mapper.GetHoiTag(1).value(), "Z00");
}

TEST(MappersWorldWorldMapperBuilderTests, AddCountryWorks)
{
   const auto worldMapper =
       WorldMapperBuilder::CreateNullMapper().AddCountries({{1, "ONE"}}).AddCountries({{2, "TWO"}}).Build();

   EXPECT_EQ(worldMapper.country_mapper.GetHoiTag(1).value(), "ONE");
   EXPECT_EQ(worldMapper.country_mapper.GetHoiTag(2).value(), "TWO");
}

TEST(MappersWorldWorldMapperBuilderTests, AddProvincesWorks)
{
   const auto worldMapper =
       WorldMapperBuilder::CreateNullMapper().AddProvinces({{"x00000001", 10}, {"x00000002", 20}}).Build();

   EXPECT_THAT(worldMapper.province_mapper.GetVic3ToHoi4ProvinceMapping("x00000002"), testing::ElementsAre(20));
   EXPECT_THAT(worldMapper.province_mapper.GetHoi4ToVic3ProvinceMapping(10), testing::ElementsAre("x00000001"));
}

TEST(MappersWorldWorldMapperBuilderTests, AddTestProvincesWorks)
{
   const auto worldMapper = WorldMapperBuilder::CreateNullMapper().AddTestProvinces(2).Build();

   EXPECT_THAT(worldMapper.province_mapper.GetVic3ToHoi4ProvinceMapping("x000001"), testing::ElementsAre(10));
   EXPECT_THAT(worldMapper.province_mapper.GetHoi4ToVic3ProvinceMapping(20), testing::ElementsAre("x000002"));
}

TEST(MappersWorldWorldMapperBuilderTests, AddTechsWorks)
{
   const auto worldMapper = WorldMapperBuilder::CreateNullMapper()
                                .AddTechs({{{"vicTech1"}, std::nullopt, {"hoiTech1", "hoiTech2"}}})
                                .Build();

   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetTechs(), testing::UnorderedElementsAre("hoiTech1", "hoiTech2"));
   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetVic3Requirements(), testing::UnorderedElementsAre("vicTech1"));
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultTechsWorks)
{
   const auto worldMapper = WorldMapperBuilder::CreateNullMapper().DefaultTechMapper().Build();

   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetTechs(),
       testing::UnorderedElementsAre("dest_tech_one", "dest_tech_two"));
   EXPECT_THAT(worldMapper.tech_mapper.at(0).GetVic3Requirements(), testing::UnorderedElementsAre("source_tech"));
}

TEST(MappersWorldWorldMapperBuilderTests, AddCultureGraphicsMapperWorks)
{
   vic3::CultureDefinition culture = vic3::CultureDefinition("culture_a", {}, {}, {});
   mappers::GraphicsBlock graphics{.graphical_culture = "graphical_culture_a"};
   mappers::CultureGraphicsMapping mapping{
       .cultures = {culture.GetName()},
       .graphics_block = graphics,
   };
   CultureGraphicsMapper cultureGraphicsMapper = CultureGraphicsMapper({mapping});
   const auto worldMapper =
       WorldMapperBuilder::CreateNullMapper().SetCultureGraphicsMapper(cultureGraphicsMapper).Build();

   EXPECT_EQ(worldMapper.culture_graphics_mapper.MatchCultureToGraphics(culture).graphical_culture,
       "graphical_culture_a");
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultCultureGraphicsMapperWorks)
{
   vic3::CultureDefinition culture = vic3::CultureDefinition("culture_1", {}, {}, {});
   const auto worldMapper = WorldMapperBuilder::CreateNullMapper().DefaultCultureGraphicsMapper().Build();

   EXPECT_EQ(worldMapper.culture_graphics_mapper.MatchCultureToGraphics(culture).graphical_culture, "asian_gfx");
}

TEST(MappersWorldWorldMapperBuilderTests, CopyToVicWorldCopiesProvinces)
{
   auto worldMapper = std::move(WorldMapperBuilder::CreateNullMapper().AddTestProvinces(3));
   auto worldBuilder = vic3::WorldBuilder::CreateNullWorld();
   worldMapper.CopyToVicWorld(worldBuilder);
   auto world = worldBuilder.Build();
   EXPECT_EQ(world.GetProvinceDefinitions().GetProvinceDefinitions().size(), 3);
}

}  // namespace mappers