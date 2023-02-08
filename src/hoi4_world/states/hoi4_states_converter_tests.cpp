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
   vic3::ProvinceDefinitions vic3_province_definitions({});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings;
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   const auto hoi4_states = StatesConverter{}.ConvertStates({},
       vic3_province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   EXPECT_TRUE(hoi4_states.states.empty());
   EXPECT_TRUE(hoi4_states.province_to_state_id_map.empty());
}


TEST(Hoi4worldStatesHoi4statesconverter, StatesAreConverted)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   const auto hoi4_states = StatesConverter{}.ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10, 20, 30}), State(2, std::nullopt, {40, 50, 60})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2),
           testing::Pair(60, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
}


TEST(Hoi4worldStatesHoi4statesconverter, SplitProvincesGoToMajorityState)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001", "0x000002", "0x000003"}},
       {20, {"0x000004", "0x000005", "0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20"}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({{"ONE", "ONE"}, {"FOR", "FOR"}});

   const auto hoi4_states =
       StatesConverter{}.ConvertStates({{1, vic3::State({.owner_tag = "ONE", .provinces = {1, 2}})},
                                           {2, vic3::State({.owner_tag = "TWO", .provinces = {3}})},
                                           {3, vic3::State({.owner_tag = "THR", .provinces = {4}})},
                                           {4, vic3::State({.owner_tag = "FOR", .provinces = {5, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
           map_data,
           hoi4_province_definitions,
           strategic_regions,
           country_mapper);

   EXPECT_THAT(hoi4_states.states, testing::ElementsAre(State(1, "ONE", {10}), State(2, "FOR", {20})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1), testing::Pair(20, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(4, 2)));
}


TEST(Hoi4worldStatesHoi4statesconverter, BadNeighborStringsAreSkipped)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "abc30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   const auto hoi4_states = StatesConverter{}.ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10, 20}),
           State(2, std::nullopt, {30}),
           State(3, std::nullopt, {40, 50, 60})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 2),
           testing::Pair(40, 3),
           testing::Pair(50, 3),
           testing::Pair(60, 3)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 3)));
}


TEST(Hoi4worldStatesHoi4statesconverter, DisconnectedStatesAreSplit)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tState 1 was split into 3 due to disconnected provinces."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tState 4 was split into 3 due to disconnected provinces."));
   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10}),
           State(2, std::nullopt, {20}),
           State(3, std::nullopt, {30}),
           State(4, std::nullopt, {40}),
           State(5, std::nullopt, {50}),
           State(6, std::nullopt, {60})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 2),
           testing::Pair(30, 3),
           testing::Pair(40, 4),
           testing::Pair(50, 5),
           testing::Pair(60, 6)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 4)));
}


TEST(Hoi4worldStatesHoi4statesconverter, StatesAllInStrategicRegionAreNotSplit)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {{10, 1}, {20, 1}, {30, 1}, {40, 2}, {50, 2}, {60, 2}});
   const mappers::CountryMapper country_mapper({});

   const auto hoi4_states = StatesConverter{}.ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10, 20, 30}), State(2, std::nullopt, {40, 50, 60})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2),
           testing::Pair(60, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
}

TEST(Hoi4worldStatesHoi4statesconverter, StatesWithNoProvincesAreNotConverted)
{
   vic3::ProvinceDefinitions province_definitions({});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings;
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   EXPECT_TRUE(hoi4_states.states.empty());
   EXPECT_TRUE(hoi4_states.province_to_state_id_map.empty());
   EXPECT_TRUE(hoi4_states.vic3_state_ids_to_hoi4_state_ids.empty());
}


TEST(Hoi4worldStatesHoi4statesconverter, MissingProvinceDefinitionIsLogged)
{
   vic3::ProvinceDefinitions province_definitions({"0x000001", "0x000002", "0x000003", "0x000004", "0x000005"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No definition for province 6."));
   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10, 20, 30}), State(2, std::nullopt, {40, 50})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
}


TEST(Hoi4worldStatesHoi4statesconverter, UnmappedProvincesAreLogged)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No mapping for hoi4 province 60."));
   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10, 20, 30}), State(2, std::nullopt, {40, 50})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
}


TEST(Hoi4worldStatesHoi4statesconverter, ProvinceWithNoStatesAreLogged)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates(
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 6}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("Vic3 province 0x000005 was not in a state."));
   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10, 20, 30}), State(2, std::nullopt, {40, 60})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(60, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
}


TEST(Hoi4worldStatesHoi4statesconverter, IdsAreSequentialFromOne)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006", "0x000007", "0x000008", "0x000009"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {50, {"0x000005"}},
       {90, {"0x000009"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "50", "90"}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   const auto hoi4_states = StatesConverter{}.ConvertStates({{0, vic3::State({.provinces = {1}})},
                                                                {5, vic3::State({.provinces = {5}})},
                                                                {9, vic3::State({.provinces = {9}})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       hoi4_province_definitions,
       strategic_regions,
       country_mapper);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10}), State(2, std::nullopt, {50}), State(3, std::nullopt, {90})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1), testing::Pair(50, 2), testing::Pair(90, 3)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(0, 1), testing::Pair(5, 2), testing::Pair(9, 3)));
}


TEST(Hoi4worldStatesHoi4statesconverter, OwnersAreConverted)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});

   const auto hoi4_states =
       StatesConverter{}.ConvertStates({{1, vic3::State({.owner_tag = "TAG", .provinces = {1, 2, 3}})},
                                           {2, vic3::State({.owner_tag = "TWO", .provinces = {4, 5, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
           map_data,
           hoi4_province_definitions,
           strategic_regions,
           country_mapper);

   EXPECT_THAT(hoi4_states.states, testing::ElementsAre(State(1, "TAG", {10, 20, 30}), State(2, "TWO", {40, 50, 60})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2),
           testing::Pair(60, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
}


TEST(Hoi4worldStatesHoi4statesconverter, UnmappedOwnersAreLogged)
{
   vic3::ProvinceDefinitions province_definitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings{
       {10, {"0x000001"}},
       {20, {"0x000002"}},
       {30, {"0x000003"}},
       {40, {"0x000004"}},
       {50, {"0x000005"}},
       {60, {"0x000006"}},
   };
   maps::ProvinceDefinitions hoi4_province_definitions{{"10", "20", "30", "40", "50", "60"}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});
   const mappers::CountryMapper country_mapper({});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states =
       StatesConverter{}.ConvertStates({{1, vic3::State({.owner_tag = "TAG", .provinces = {1, 2, 3}})},
                                           {2, vic3::State({.owner_tag = "TWO", .provinces = {4, 5, 6}})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
           map_data,
           hoi4_province_definitions,
           strategic_regions,
           country_mapper);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, std::nullopt, {10, 20, 30}), State(2, std::nullopt, {40, 50, 60})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2),
           testing::Pair(60, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 2)));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not get tag for owner of state 1."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not get tag for owner of state 2."));
}

}  // namespace hoi4