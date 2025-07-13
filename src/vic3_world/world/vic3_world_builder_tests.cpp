#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/vic3_world/world/vic3_world.h"
#include "src/vic3_world/world/vic3_world_builder.h"



namespace vic3
{

TEST(Vic3WorldWorldVic3WorldBuilder, NullBuilderOutputsNull)
{
   const auto null_world = WorldBuilder::CreateNullWorld().Build();

   EXPECT_TRUE(null_world.GetCountries().empty());
   EXPECT_TRUE(null_world.GetProvinceDefinitions().GetProvinceDefinitions().empty());
}

TEST(Vic3WorldWorldVic3WorldBuilder, AddProvincesWorks)
{
   const auto null_world = WorldBuilder::CreateNullWorld().AddProvinces({{"prov1"}, {"prov2"}}).Build();

   EXPECT_EQ(null_world.GetProvinceDefinitions().GetProvinceDefinitions().at(0), "prov1");
   EXPECT_EQ(null_world.GetProvinceDefinitions().GetProvinceDefinitions().at(1), "prov2");
}

TEST(Vic3WorldWorldVic3WorldBuilder, AddStatesWorks)
{
   const auto null_world =
       WorldBuilder::CreateNullWorld().AddStates({{1, State({.owner_number = 1, .provinces = {1, 2, 3}})}}).Build();

   EXPECT_EQ(null_world.GetStates().at(1), State({.owner_number = 1, .provinces = {1, 2, 3}}));
}

TEST(Vic3WorldWorldVic3WorldBuilder, AddTestStatesWorks)
{
   const auto null_world = WorldBuilder::CreateNullWorld().AddTestStates({{1, 2, 3}, {4, 5}}).Build();

   EXPECT_EQ(null_world.GetStates().at(1), State({.id = 1, .owner_number = 1, .provinces = {1, 2, 3}}));
   EXPECT_EQ(null_world.GetStates().at(2), State({.id = 2, .owner_number = 2, .provinces = {4, 5}}));
}

TEST(Vic3WorldWorldVic3WorldBuilder, AddStateRegionsWorks)
{
   const auto null_world = WorldBuilder::CreateNullWorld()
                               .AddStateRegions({
                                   {"REGION_ONE", vic3::StateRegion({{"x000002", "port"}}, {})},
                                   {"REGION_TWO", vic3::StateRegion({{"x000005", "port"}}, {})},
                               })
                               .Build();

   EXPECT_EQ(null_world.GetStateRegions().at("REGION_ONE"), vic3::StateRegion({{"x000002", "port"}}, {}));
   EXPECT_EQ(null_world.GetStateRegions().at("REGION_TWO"), vic3::StateRegion({{"x000005", "port"}}, {}));
}

TEST(Vic3WorldWorldVic3WorldBuilder, AddTestStateRegionsWorks)
{
   const auto null_world = WorldBuilder::CreateNullWorld().AddTestStateRegions({{1, 2, 3}, {4, 5}}).Build();

   EXPECT_EQ(null_world.GetStateRegions().at("REGION_001"), vic3::StateRegion({}, {"x000001", "x000002", "x000003"}));
   EXPECT_EQ(null_world.GetStateRegions().at("REGION_002"), vic3::StateRegion({}, {"x000004", "x000005"}));
}

TEST(Vic3WorldWorldVic3WorldBuilder, AddBuildingsWorks)
{
   const auto null_world = WorldBuilder::CreateNullWorld()
                               .AddBuildings({vic3::Building("", 1, 700'000), vic3::Building("", 2, 1'400'000)})
                               .Build();

   EXPECT_EQ(null_world.GetBuildings().GetTotalGoodSalesValueInState(1), 700'000);
   EXPECT_EQ(null_world.GetBuildings().GetTotalGoodSalesValueInState(2), 1'400'000);
}

}  // namespace vic3