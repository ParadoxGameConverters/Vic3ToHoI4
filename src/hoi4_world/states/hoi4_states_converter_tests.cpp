#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/provinces/province_mapping_types.h"
#include "src/vic3_world/states/vic3_state.h"



namespace hoi4
{

TEST(Hoi4worldStatesHoi4statesconverter, NoStatesConvertToNoStates)
{
   vic3::ProvinceDefinitions province_definitions({});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings;

   const auto hoi4_states = StatesConverter{}.ConvertStates({}, province_definitions, hoi4_to_vic3_province_mappings);

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

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 5, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings);

   EXPECT_THAT(hoi4_states, testing::ElementsAre(State(1, {10, 20, 30}), State(2, {40, 50, 60})));
}


TEST(Hoi4worldStatesHoi4statesconverter, StatesWithNoProvincesAreNotConverted)
{
   vic3::ProvinceDefinitions province_definitions({});
   mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_mappings;

   const auto hoi4_states =
       StatesConverter{}.ConvertStates({{1, vic3::State({})}}, province_definitions, hoi4_to_vic3_province_mappings);

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

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 5, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings);

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

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 5, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings);

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

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto hoi4_states = StatesConverter{}.ConvertStates({{1, vic3::State({1, 2, 3})}, {2, vic3::State({4, 6})}},
       province_definitions,
       hoi4_to_vic3_province_mappings);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("Vic3 province 0x000005 was not in a state."));
   EXPECT_THAT(hoi4_states, testing::ElementsAre(State(1, {10, 20, 30}), State(2, {40, 60})));
}

}  // namespace hoi4