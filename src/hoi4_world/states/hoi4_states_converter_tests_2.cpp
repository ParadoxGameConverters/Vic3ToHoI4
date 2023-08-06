#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/provinces/province_mapping_types.h"
#include "src/maps/map_data.h"
#include "src/vic3_world/states/vic3_state.h"

namespace hoi4
{


TEST(Hoi4worldStatesHoi4statesconverter, IndustryIsLogged)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 875'000)}},
           {2, std::vector{vic3::Building("", 2, 875'000)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {
           {1, DefaultState({.civilian_factories = 1})},
           {2, DefaultState({.military_factories = 2})},
           {3, DefaultState({.dockyards = 3})},
       },
       {},
       CoastalProvinces(),
       {},
       false);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \t\tTotal factories: 10 (vanilla hoi4 had 6)"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \t\t\tCivilian factories: 6 (vanilla hoi4 had 1)"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \t\t\tMilitary factories: 4 (vanilla hoi4 had 2)"));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \t\t\tDockyards: 0 (vanilla hoi4 had 3)"));
}
TEST(Hoi4worldStatesHoi4statesconverter, IndustryIsNotConvertedInUnownedStates)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 1'250'000)}},
           {2, std::vector{vic3::Building("", 2, 1'250'000)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .civilian_factories = 0, .military_factories = 0}),
           State(2, {.provinces = {40, 50, 60}, .civilian_factories = 0, .military_factories = 0})));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryIsCappedAtTwelve)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper(std::map<int, std::string>{{1, "ONE"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
       },
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 2'100'000)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "ONE", .provinces = {10, 20, 30}, .civilian_factories = 8, .military_factories = 4})));
}

TEST(Hoi4worldStatesHoi4statesconverter, StatesAreSortedByIndustry)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper(std::map<int, std::string>{{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5}})},
           {3, vic3::State({.owner_number = 2, .provinces = {6}})},
       },
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 350'000)}},
           {2, std::vector{vic3::Building("", 2, 2'100'000)}},
           {3, std::vector{vic3::Building("", 3, 1'575'000)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "TWO", .provinces = {40, 50}, .civilian_factories = 8, .military_factories = 4}),
           State(2, {.owner = "TWO", .provinces = {60}, .civilian_factories = 6, .military_factories = 3}),
           State(3, {.owner = "ONE", .provinces = {10, 20, 30}, .civilian_factories = 1, .military_factories = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, UnconvertedIndustryIsConvertedInNextStateOfSameOwner)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20"}},
               {"30", {"40"}},
               {"50", {"60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper(std::map<int, std::string>{{1, "ONE"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2}})},
           {2, vic3::State({.owner_number = 1, .provinces = {3, 4}})},
           {3, vic3::State({.owner_number = 1, .provinces = {5, 6}})},
       },
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 5'250'000)}},
           {2, std::vector{vic3::Building("", 2, 0)}},
           {3, std::vector{vic3::Building("", 3, 0)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "ONE", .provinces = {10, 20}, .civilian_factories = 3, .military_factories = 9}),
           State(2, {.owner = "ONE", .provinces = {30, 40}, .civilian_factories = 12, .military_factories = 0}),
           State(3, {.owner = "ONE", .provinces = {50, 60}, .civilian_factories = 6, .military_factories = 0})));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryInSplitStatesIsProportionalToTotalProvinces)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20"}},
               {"40", {"50"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
                                              {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})}},
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 700'000)}},
           {2, std::vector{vic3::Building("", 2, 1'575'000)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "TWO", .provinces = {40, 50}, .civilian_factories = 4, .military_factories = 2}),
           State(2, {.owner = "TWO", .provinces = {60}, .civilian_factories = 2, .military_factories = 1}),
           State(3, {.owner = "ONE", .provinces = {10, 20}, .civilian_factories = 2, .military_factories = 1}),
           State(4, {.owner = "ONE", .provinces = {30}, .civilian_factories = 0, .military_factories = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryInWastelandSplitStatesIsZero)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 875'000)}},
           {2, std::vector{vic3::Building("", 2, 875'000)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {
           {1, DefaultState({.provinces = {10, 20}})},
           {2, DefaultState({.impassable = true, .provinces = {30, 40, 50}})},
           {3, DefaultState({.provinces = {60}})},
       },
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "ONE", .provinces = {10, 20}, .civilian_factories = 3, .military_factories = 2}),
           State(2, {.owner = "ONE", .provinces = {30}, .category = "wasteland"}),
           State(3, {.owner = "TWO", .provinces = {60}, .civilian_factories = 3, .military_factories = 2}),
           State(4, {.owner = "TWO", .provinces = {40, 50}, .category = "wasteland"})));
}


TEST(Hoi4worldStatesHoi4statesconverter, NavalBasesAreConvertedInCoastalStates)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper(
       {
           {"0x000001", {10}},
           {"0x000002", {20}},
           {"0x000003", {30}},
           {"0x000004", {40}},
           {"0x000005", {50}},
           {"0x000006", {60}},
       },
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {
           {"REGION_ONE", vic3::StateRegion({{"0x000002", "port"}}, {})},
           {"REGION_TWO", vic3::StateRegion({{"0x000005", "port"}}, {})},
       },
       CoastalProvinces(std::map<int, std::vector<int>>{{20, {21}}, {50, {51}}}),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .victory_points = {{20, 1}},
                                    .naval_base_location = 20,
                                    .naval_base_level = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .victory_points = {{50, 1}},
                   .naval_base_location = 50,
                   .naval_base_level = 1})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ResourcesDefaultToEmpty)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
           province_definitions,
           {},
           {},
           province_mapper,
           map_data,
           hoi4_province_definitions,
           strategic_regions,
           country_mapper,
           StateCategories(),
           {},
           {},
           CoastalProvinces(),
           {},
           false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .resources = {}}),
           State(2, {.provinces = {40, 50, 60}, .resources = {}})));
}



TEST(Hoi4worldStatesHoi4statesconverter, ResourcesAreAssigned)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {
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
       },
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .resources = {{"test_resource", 5.0}}}),
           State(2, {.provinces = {40, 50, 60}, .resources = {{"test_resource", 7.0}, {"test_resource_two", 11.0}}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, CategoryDefaultsToRural)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .category = "rural"}),
           State(2, {.provinces = {40, 50, 60}, .category = "rural"})));
}


TEST(Hoi4worldStatesHoi4statesconverter, CategoriesAreSet)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 700'000)}},
           {2, std::vector{vic3::Building("", 2, 1'400'000)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories({
           {5, "test_category_one"},
           {7, "test_category_two"},
           {9, "test_category_three"},
           {11, "test_category_four"},
       }),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "TWO",
                                    .provinces = {40, 50, 60},
                                    .category = "test_category_four",
                                    .civilian_factories = 5,
                                    .military_factories = 3}),
           State(2,
               {.owner = "ONE",
                   .provinces = {10, 20, 30},
                   .category = "test_category_two",
                   .civilian_factories = 2,
                   .military_factories = 2})));
}


TEST(Hoi4worldStatesHoi4statesconverter, VictoryPointsDefaultToEmpty)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .victory_points = {}}),
           State(2, {.provinces = {40, 50, 60}, .victory_points = {}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, VictoryPointsArePlacedInHighestValueSignificantProvinces)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper(
       {
           {"0x000001", {10}},
           {"0x000002", {20}},
           {"0x000003", {30}},
           {"0x000004", {40}},
           {"0x000005", {50}},
           {"0x000006", {60}},
       },
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {
           {"STATE_ONE",
               vic3::StateRegion(
                   {
                       {"0x000005", "city"},
                       {"0x000004", "port"},
                       {"0x000003", "farm"},
                       {"0x000002", "mine"},
                       {"0x000001", "wood"},
                   },
                   {})},
       },
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .victory_points = {{30, 1}}}),
           State(2, {.provinces = {40, 50, 60}, .victory_points = {{50, 1}}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, VictoryPointsAreValuedAtHalfTotalFactoriesRoundedUp)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper(
       {
           {"0x000001", {10}},
           {"0x000002", {20}},
           {"0x000003", {30}},
           {"0x000004", {40}},
           {"0x000005", {50}},
           {"0x000006", {60}},
       },
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       vic3::Buildings({
           {1, std::vector{vic3::Building("", 1, 875'000)}},
           {2, std::vector{vic3::Building("", 2, 700'000)}},
       }),
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {
           {"STATE_ONE",
               vic3::StateRegion(
                   {
                       {"0x000005", "city"},
                       {"0x000004", "port"},
                       {"0x000003", "farm"},
                       {"0x000002", "mine"},
                       {"0x000001", "wood"},
                   },
                   {})},
       },
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .victory_points = {{30, 3}},
                                    .civilian_factories = 3,
                                    .military_factories = 2}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .victory_points = {{50, 2}},
                   .civilian_factories = 2,
                   .military_factories = 2})));
}


TEST(Hoi4worldStatesHoi4statesconverter, DebugVictoryPointsAreConverted)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper(
       {
           {"0x000001", {10}},
           {"0x000002", {20}},
           {"0x000003", {30}},
           {"0x000004", {40}},
           {"0x000005", {50}},
           {"0x000006", {60}},
       },
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {
           {"STATE_ONE",
               vic3::StateRegion(
                   {
                       {"0x000005", "city"},
                       {"0x000004", "port"},
                       {"0x000003", "farm"},
                       {"0x000002", "mine"},
                       {"0x000001", "wood"},
                   },
                   {})},
       },
       CoastalProvinces(),
       {},
       true);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .victory_points = {{10, 1}, {20, 2}, {30, 3}}}),
           State(2, {.provinces = {40, 50, 60}, .victory_points = {{40, 4}, {50, 5}}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, OwnerGetsCoreOnIncorporatedStates)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .incorporated = true, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .incorporated = true, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.owner = "ONE", .provinces = {10, 20, 30}, .cores = {"ONE"}}),
           State(2, {.owner = "TWO", .provinces = {40, 50, 60}, .cores = {"TWO"}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, InfrastructureIsTransferredFromVic3State)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .infrastructure = 123.4F, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .infrastructure = 567.8F, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       {},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {
           {"REGION_ONE", vic3::StateRegion({}, {"0x000001", "0x000002", "0x000003"})},
           {"REGION_TWO", vic3::StateRegion({}, {"0x000004", "0x000005", "0x000006"})},
       },
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.owner = "TAG", .provinces = {10, 20, 30}, .vic3_infrastructure = 123.4F}),
           State(2, {.owner = "TWO", .provinces = {40, 50, 60}, .vic3_infrastructure = 567.8F})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2),
           testing::Pair(60, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"), testing::Pair("STATE_2", "REGION_TWO")));
}
}