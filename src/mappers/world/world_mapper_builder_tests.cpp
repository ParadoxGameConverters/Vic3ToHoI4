#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <string>

#include "src/mappers/world/world_mapper_builder.h"



constexpr float kTolerance = 0.0001F;

namespace mappers
{

TEST(MappersWorldWorldMapperBuilderTests, NullBuilderOutputsNull)
{
   const auto null_mapper = WorldMapperBuilder::CreateNullMapper().Build();

   EXPECT_TRUE(null_mapper.country_mapper.GetCountryMappings().empty());
   EXPECT_TRUE(null_mapper.province_mapper.GetHoi4ToVic3ProvinceMappings().empty());
   EXPECT_TRUE(null_mapper.province_mapper.GetVic3ToHoi4ProvinceMappings().empty());
   EXPECT_TRUE(null_mapper.tech_mapper.empty());
   std::vector<vic3::Building> buildings{
       {"building_iron_mine", 1, 1.0F, 1.0F, {}},
   };
   EXPECT_NEAR(null_mapper.resource_mapper.CalculateScore("steel", buildings), 0.0F, kTolerance);
   EXPECT_NEAR(null_mapper.resource_mapper.CalculateScore("oil", buildings), 0.0F, kTolerance);
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultBuilderOutputsDefaults)
{
   const vic3::World world(vic3::WorldOptions({.countries = {
                                                   {1, vic3::Country({.number = 1, .tag = "Z00"})},
                                               }}));
   const auto world_mapper =
       WorldMapperBuilder::CreateDefaultMapper(commonItems::ModFilesystem("test_files/hoi4_world", {}), world).Build();

   EXPECT_EQ(world_mapper.country_mapper.GetHoiTag(1).value_or(""), "Z00");
   EXPECT_EQ(world_mapper.province_mapper.GetVic3ToHoi4ProvinceMapping("x002000").at(0), 2);
   EXPECT_THAT(world_mapper.tech_mapper.at(0).GetTechs(),
       testing::UnorderedElementsAre("dest_tech_one", "dest_tech_two"));
}

TEST(MappersWorldWorldMapperBuilderTests, LoadResourceMappingWorks)
{
   const vic3::World world(vic3::WorldOptions({.countries = {
                                                   {1, vic3::Country({.number = 1, .tag = "Z00"})},
                                               }}));
   auto builder =
       WorldMapperBuilder::CreateDefaultMapper(commonItems::ModFilesystem("test_files/hoi4_world", {}), world);
   builder.LoadResourceMapper("test_files/configurables/resource_mappings.txt");

   const auto world_mapper = builder.Build();
   std::vector<vic3::Building> buildings{
       {"building_iron_mine", 1, 1.0F, 1.0F, {}},
       {"building_oil_rig", 1, 0.8F, 1.0F, {}},
       {"building_coal_mine", 1, 0.2F, 1.0F, {}},
       {"building_steel_factory", 1, 0.8F, 1.0F, {}},
   };
   EXPECT_NEAR(world_mapper.resource_mapper.CalculateScore("steel", buildings), 1.1F, kTolerance);
   EXPECT_NEAR(world_mapper.resource_mapper.CalculateScore("oil", buildings), 0.48F, kTolerance);
   EXPECT_NEAR(world_mapper.resource_mapper.CalculateScore("tungsten", buildings), 0.0F, kTolerance);
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultCountryWorks)
{
   const vic3::World world(vic3::WorldOptions({.countries = {
                                                   {1, vic3::Country({.number = 1, .tag = "Z00"})},
                                               }}));
   const auto world_mapper = WorldMapperBuilder::CreateNullMapper().DefaultCountryMapper(world).Build();

   EXPECT_EQ(world_mapper.country_mapper.GetHoiTag(1).value_or(""), "Z00");
}

TEST(MappersWorldWorldMapperBuilderTests, AddCountryWorks)
{
   const auto world_mapper =
       WorldMapperBuilder::CreateNullMapper().AddCountries({{1, "ONE"}}).AddCountries({{2, "TWO"}}).Build();

   EXPECT_EQ(world_mapper.country_mapper.GetHoiTag(1).value_or(""), "ONE");
   EXPECT_EQ(world_mapper.country_mapper.GetHoiTag(2).value_or(""), "TWO");
}

TEST(MappersWorldWorldMapperBuilderTests, AddProvincesWorks)
{
   const auto world_mapper =
       WorldMapperBuilder::CreateNullMapper().AddProvinces({{"x00000001", 10}, {"x00000002", 20}}).Build();

   EXPECT_THAT(world_mapper.province_mapper.GetVic3ToHoi4ProvinceMapping("x00000002"), testing::ElementsAre(20));
   EXPECT_THAT(world_mapper.province_mapper.GetHoi4ToVic3ProvinceMapping(10), testing::ElementsAre("x00000001"));
}

TEST(MappersWorldWorldMapperBuilderTests, AddTestProvincesWorks)
{
   const auto world_mapper = WorldMapperBuilder::CreateNullMapper().AddTestProvinces(2).Build();

   EXPECT_THAT(world_mapper.province_mapper.GetVic3ToHoi4ProvinceMapping("x000001"), testing::ElementsAre(10));
   EXPECT_THAT(world_mapper.province_mapper.GetHoi4ToVic3ProvinceMapping(20), testing::ElementsAre("x000002"));
}

TEST(MappersWorldWorldMapperBuilderTests, AddTechsWorks)
{
   const auto world_mapper = WorldMapperBuilder::CreateNullMapper()
                                 .AddTechs({{{"vicTech1"}, std::nullopt, {"hoiTech1", "hoiTech2"}}})
                                 .Build();

   EXPECT_THAT(world_mapper.tech_mapper.at(0).GetTechs(), testing::UnorderedElementsAre("hoiTech1", "hoiTech2"));
   EXPECT_THAT(world_mapper.tech_mapper.at(0).GetVic3Requirements(), testing::UnorderedElementsAre("vicTech1"));
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultTechsWorks)
{
   const auto world_mapper = WorldMapperBuilder::CreateNullMapper().DefaultTechMapper().Build();

   EXPECT_THAT(world_mapper.tech_mapper.at(0).GetTechs(),
       testing::UnorderedElementsAre("dest_tech_one", "dest_tech_two"));
   EXPECT_THAT(world_mapper.tech_mapper.at(0).GetVic3Requirements(), testing::UnorderedElementsAre("source_tech"));
}

TEST(MappersWorldWorldMapperBuilderTests, AddCultureGraphicsMapperWorks)
{
   vic3::CultureDefinition culture = vic3::CultureDefinition("culture_a", {}, {}, {});
   mappers::GraphicsBlock graphics{.graphical_culture = "graphical_culture_a"};
   mappers::CultureGraphicsMapping mapping{
       .cultures = {culture.GetName()},
       .graphics_block = graphics,
   };
   CultureGraphicsMapper culture_graphics_mapper = CultureGraphicsMapper({mapping});
   const auto world_mapper =
       WorldMapperBuilder::CreateNullMapper().SetCultureGraphicsMapper(culture_graphics_mapper).Build();

   EXPECT_EQ(world_mapper.culture_graphics_mapper.MatchCultureToGraphics(culture).graphical_culture,
       "graphical_culture_a");
}

TEST(MappersWorldWorldMapperBuilderTests, DefaultCultureGraphicsMapperWorks)
{
   vic3::CultureDefinition culture = vic3::CultureDefinition("culture_1", {}, {}, {});
   const auto world_mapper = WorldMapperBuilder::CreateNullMapper().DefaultCultureGraphicsMapper().Build();

   EXPECT_EQ(world_mapper.culture_graphics_mapper.MatchCultureToGraphics(culture).graphical_culture, "asian_gfx");
}

TEST(MappersWorldWorldMapperBuilderTests, CopyToVicWorldCopiesProvinces)
{
   auto world_mapper = std::move(WorldMapperBuilder::CreateNullMapper().AddTestProvinces(3));
   auto world_builder = vic3::WorldBuilder::CreateNullWorld();
   world_mapper.CopyToVicWorld(world_builder);
   auto world = world_builder.Build();
   EXPECT_EQ(world.GetProvinceDefinitions().GetProvinceDefinitions().size(), 3);
}

}  // namespace mappers
