#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"
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

   const auto hoi4_states = StatesConverter{}.ConvertStates({},
       vic3_province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       {},
       strategic_regions);

   EXPECT_TRUE(hoi4_states.empty());
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
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 5, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       {{10, hoi4::Province{true, ""}},
           {20, hoi4::Province{true, ""}},
           {30, hoi4::Province{true, ""}},
           {40, hoi4::Province{true, ""}},
           {50, hoi4::Province{true, ""}},
           {60, hoi4::Province{true, ""}}},
       strategic_regions);

   EXPECT_THAT(hoi4_states, testing::ElementsAre(State(1, {10, 20, 30}), State(2, {40, 50, 60})));
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
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 5, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       {{10, hoi4::Province{true, ""}},
           {20, hoi4::Province{true, ""}},
           {30, hoi4::Province{true, ""}},
           {40, hoi4::Province{true, ""}},
           {50, hoi4::Province{true, ""}},
           {60, hoi4::Province{true, ""}}},
       strategic_regions);

   EXPECT_THAT(hoi4_states,
       testing::ElementsAre(State(1, {10}),
           State(2, {20}),
           State(3, {30}),
           State(4, {40}),
           State(5, {50}),
           State(6, {60})));
}

TEST(Hoi4worldStatesHoi4statesconverter, StatesWithNoProvincesAreNotConverted)
{
   vic3::ProvinceDefinitions province_definitions({});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings;
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       {},
       strategic_regions);

   EXPECT_TRUE(hoi4_states.empty());
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
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 5, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       {{10, hoi4::Province{true, ""}},
           {20, hoi4::Province{true, ""}},
           {30, hoi4::Province{true, ""}},
           {40, hoi4::Province{true, ""}},
           {50, hoi4::Province{true, ""}},
           {60, hoi4::Province{true, ""}}},
       strategic_regions);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No definition for province 6."));
   EXPECT_THAT(hoi4_states, testing::ElementsAre(State(1, {10, 20, 30}), State(2, {40, 50})));
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
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 5, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       {{10, hoi4::Province{true, ""}},
           {20, hoi4::Province{true, ""}},
           {30, hoi4::Province{true, ""}},
           {40, hoi4::Province{true, ""}},
           {50, hoi4::Province{true, ""}},
           {60, hoi4::Province{true, ""}}},
       strategic_regions);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] No mapping for hoi4 province 60."));
   EXPECT_THAT(hoi4_states, testing::ElementsAre(State(1, {10, 20, 30}), State(2, {40, 50})));
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
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{{"10", {"20", "30"}}, {"40", {"50", "60"}}}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings,
       map_data,
       {{10, hoi4::Province{true, ""}},
           {20, hoi4::Province{true, ""}},
           {30, hoi4::Province{true, ""}},
           {40, hoi4::Province{true, ""}},
           {50, hoi4::Province{true, ""}},
           {60, hoi4::Province{true, ""}}},
       strategic_regions);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("Vic3 province 0x000005 was not in a state."));
   EXPECT_THAT(hoi4_states, testing::ElementsAre(State(1, {10, 20, 30}), State(2, {40, 60})));
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
   maps::ProvinceDefinitions hoi4_province_definitions{{}, {}, {}, {}};
   maps::MapData map_data{{}, {}, {}, hoi4_province_definitions, {}};
   hoi4::StrategicRegions strategic_regions({}, {});

   const auto hoi4_states =
       StatesConverter{}.ConvertStates({{0, vic3::State({1})}, {5, vic3::State({5})}, {9, vic3::State({9})}},
           province_definitions,
           hoi4_to_vic3_province_mappings,
           map_data,
           {},
           strategic_regions);

   EXPECT_THAT(hoi4_states, testing::ElementsAre(State(1, {10}), State(2, {50}), State(3, {90})));
}

}  // namespace hoi4