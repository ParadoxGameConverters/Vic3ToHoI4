#include <gmock/gmock-matchers.h>

#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/railways_converter.h"



namespace hoi4
{

TEST(Hoi4worldMapRailwaysConverterTests, IntrastateRailwaysAreCreated)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 2}), Railway(3, {1, 3})));
}


TEST(Hoi4worldMapRailwaysConverterTests, NoCityMeansNoIntrastateConnections)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "forest"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoIntrastateRailwaysToSameVic3Province)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000001", "dock"},
                                 {"0x000001", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoIntrastateRailwaysToSameHoi4Province)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {1}},
                                                     {"0x000003", {1}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoRailwayForUnmappedProvinces)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{}, {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(railways.railways, testing::ElementsAre());
   EXPECT_THAT(log.str(), testing::HasSubstr("No hoi4 provinces for significant vic3 province 0x000001 or 0x000002."));
   EXPECT_THAT(log.str(), testing::HasSubstr("No hoi4 provinces for significant vic3 province 0x000001 or 0x000003."));
}


TEST(Hoi4worldMapRailwaysConverterTests, NoRailwayForProvincesMappedToNothing)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {}},
                                                     {"0x000002", {}},
                                                     {"0x000003", {}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoIntrastateRailwaysForDisconnectedProvinces)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data;

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, InterstateRailwaysAreCreated)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                             },
               {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                             },
               {}}},
       {"STATE_THREE",
           vic3::StateRegion{{
                                 {"0x000100", "city"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                     {"0x000100", {100}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"10", "100", "bad_neighbor_to_verify_catch"}},
               {"10", {"1"}},
               {"100", {"1"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10", "100"},
   }};

   const hoi4::States hoi4_states{
       .states = {hoi4::State(1, {}), hoi4::State(2, {})},
       .province_to_state_id_map =
           {
               {1, 1},
               {10, 2},
               {100, 3},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 10}), Railway(3, {1, 100})));
}


TEST(Hoi4worldMapRailwaysConverterTests, NoInterstateRailroadsForNonBorderingStates)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                             },
               {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                             },
               {}}},
       {"STATE_THREE",
           vic3::StateRegion{{
                                 {"0x000100", "city"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                     {"0x000100", {100}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data;

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10", "100"},
   }};

   const hoi4::States hoi4_states{
       .states = {hoi4::State(1, {}), hoi4::State(2, {})},
       .province_to_state_id_map =
           {
               {1, 1},
               {10, 2},
               {100, 3},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoInterstateRailroadsForStatesWithoutSignificantProvinces)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE", vic3::StateRegion{{}, {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                             },
               {}}},
       {"STATE_THREE",
           vic3::StateRegion{{
                                 {"0x000100", "city"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                     {"0x000100", {100}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"10", "100"}},
               {"10", {"1"}},
               {"100", {"1"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10", "100"},
   }};

   const hoi4::States hoi4_states{
       .states = {hoi4::State(1, {}), hoi4::State(2, {})},
       .province_to_state_id_map =
           {
               {1, 1},
               {10, 2},
               {100, 3},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, LowestCostConnectionFormsInterstateRailroad)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "port"},
                                 {"0x000003", "mine"},
                             },
               {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                                 {"0x000020", "port"},
                                 {"0x000030", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                     {"0x000010", {10}},
                                                     {"0x000020", {20}},
                                                     {"0x000030", {30}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"100"}},
               {"100", {"1000", "1"}},
               {"1000", {"100", "10"}},
               {"10", {"1000"}},
               {"2", {"20"}},
               {"20", {"2"}},
               {"3", {"300"}},
               {"300", {"30", "3"}},
               {"30", {"300"}},

           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10", "100", "1000", "2", "20", "3", "30", "300"},
   }};

   const hoi4::States hoi4_states{
       .states = {hoi4::State(1, {}), hoi4::State(2, {})},
       .province_to_state_id_map =
           {
               {1, 1},
               {100, 1},
               {2, 1},
               {3, 1},
               {300, 1},
               {10, 2},
               {1000, 2},
               {20, 2},
               {30, 2},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {2, 20})));
}


// add tests for pathing - shorter paths better, lower cost provinces better


}  // namespace hoi4