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
#include "src/vic3_world/world/vic3_world.h"
#include "src/mappers/world/world_mapper.h"
#include "src/hoi4_world/world/hoi4_world.h"



namespace hoi4
{

TEST(Hoi4worldStatesHoi4statesconverter, NoStatesConvertToNoStates)
{
   const maps::ProvinceDefinitions hoi4_province_definitions;
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});

   vic3::World world({});
   mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper().Build();
   hoi4::WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().Build();

   const auto hoi4_states = ConvertStates2(world,
      world_mapper,
       world_framework,
       {},
       map_data,
       hoi4_province_definitions,
       CoastalProvinces(),
       false);

   EXPECT_TRUE(hoi4_states.states.empty());
   EXPECT_TRUE(hoi4_states.province_to_state_id_map.empty());
   EXPECT_TRUE(hoi4_states.vic3_state_ids_to_hoi4_state_ids.empty());
   EXPECT_TRUE(hoi4_states.hoi4_state_names_to_vic3_state_names.empty());
}


TEST(Hoi4worldStatesHoi4statesconverter, StatesAreConverted)
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
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}})}},
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
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}}), State(2, {.provinces = {40, 50, 60}})));
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


TEST(Hoi4worldStatesHoi4statesconverter, SplitProvincesGoToCityandPortsOwnersStates)
{
   const vic3::ProvinceDefinitions province_definitions({
       "x000001",
       "x000002",
       "x000003",
       "x000004",
       "x000005",
       "x000006",
   });
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"x000001", "x000002", "x000003"}},
           {20, {"x000004", "x000005", "x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20"}});
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}, {3, "THR"}, {4, "FOR"}});

   const auto hoi4_states = ConvertStates({{1, vic3::State({.owner_number = 1, .provinces = {1}})},
                                              {2, vic3::State({.owner_number = 2, .provinces = {2, 3}})},
                                              {3, vic3::State({.owner_number = 3, .provinces = {4}})},
                                              {4, vic3::State({.owner_number = 4, .provinces = {5, 6}})}},
       province_definitions,
       {{"x000001", "city"}, {"x000004", "port"}},
       {},
       province_mapper,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {
           {"REGION_ONE", vic3::StateRegion({}, {"x000001", "x000002", "x000003"})},
           {"REGION_TWO", vic3::StateRegion({}, {"x000004", "x000005", "x000006"})},
       },
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.owner = "ONE", .provinces = {10}}),
           State(2, {.owner = "THR", .provinces = {20}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1), testing::Pair(20, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(3, 2)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"), testing::Pair("STATE_2", "REGION_TWO")));
}


TEST(Hoi4worldStatesHoi4statesconverter, SplitProvincesGoToMajorityState)
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
           {10, {"0x000001", "0x000002", "0x000003"}},
           {20, {"0x000004", "0x000005", "0x000006"}},
       });
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20"}});
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {4, "FOR"}});

   const auto hoi4_states = ConvertStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2}})},
                                              {2, vic3::State({.owner_number = 2, .provinces = {3}})},
                                              {3, vic3::State({.owner_number = 3, .provinces = {4}})},
                                              {4, vic3::State({.owner_number = 4, .provinces = {5, 6}})}},
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
       testing::ElementsAre(State(1, {.owner = "ONE", .provinces = {10}}),
           State(2, {.owner = "FOR", .provinces = {20}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1), testing::Pair(20, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(4, 2)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"), testing::Pair("STATE_2", "REGION_TWO")));
}


TEST(Hoi4worldStatesHoi4statesconverter, BadNeighborStringsAreSkipped)
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
               {"10", {"20", "abc30"}},
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
           {
               {"REGION_ONE", vic3::StateRegion({}, {"0x000001", "0x000002", "0x000003"})},
               {"REGION_TWO", vic3::StateRegion({}, {"0x000004", "0x000005", "0x000006"})},
           },
           CoastalProvinces(),
           {},
           false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20}}),
           State(2, {.provinces = {30}}),
           State(3, {.provinces = {40, 50, 60}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 2),
           testing::Pair(40, 3),
           testing::Pair(50, 3),
           testing::Pair(60, 3)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 3)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"),
           testing::Pair("STATE_2", "REGION_ONE"),
           testing::Pair("STATE_3", "REGION_TWO")));
}


TEST(Hoi4worldStatesHoi4statesconverter, DisconnectedStatesAreSplit)
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
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

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
           {
               {"REGION_ONE", vic3::StateRegion({}, {"0x000001", "0x000002", "0x000003"})},
               {"REGION_TWO", vic3::StateRegion({}, {"0x000004", "0x000005", "0x000006"})},
           },
           CoastalProvinces(),
           {},
           false);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tState 1 was split into 3 due to disconnected provinces."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tState 4 was split into 3 due to disconnected provinces."));
   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10}}),
           State(2, {.provinces = {20}}),
           State(3, {.provinces = {30}}),
           State(4, {.provinces = {40}}),
           State(5, {.provinces = {50}}),
           State(6, {.provinces = {60}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 2),
           testing::Pair(30, 3),
           testing::Pair(40, 4),
           testing::Pair(50, 5),
           testing::Pair(60, 6)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 4)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"),
           testing::Pair("STATE_2", "REGION_ONE"),
           testing::Pair("STATE_3", "REGION_ONE"),
           testing::Pair("STATE_4", "REGION_TWO"),
           testing::Pair("STATE_5", "REGION_TWO"),
           testing::Pair("STATE_6", "REGION_TWO")));
}


TEST(Hoi4worldStatesHoi4statesconverter, StatesAllInStrategicRegionAreNotSplit)
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
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const StrategicRegions strategic_regions({
       .province_to_strategic_region_map =
           {
               {10, 1},
               {20, 1},
               {30, 1},
               {40, 2},
               {50, 2},
               {60, 2},
           },
   });
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
           {
               {"REGION_ONE", vic3::StateRegion({}, {"0x000001", "0x000002", "0x000003"})},
               {"REGION_TWO", vic3::StateRegion({}, {"0x000004", "0x000005", "0x000006"})},
           },
           CoastalProvinces(),
           {},
           false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}}), State(2, {.provinces = {40, 50, 60}})));
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


TEST(Hoi4worldStatesHoi4statesconverter, WastelandProvincesAreSplit)
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
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}})}},
       province_definitions,
       {},
       {},
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
       {
           {"REGION_ONE", vic3::StateRegion({}, {"0x000001", "0x000002", "0x000003"})},
           {"REGION_TWO", vic3::StateRegion({}, {"0x000004", "0x000005", "0x000006"})},
       },
       CoastalProvinces(),
       {},
       false);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20}}),
           State(2, {.provinces = {30}, .category = "wasteland"}),
           State(3, {.provinces = {60}}),
           State(4, {.provinces = {40, 50}, .category = "wasteland"})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 2),
           testing::Pair(40, 4),
           testing::Pair(50, 4),
           testing::Pair(60, 3)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 3)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"),
           testing::Pair("STATE_2", "REGION_ONE"),
           testing::Pair("STATE_3", "REGION_TWO"),
           testing::Pair("STATE_4", "REGION_TWO")));
}


TEST(Hoi4worldStatesHoi4statesconverter, StatesWithNoProvincesAreNotConverted)
{
   const vic3::ProvinceDefinitions province_definitions;
   const mappers::ProvinceMapper province_mapper({}, {});
   const maps::ProvinceDefinitions hoi4_province_definitions;
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates({{1, vic3::State()}},
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

   EXPECT_TRUE(hoi4_states.states.empty());
   EXPECT_TRUE(hoi4_states.province_to_state_id_map.empty());
   EXPECT_TRUE(hoi4_states.vic3_state_ids_to_hoi4_state_ids.empty());
   EXPECT_TRUE(hoi4_states.hoi4_state_names_to_vic3_state_names.empty());
}


TEST(Hoi4worldStatesHoi4statesconverter, MissingProvinceDefinitionIsLogged)
{
   const vic3::ProvinceDefinitions province_definitions({"0x000001", "0x000002", "0x000003", "0x000004", "0x000005"});
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

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

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
           {
               {"REGION_ONE", vic3::StateRegion({}, {"0x000001", "0x000002", "0x000003"})},
               {"REGION_TWO", vic3::StateRegion({}, {"0x000004", "0x000005", "0x000006"})},
           },
           CoastalProvinces(),
           {},
           false);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No definition for province 6."));
   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}}), State(2, {.provinces = {40, 50}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"), testing::Pair("STATE_2", "REGION_TWO")));
}

}  // namespace hoi4