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

TEST(Hoi4worldStatesHoi4statesconverter, NoStatesConvertToNoStates)
{
   const vic3::ProvinceDefinitions vic3_province_definitions;
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings;
   const maps::ProvinceDefinitions hoi4_province_definitions;
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates({},
       vic3_province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001", "0x000002", "0x000003"}},
       {20, {"0x000004", "0x000005", "0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20"}});
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {4, "FOR"}});

   const auto hoi4_states = ConvertStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2}})},
                                              {2, vic3::State({.owner_number = 2, .provinces = {3}})},
                                              {3, vic3::State({.owner_number = 3, .provinces = {4}})},
                                              {4, vic3::State({.owner_number = 4, .provinces = {5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "abc30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
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
           {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
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
           {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const hoi4::StrategicRegions strategic_regions({
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
           hoi4_to_vic3_province_mappings,
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
           {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings;
   const maps::ProvinceDefinitions hoi4_province_definitions;
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates({{1, vic3::State()}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

   EXPECT_TRUE(hoi4_states.states.empty());
   EXPECT_TRUE(hoi4_states.province_to_state_id_map.empty());
   EXPECT_TRUE(hoi4_states.vic3_state_ids_to_hoi4_state_ids.empty());
   EXPECT_TRUE(hoi4_states.hoi4_state_names_to_vic3_state_names.empty());
}


TEST(Hoi4worldStatesHoi4statesconverter, MissingProvinceDefinitionIsLogged)
{
   const vic3::ProvinceDefinitions province_definitions({"0x000001", "0x000002", "0x000003", "0x000004", "0x000005"});
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
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
           {});

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


TEST(Hoi4worldStatesHoi4statesconverter, UnmappedProvincesAreLogged)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
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
           {});

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No mapping for hoi4 province 60."));
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


TEST(Hoi4worldStatesHoi4statesconverter, ProvinceWithNoStatesAreLogged)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
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
           {});

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("Vic3 province 0x000005 was not in a state."));
   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}}), State(2, {.provinces = {40, 60}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(60, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"), testing::Pair("STATE_2", "REGION_TWO")));
}


TEST(Hoi4worldStatesHoi4statesconverter, IdsAreSequentialFromOne)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
       "0x000007",
       "0x000008",
       "0x000009",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {50, {"0x000005"}},
       {90, {"0x000009"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "50", "90"}});
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {0, vic3::State({.provinces = {1}})},
           {5, vic3::State({.provinces = {5}})},
           {9, vic3::State({.provinces = {9}})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {
           {"REGION_ONE", vic3::StateRegion({}, {"0x000001", "0x000002", "0x000003"})},
           {"REGION_TWO", vic3::StateRegion({}, {"0x000004", "0x000005", "0x000006"})},
           {"REGION_THREE", vic3::StateRegion({}, {"0x000007", "0x000008", "0x000009"})},
       },
       CoastalProvinces(),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10}}),
           State(2, {.provinces = {50}}),
           State(3, {.provinces = {90}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1), testing::Pair(50, 2), testing::Pair(90, 3)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(0, 1), testing::Pair(5, 2), testing::Pair(9, 3)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_ONE"),
           testing::Pair("STATE_2", "REGION_TWO"),
           testing::Pair("STATE_3", "REGION_THREE")));
}


TEST(Hoi4worldStatesHoi4statesconverter, OwnersAreConverted)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.owner = "TAG", .provinces = {10, 20, 30}}),
           State(2, {.owner = "TWO", .provinces = {40, 50, 60}})));
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


TEST(Hoi4worldStatesHoi4statesconverter, UnmappedOwnersAreLogged)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

   std::cout.rdbuf(cout_buffer);

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
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not get tag for owner of Vic3 state 1."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not get tag for owner of Vic3 state 2."));
}


TEST(Hoi4worldStatesHoi4statesconverter, ManpowerIsConverted)
{
   const vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}, .population = 12345})},
           {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}, .population = 67890})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .manpower = 12345}),
           State(2, {.provinces = {40, 50, 60}, .manpower = 67890})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ManpowerInSplitStatesIsProportionalToTotalProvinces)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20"}},
               {"40", {"50"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}, .population = 12345})},
           {2, vic3::State({.provinces = {4, 5, 6}, .population = 67890})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20}, .manpower = 8230}),
           State(2, {.provinces = {30}, .manpower = 4115}),
           State(3, {.provinces = {40, 50}, .manpower = 45'260}),
           State(4, {.provinces = {60}, .manpower = 22'630})));
}


TEST(Hoi4worldStatesHoi4statesconverter, ManpowerInWastelandStatesIsProportionalToTotalProvinces)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.provinces = {1, 2, 3}, .population = 12345})},
                         {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}, .population = 67890})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
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
           {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20}, .manpower = 8230}),
           State(2, {.provinces = {30}, .manpower = 4115, .category = "wasteland"}),
           State(3, {.provinces = {60}, .manpower = 22'630}),
           State(4, {.provinces = {40, 50}, .manpower = 45'260, .category = "wasteland"})));
}


TEST(Hoi4worldStatesHoi4statesconverter, IndustryIsConverted)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 500'000})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}, .employed_population = 500'000})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "ONE", .provinces = {10, 20, 30}, .civilian_factories = 3, .military_factories = 2}),
           State(2, {.owner = "TWO", .provinces = {40, 50, 60}, .civilian_factories = 3, .military_factories = 2})));
}


TEST(Hoi4worldStatesHoi4statesconverter, DockyardsAreConvertedInCoastalStates)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 500'000})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}, .employed_population = 500'000})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(std::map<int, std::vector<int>>{{40, {41}}}),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "ONE", .provinces = {10, 20, 30}, .civilian_factories = 3, .military_factories = 2}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .civilian_factories = 2,
                   .military_factories = 2,
                   .dockyards = 1})));
}


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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 500'000})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}, .employed_population = 500'000})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}, .employed_population = 500'000})},
           {2, vic3::State({.provinces = {4, 5, 6}, .employed_population = 500'000})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper(std::map<int, std::string>{{1, "ONE"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 1'500'000})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "ONE", .provinces = {10, 20, 30}, .civilian_factories = 7, .military_factories = 5})));
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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper(std::map<int, std::string>{{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 200'000})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5}, .employed_population = 1'500'000})},
           {3, vic3::State({.owner_number = 2, .provinces = {6}, .employed_population = 600'000})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "TWO", .provinces = {40, 50}, .civilian_factories = 7, .military_factories = 5}),
           State(2, {.owner = "TWO", .provinces = {60}, .civilian_factories = 7, .military_factories = 2}),
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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper(std::map<int, std::string>{{1, "ONE"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 1'500'000})},
           {2, vic3::State({.owner_number = 1, .provinces = {4, 5, 6}, .employed_population = 0})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "ONE", .provinces = {10, 20, 30}, .civilian_factories = 7, .military_factories = 5}),
           State(2, {.owner = "ONE", .provinces = {40, 50, 60}, .civilian_factories = 4, .military_factories = 0})));
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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20"}},
               {"40", {"50"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 300'000})},
                         {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}, .employed_population = 600'000})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
           map_data,
           hoi4_province_definitions,
           strategic_regions,
           country_mapper,
           StateCategories(),
           {},
           {},
           CoastalProvinces(),
           {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(
           State(1, {.owner = "TWO", .provinces = {40, 50}, .civilian_factories = 2, .military_factories = 2}),
           State(2, {.owner = "TWO", .provinces = {60}, .civilian_factories = 3, .military_factories = 0}),
           State(3, {.owner = "ONE", .provinces = {10, 20}, .civilian_factories = 1, .military_factories = 1}),
           State(4, {.owner = "ONE", .provinces = {30}, .civilian_factories = 2, .military_factories = 0})));
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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 500'000})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}, .employed_population = 500'000})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1,
                                {.owner = "ONE",
                                    .provinces = {10, 20, 30},
                                    .victory_points = {{20, 4}},
                                    .naval_base_location = 20,
                                    .naval_base_level = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .victory_points = {{50, 4}},
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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states =
       ConvertStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
           map_data,
           hoi4_province_definitions,
           strategic_regions,
           country_mapper,
           StateCategories(),
           {},
           {},
           CoastalProvinces(),
           {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       });

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}});

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}, .employed_population = 500'000})},
           {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}, .employed_population = 800'000})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories({
           {3, "test_category_one"},
           {5, "test_category_two"},
           {7, "test_category_three"},
           {9, "test_category_four"},
       }),
       {},
       {},
       CoastalProvinces(),
       {});

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
                   .civilian_factories = 3,
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
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper,
       StateCategories(),
       {},
       {},
       CoastalProvinces(),
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .victory_points = {}}),
           State(2, {.provinces = {40, 50, 60}, .victory_points = {}})));
}


TEST(Hoi4worldStatesHoi4statesconverter, VictoryPointsAreConverted)
{
   const vic3::ProvinceDefinitions province_definitions({
       "0x000001",
       "0x000002",
       "0x000003",
       "0x000004",
       "0x000005",
       "0x000006",
   });
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   const maps::ProvinceDefinitions hoi4_province_definitions({.land_provinces = {"10", "20", "30", "40", "50", "60"}});
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = hoi4_province_definitions,
   });
   const hoi4::StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(
       {
           {1, vic3::State({.provinces = {1, 2, 3}})},
           {2, vic3::State({.provinces = {4, 5, 6}})},
       },
       province_definitions,
       hoi4_to_vic3_province_mappings,
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
       {});

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10, 20, 30}, .victory_points = {{10, 1}, {20, 2}, {30, 3}}}),
           State(2, {.provinces = {40, 50, 60}, .victory_points = {{40, 4}, {50, 5}}})));
}

}  // namespace hoi4