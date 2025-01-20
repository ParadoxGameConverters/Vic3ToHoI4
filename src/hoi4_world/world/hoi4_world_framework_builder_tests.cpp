#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/hoi4_world/world/hoi4_world_framework_builder.h"

namespace hoi4
{

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, NullFrameworkIsEmpty)
{
   WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().Build();

   EXPECT_TRUE(world_framework.default_states.empty());
   EXPECT_TRUE(world_framework.resources_map.empty());
   EXPECT_EQ(world_framework.state_categories.GetBestCategory(12), "rural");
   EXPECT_TRUE(world_framework.strategic_regions.GetStrategicRegions().empty());
   EXPECT_TRUE(world_framework.province_definitions.GetLandProvinces().empty());
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, DefaultFrameworkContainsDefaults)
{
   WorldFramework world_framework = WorldFrameworkBuilder::CreateDefaultWorldFramework(
       commonItems::ModFilesystem(std::filesystem::path("test_files/hoi4_world"), {}))
                                        .Build();

   EXPECT_TRUE(world_framework.default_states.empty());
   EXPECT_TRUE(world_framework.resources_map.empty());
   EXPECT_EQ(world_framework.state_categories.GetBestCategory(12), "megalopolis");
   EXPECT_THAT(world_framework.strategic_regions.GetStrategicRegions(),
       testing::UnorderedElementsAre(testing::Pair(42,
           StrategicRegion({.filename = "test_strategic_region.txt",
               .id = 42,
               .name = "STRATEGICREGION_42",
               .old_provinces = {144, 169},
               .static_modifiers = {{"test_modifier", "always"}, {"test_modifier_two", "always"}},
               .naval_terrain = "test_naval_terrain",
               .weather = "= {\n"
                          "\t\tperiod={\n"
                          "\t\t\tbetween={ 0.0 30.0 }\n"
                          "\t\t\ttemperature={ -6.0 12.0 }\n"
                          "\t\t\tno_phenomenon=0.500\n"
                          "\t\t\train_light=1.000\n"
                          "\t\t\train_heavy=0.150\n"
                          "\t\t\tsnow=0.200\n"
                          "\t\t\tblizzard=0.000\n"
                          "\t\t\tarctic_water=0.000\n"
                          "\t\t\tmud=0.300\n"
                          "\t\t\tsandstorm=0.000\n"
                          "\t\t\tmin_snow_level=0.000\n"
                          "\t\t}\n"
                          "\t\tperiod={\n"
                          "\t\t\tbetween={ 0.1 27.1 }\n"
                          "\t\t\ttemperature={ -7.0 12.0 }\n"
                          "\t\t\tno_phenomenon=0.500\n"
                          "\t\t\train_light=1.000\n"
                          "\t\t\train_heavy=0.150\n"
                          "\t\t\tsnow=0.200\n"
                          "\t\t\tblizzard=0.050\n"
                          "\t\t\tarctic_water=0.000\n"
                          "\t\t\tmud=0.300\n"
                          "\t\t\tsandstorm=0.000\n"
                          "\t\t\tmin_snow_level=0.000\n"
                          "\t\t}\n"
                          "\t}"}))));
   EXPECT_THAT(world_framework.strategic_regions.GetProvinceToStrategicRegionMap(),
       testing::UnorderedElementsAre(testing::Pair(144, 42), testing::Pair(169, 42)));
   EXPECT_THAT(world_framework.province_definitions.GetLandProvinces(),
       testing::ElementsAre("10", "20", "30", "40", "50", "60"));
   EXPECT_THAT(world_framework.province_definitions.GetSeaProvinces(), testing::ElementsAre("2"));
   EXPECT_TRUE(world_framework.coastal_provinces.GetCoastalProvinces().empty());
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, DefaultStrategicRegionsHasValues)
{
   WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().DefaultStrategicRegions().Build();
   EXPECT_THAT(world_framework.strategic_regions.GetStrategicRegions(),
       testing::UnorderedElementsAre(testing::Pair(42,
           StrategicRegion({.filename = "test_strategic_region.txt",
               .id = 42,
               .name = "STRATEGICREGION_42",
               .old_provinces = {144, 169},
               .static_modifiers = {{"test_modifier", "always"}, {"test_modifier_two", "always"}},
               .naval_terrain = "test_naval_terrain",
               .weather = "= {\n"
                          "\t\tperiod={\n"
                          "\t\t\tbetween={ 0.0 30.0 }\n"
                          "\t\t\ttemperature={ -6.0 12.0 }\n"
                          "\t\t\tno_phenomenon=0.500\n"
                          "\t\t\train_light=1.000\n"
                          "\t\t\train_heavy=0.150\n"
                          "\t\t\tsnow=0.200\n"
                          "\t\t\tblizzard=0.000\n"
                          "\t\t\tarctic_water=0.000\n"
                          "\t\t\tmud=0.300\n"
                          "\t\t\tsandstorm=0.000\n"
                          "\t\t\tmin_snow_level=0.000\n"
                          "\t\t}\n"
                          "\t\tperiod={\n"
                          "\t\t\tbetween={ 0.1 27.1 }\n"
                          "\t\t\ttemperature={ -7.0 12.0 }\n"
                          "\t\t\tno_phenomenon=0.500\n"
                          "\t\t\train_light=1.000\n"
                          "\t\t\train_heavy=0.150\n"
                          "\t\t\tsnow=0.200\n"
                          "\t\t\tblizzard=0.050\n"
                          "\t\t\tarctic_water=0.000\n"
                          "\t\t\tmud=0.300\n"
                          "\t\t\tsandstorm=0.000\n"
                          "\t\t\tmin_snow_level=0.000\n"
                          "\t\t}\n"
                          "\t}"}))));
   EXPECT_THAT(world_framework.strategic_regions.GetProvinceToStrategicRegionMap(),
       testing::UnorderedElementsAre(testing::Pair(144, 42), testing::Pair(169, 42)));
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, SetStrategicRegionsWorks)
{
   const StrategicRegions strategic_regions({
       .original_province_to_strategic_region_map =
           {
               {10, 1},
               {20, 1},
               {30, 1},
               {40, 2},
               {50, 2},
               {60, 2},
           },
   });
   WorldFramework world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().SetStrategicRegions(strategic_regions).Build();

   EXPECT_EQ(world_framework.strategic_regions.GetProvinceToStrategicRegionMap().at(10), 1);
   EXPECT_EQ(world_framework.strategic_regions.GetProvinceToStrategicRegionMap().at(40), 2);
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, AddDefaultStatesWorks)
{
   const std::map<int, DefaultState> default_states = {
       {1, DefaultState({.provinces = {10, 20}})},
       {2, DefaultState({.impassable = true, .provinces = {30, 40, 50}})},
       {3, DefaultState({.provinces = {60}})},
   };

   hoi4::WorldFramework world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddDefaultStates(default_states).Build();

   EXPECT_EQ(world_framework.default_states.at(2), DefaultState({.impassable = true, .provinces = {30, 40, 50}}));
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, AddResourcesWorks)
{
   WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework()
                                        .AddResources({
                                            /* 10 not included to force the continue */
                                            {
                                                20,
                                                {{"test_resource", 2.0}},
                                            },
                                            {
                                                30,
                                                {{"test_resource", 3.0}},
                                            },
                                            {40,
                                                {
                                                    {"test_resource", 7.0},
                                                    {"test_resource_two", 11.0},
                                                }},
                                        })
                                        .Build();

   EXPECT_EQ(world_framework.resources_map.at(20), Resources({{"test_resource", 2.0}}));
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, DefaultStateCategoriesHasValues)
{
   WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().DefaultStateCategories().Build();

   EXPECT_EQ(world_framework.state_categories.GetBestCategory(12), "megalopolis");
}
TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, SetStateCategoriesWorks)
{
   WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework()
                                        .SetStateCategories(hoi4::StateCategories({
                                            {5, "test_category_one"},
                                            {7, "test_category_two"},
                                            {9, "test_category_three"},
                                            {11, "test_category_four"},
                                        }))
                                        .Build();

   EXPECT_EQ(world_framework.state_categories.GetBestCategory(10), "test_category_four");
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, AddLandProvincesWorks)
{
   WorldFramework world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddLandProvinces({"10", "30"}).Build();

   EXPECT_THAT(world_framework.province_definitions.GetLandProvinces(), testing::ElementsAreArray({"10", "30"}));
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, AddTestLandProvincesWorks)
{
   WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(3).Build();

   EXPECT_THAT(world_framework.province_definitions.GetLandProvinces(), testing::ElementsAreArray({"10", "20", "30"}));
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, AddSeaProvincesWorks)
{
   WorldFramework world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddSeaProvinces({"10", "30"}).Build();

   EXPECT_THAT(world_framework.province_definitions.GetSeaProvinces(), testing::ElementsAreArray({"10", "30"}));
}

TEST(Hoi4worldWorldHoi4worldFrameworkBuilder, AddCoastalProvincesWorks)
{
   WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework()
                                        .AddTestLandProvinces(2)
                                        .AddSeaProvinces({"30"})
                                        .AddCoastalProvinces({{10, {30}}})
                                        .Build();

   EXPECT_TRUE(world_framework.coastal_provinces.contains(10));
}
}  // namespace hoi4