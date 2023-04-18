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


TEST(Hoi4worldMapRailwaysConverterTests, NoIntrastateRailwaysForNonNeighbors)
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
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"10"}},
               {"10", {"1"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10"},
   }};

   const hoi4::States hoi4_states({hoi4::State(1, {}), hoi4::State(2, {})},
       {
           {1, 1},
           {10, 2},
       },
       {},
       {});

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 10})));
}


// add tests for interstate railways
// add tests for pathing - shorter paths better, lower cost provinces better


}  // namespace hoi4