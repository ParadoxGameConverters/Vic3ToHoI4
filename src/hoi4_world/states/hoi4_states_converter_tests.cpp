#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/hoi4_world/world/hoi4_world_framework_builder.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/provinces/province_mapping_types.h"
#include "src/mappers/world/world_mapper.h"
#include "src/mappers/world/world_mapper_builder.h"
#include "src/maps/map_data.h"
#include "src/vic3_world/states/vic3_state.h"
#include "src/vic3_world/world/vic3_world.h"


namespace hoi4
{

TEST(Hoi4worldStatesHoi4statesconverter, NoStatesConvertToNoStates)
{
   const maps::ProvinceDefinitions hoi4_province_definitions;
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});

   vic3::World world({});
   mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper().Build();
   hoi4::WorldFramework world_framework = WorldFrameworkBuilder::CreateNullWorldFramework().Build();

   const auto hoi4_states = ConvertStates(world, world_mapper, world_framework, {}, map_data);

   EXPECT_TRUE(hoi4_states.states.empty());
   EXPECT_TRUE(hoi4_states.province_to_state_id_map.empty());
   EXPECT_TRUE(hoi4_states.vic3_state_ids_to_hoi4_state_ids.empty());
   EXPECT_TRUE(hoi4_states.hoi4_state_names_to_vic3_state_names.empty());
}

TEST(Hoi4worldStatesHoi4statesconverter, StatesAreConverted)
{
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}

TEST(Hoi4worldStatesHoi4statesconverter, ProvincesMapToStates)
{
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(5));
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5}});
   world_mapper.CopyToVicWorld(world);

   vic3::World source_world = world.Build();
   const auto provinceMap = MapVic3ProvincesToStates(source_world.GetStates(), source_world.GetProvinceDefinitions());

   EXPECT_THAT(provinceMap,
       testing::ElementsAre(testing::Pair("x000001", 1),
           testing::Pair("x000002", 1),
           testing::Pair("x000003", 1),
           testing::Pair("x000004", 2),
           testing::Pair("x000005", 2)));
}

TEST(Hoi4worldStatesHoi4statesconverter, DefaultProvinceMapIsEmpty)
{
   const auto provinceMap = MapVic3ProvincesToStates({}, {});

   EXPECT_TRUE(provinceMap.empty());
}

TEST(Hoi4worldStatesHoi4statesconverter, SplitProvincesGoToCityandPortsOwnersStates)
{
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"x000001", "x000002", "x000003"}},
           {20, {"x000004", "x000005", "x000006"}},
       });

   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper({{1, "ONE"}, {2, "TWO"}, {3, "THR"}, {4, "FOR"}});

   vic3::World world = vic3::WorldBuilder::CreateNullWorld()
                           .AddStates({{1, vic3::State({.owner_number = 1, .provinces = {1}})},
                               {2, vic3::State({.owner_number = 2, .provinces = {2, 3}})},
                               {3, vic3::State({.owner_number = 3, .provinces = {4}})},
                               {4, vic3::State({.owner_number = 4, .provinces = {5, 6}})}})
                           .AddProvinces({
                               "x000001",
                               "x000002",
                               "x000003",
                               "x000004",
                               "x000005",
                               "x000006",
                           })
                           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                           .Build();
   mappers::WorldMapper world_mapper(std::move(country_mapper),
       std::move(province_mapper),
       {},
       mappers::CultureGraphicsMapper({}));
   hoi4::WorldFramework world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(2).Build();
   const maps::MapData map_data({.province_definitions = world_framework.province_definitions});
   const auto hoi4_states =
       ConvertStates(world, world_mapper, world_framework, {{"x000001", "city"}, {"x000004", "port"}}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.owner = "ONE", .provinces = {10}}),
           State(2, {.owner = "THR", .provinces = {20}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1), testing::Pair(20, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(3, 2)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, SplitProvincesGoToMajorityState)
{
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"x000001", "x000002", "x000003"}},
           {20, {"x000004", "x000005", "x000006"}},
       });

   const mappers::CountryMapper country_mapper({{1, "ONE"}, {4, "FOR"}});

   vic3::World world = vic3::WorldBuilder::CreateNullWorld()
                           .AddStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2}})},
                               {2, vic3::State({.owner_number = 2, .provinces = {3}})},
                               {3, vic3::State({.owner_number = 3, .provinces = {4}})},
                               {4, vic3::State({.owner_number = 4, .provinces = {5, 6}})}})
                           .AddProvinces({
                               "x000001",
                               "x000002",
                               "x000003",
                               "x000004",
                               "x000005",
                               "x000006",
                           })
                           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                           .Build();
   mappers::WorldMapper world_mapper(std::move(country_mapper),
       std::move(province_mapper),
       {},
       mappers::CultureGraphicsMapper({}));
   hoi4::WorldFramework world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(2).Build();
   const maps::MapData map_data({.province_definitions = world_framework.province_definitions});
   const auto hoi4_states = ConvertStates(world, world_mapper, world_framework, {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.owner = "ONE", .provinces = {10}}),
           State(2, {.owner = "FOR", .provinces = {20}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1), testing::Pair(20, 2)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(4, 2)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, BadNeighborStringsAreSkipped)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "abc30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"),
           testing::Pair("STATE_2", "REGION_001"),
           testing::Pair("STATE_3", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, DisconnectedStatesAreSplit)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"),
           testing::Pair("STATE_2", "REGION_001"),
           testing::Pair("STATE_3", "REGION_001"),
           testing::Pair("STATE_4", "REGION_002"),
           testing::Pair("STATE_5", "REGION_002"),
           testing::Pair("STATE_6", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, StatesAllInStrategicRegionAreNotSplit)
{
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

   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().SetStrategicRegions(strategic_regions).AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, WastelandProvincesAreSplit)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}})
                                  .AddStates({{2, vic3::State({.owner_number = 42, .provinces = {4, 5, 6}})}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6).AddDefaultStates({
           {1, DefaultState({.provinces = {10, 20}})},
           {2, DefaultState({.impassable = true, .provinces = {30, 40, 50}})},
           {3, DefaultState({.provinces = {60}})},
       });
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"),
           testing::Pair("STATE_2", "REGION_001"),
           testing::Pair("STATE_3", "REGION_002"),
           testing::Pair("STATE_4", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, StatesWithNoProvincesAreNotConverted)
{
   const maps::ProvinceDefinitions hoi4_province_definitions;
   const maps::MapData map_data({.province_definitions = hoi4_province_definitions});

   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld().AddStates({{1, vic3::State()}});
   mappers::WorldMapperBuilder world_mapper = mappers::WorldMapperBuilder::CreateNullMapper();
   hoi4::WorldFrameworkBuilder world_framework = WorldFrameworkBuilder::CreateNullWorldFramework();

   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_TRUE(hoi4_states.states.empty());
   EXPECT_TRUE(hoi4_states.province_to_state_id_map.empty());
   EXPECT_TRUE(hoi4_states.vic3_state_ids_to_hoi4_state_ids.empty());
   EXPECT_TRUE(hoi4_states.hoi4_state_names_to_vic3_state_names.empty());
}


TEST(Hoi4worldStatesHoi4statesconverter, MissingProvinceDefinitionIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                                  .AddProvinces({"x000001", "x000002", "x000003", "x000004", "x000005"});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}



TEST(Hoi4worldStatesHoi4statesconverter, UnmappedProvincesAreLogged)
{
   const mappers::ProvinceMapper province_mapper({},
       {
           {10, {"x000001"}},
           {20, {"x000002"}},
           {30, {"x000003"}},
           {40, {"x000004"}},
           {50, {"x000005"}},
           {60, {}},
       });
   const StrategicRegions strategic_regions;
   const mappers::CountryMapper country_mapper;

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());


   vic3::World world = vic3::WorldBuilder::CreateNullWorld()
                           .AddTestStates({{1, 2, 3}, {4, 5, 6}})
                           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}})
                           .AddProvinces({
                               "x000001",
                               "x000002",
                               "x000003",
                               "x000004",
                               "x000005",
                               "x000006",
                           })
                           .Build();
   mappers::WorldMapper world_mapper(mappers::CountryMapper(),
       std::move(province_mapper),
       {},
       mappers::CultureGraphicsMapper({}));
   hoi4::WorldFramework world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6).Build();
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.province_definitions,
   });
   const auto hoi4_states = ConvertStates(world, world_mapper, world_framework, {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, ProvinceWithNoStatesAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   vic3::WorldBuilder world =
       vic3::WorldBuilder::CreateNullWorld()
           .AddStates({{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 6}})}})
           .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("Vic3 province x000005 was not in a state."));
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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, IdsAreSequentialFromOne)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddStates({
                                      {0, vic3::State({.provinces = {1}})},
                                      {5, vic3::State({.provinces = {5}})},
                                      {9, vic3::State({.provinces = {9}})},
                                  })
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}})
                                  .AddProvinces({
                                      "x000001",
                                      "x000002",
                                      "x000003",
                                      "x000004",
                                      "x000005",
                                      "x000006",
                                      "x000007",
                                      "x000008",
                                      "x000009",
                                  });
   mappers::WorldMapperBuilder world_mapper = std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddProvinces({
       {{"x000001"}, 10},
       {{"x000005"}, 50},
       {{"x000009"}, 90},
   }));
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddLandProvinces({"10", "50", "90"});
   const maps::MapData map_data({
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

   EXPECT_THAT(hoi4_states.states,
       testing::ElementsAre(State(1, {.provinces = {10}}),
           State(2, {.provinces = {50}}),
           State(3, {.provinces = {90}})));
   EXPECT_THAT(hoi4_states.province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1), testing::Pair(50, 2), testing::Pair(90, 3)));
   EXPECT_THAT(hoi4_states.vic3_state_ids_to_hoi4_state_ids,
       testing::UnorderedElementsAre(testing::Pair(0, 1), testing::Pair(5, 2), testing::Pair(9, 3)));
   EXPECT_THAT(hoi4_states.hoi4_state_names_to_vic3_state_names,
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"),
           testing::Pair("STATE_2", "REGION_002"),
           testing::Pair("STATE_3", "REGION_003")));
}


TEST(Hoi4worldStatesHoi4statesconverter, OwnersAreConverted)
{
   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddStates({{1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
                                      {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})}})
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper = std::move(
       mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6).AddCountries({{1, "TAG"}, {2, "TWO"}}));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
}


TEST(Hoi4worldStatesHoi4statesconverter, UnmappedOwnersAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   vic3::WorldBuilder world = vic3::WorldBuilder::CreateNullWorld()
                                  .AddStates({
                                      {1, vic3::State({.owner_number = 1, .provinces = {1, 2, 3}})},
                                      {2, vic3::State({.owner_number = 2, .provinces = {4, 5, 6}})},
                                  })
                                  .AddTestStateRegions({{1, 2, 3}, {4, 5, 6}});
   mappers::WorldMapperBuilder world_mapper =
       std::move(mappers::WorldMapperBuilder::CreateNullMapper().AddTestProvinces(6));
   world_mapper.CopyToVicWorld(world);
   hoi4::WorldFrameworkBuilder world_framework =
       WorldFrameworkBuilder::CreateNullWorldFramework().AddTestLandProvinces(6);
   const maps::MapData map_data({
       .province_neighbors =
           {
               {"10", {"20", "30"}},
               {"40", {"50", "60"}},
           },
       .province_definitions = world_framework.CopyProvinceDefinitions(),
   });
   const auto hoi4_states = ConvertStates(world.Build(), world_mapper.Build(), world_framework.Build(), {}, map_data);

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
       testing::UnorderedElementsAre(testing::Pair("STATE_1", "REGION_001"), testing::Pair("STATE_2", "REGION_002")));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not get tag for owner of Vic3 state 1."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not get tag for owner of Vic3 state 2."));
}

}  // namespace hoi4