#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/buildings_creator.h"



namespace hoi4
{

TEST(Hoi4worldMapBuildingsCreatorTests, ExceptionForMissingBuildingsDotTxt)
{
   EXPECT_THROW(ImportBuildings(States(),
                    CoastalProvinces({}),
                    maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
                    commonItems::ModFilesystem{"ExceptionForMissingBuildingsDotTxt", {}}),
       std::runtime_error);
}


TEST(Hoi4worldMapBuildingsCreatorTests, DefaultsToNoBuildings)
{
   Buildings buildings = ImportBuildings(States(),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_TRUE(buildings.GetAirportLocations().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportPlacedInCenterOfFirstProvinceOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
           },
           {{}, {}, {}, {}},
           {
               {maps::Point{1, 1}, "1"},
               {maps::Point{1, 2}, "1"},
               {maps::Point{1, 3}, "1"},
               {maps::Point{2, 1}, "1"},
               {maps::Point{2, 2}, "1"},
               {maps::Point{2, 3}, "1"},
               {maps::Point{3, 1}, "1"},
               {maps::Point{3, 2}, "1"},
               {maps::Point{3, 3}, "1"},
               {maps::Point{4, 1}, "2"},
               {maps::Point{4, 2}, "2"},
               {maps::Point{4, 3}, "2"},
               {maps::Point{5, 1}, "2"},
               {maps::Point{5, 2}, "2"},
               {maps::Point{5, 3}, "2"},
               {maps::Point{6, 1}, "2"},
               {maps::Point{6, 2}, "2"},
               {maps::Point{6, 3}, "2"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "air_base",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0})}));
   EXPECT_THAT(buildings.GetAirportLocations(), testing::UnorderedElementsAre(testing::Pair(1, 1)));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoAirportInStateWithNoProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
           },
           {{}, {}, {}, {}},
           {
               {maps::Point{2, 2}, "1"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_TRUE(buildings.GetAirportLocations().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportNotPlacedInMisnamedProvince)
{
    Buildings buildings = ImportBuildings(States({ State(1, std::nullopt, {1, 2, 3, 4, 5}) },
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
                                             CoastalProvinces({}),
                                             maps::MapData({},
                                                 {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
           },
                                                 { {}, {}, {}, {} },
           {
               {maps::Point{2, 2}, "bad_name"},
           }),
           commonItems::ModFilesystem{ "test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {} });

    EXPECT_THAT(buildings.GetBuildings(),
        testing::IsSupersetOf({ Building(1,
            "air_base",
            {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}) }));
    EXPECT_TRUE(buildings.GetAirportLocations().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportNotPlacedInProvinceWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_TRUE(buildings.GetAirportLocations().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. air_base not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportPlacementOverridenByDefaultLocation)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
           },
           {{}, {}, {}, {}},
           {
               {maps::Point{1, 1}, "1"},
               {maps::Point{1, 2}, "1"},
               {maps::Point{1, 3}, "1"},
               {maps::Point{2, 1}, "1"},
               {maps::Point{2, 2}, "1"},
               {maps::Point{2, 3}, "1"},
               {maps::Point{3, 1}, "1"},
               {maps::Point{3, 2}, "1"},
               {maps::Point{3, 3}, "1"},
               {maps::Point{4, 1}, "2"},
               {maps::Point{4, 2}, "2"},
               {maps::Point{4, 3}, "2"},
               {maps::Point{5, 1}, "2"},
               {maps::Point{5, 2}, "2"},
               {maps::Point{5, 3}, "2"},
               {maps::Point{6, 1}, "2"},
               {maps::Point{6, 2}, "2"},
               {maps::Point{6, 3}, "2"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "air_base",
           {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0})}));
   EXPECT_THAT(buildings.GetAirportLocations(), testing::UnorderedElementsAre(testing::Pair(1, 2)));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
           },
           {{}, {}, {}, {}},
           {
               {maps::Point{1, 1}, "1"},
               {maps::Point{1, 2}, "1"},
               {maps::Point{1, 3}, "1"},
               {maps::Point{2, 1}, "1"},
               {maps::Point{2, 2}, "1"},
               {maps::Point{2, 3}, "1"},
               {maps::Point{3, 1}, "1"},
               {maps::Point{3, 2}, "1"},
               {maps::Point{3, 3}, "1"},
               {maps::Point{4, 1}, "2"},
               {maps::Point{4, 2}, "2"},
               {maps::Point{4, 3}, "2"},
               {maps::Point{5, 1}, "2"},
               {maps::Point{5, 2}, "2"},
               {maps::Point{5, 3}, "2"},
               {maps::Point{6, 1}, "2"},
               {maps::Point{6, 2}, "2"},
               {maps::Point{6, 3}, "2"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "air_base",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0})}));
   EXPECT_THAT(buildings.GetAirportLocations(), testing::UnorderedElementsAre(testing::Pair(1, 1)));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirPlacedInCenterOfFirstThreeProvincesOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "anti_air_building",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "anti_air_building",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "anti_air_building",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoAntiAirInStateWithNoProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. anti_air_building not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 2 did not have any points. anti_air_building not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 3 did not have any points. anti_air_building not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 4 did not have any points. anti_air_building not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {
               {{4, 1}, "2"},
               {{1, 4}, "3"},
               {{4, 4}, "4"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "anti_air_building",
                                  {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}),
           Building(1,
               "anti_air_building",
               {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 180.0}),
           Building(1,
               "anti_air_building",
               {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 270.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirPlacedInCenterOfFirstThreeProvincesOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "anti_air_building",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "anti_air_building",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "anti_air_building",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, ArmsFactoriesPlacedInCenterOfFirstSixProvincesOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5, 6, 7})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
               {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
               {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
               {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "arms_factory",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoArmsFactoriesInStateWithNoProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
               {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
               {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
               {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, ArmsFactoriesNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5, 6, 7})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. arms_factory not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 2 did not have any points. arms_factory not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 3 did not have any points. arms_factory not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 4 did not have any points. arms_factory not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, ArmsFactoriesPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5, 6, 7})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
               {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
               {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
               {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
           },
           {{}, {}, {}, {}},
           {
               {{4, 1}, "2"},
               {{1, 4}, "3"},
               {{4, 4}, "4"},
               {{7, 1}, "5"},
               {{7, 4}, "6"},
               {{1, 7}, "7"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "arms_factory",
                                  {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 45.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 90.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 135.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 7.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 180.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 7.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 225.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 7.0, .rotation = 270.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, ArmsFactoriesPlacedInCenterOfFirstSixProvincesOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5, 6, 7})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
               {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
               {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
               {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "arms_factory",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "arms_factory",
               {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, BunkerPlacedInCenterOfABorderForAllProvincesOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "bunker",
                                  {.x_coordinate = 3.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1, "bunker", {.x_coordinate = 4.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1, "bunker", {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}),
           Building(1, "bunker", {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoBunkersInStateWithNoProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   for (const auto building: buildings.GetBuildings())
   {
      EXPECT_NE(building.GetType(), "bunker");
   }
}


TEST(Hoi4worldMapBuildingsCreatorTests, BunkersNotPlacedInProvincesWithNoBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any border points. Bunkers not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 2 did not have any border points. Bunkers not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 3 did not have any border points. Bunkers not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 4 did not have any border points. Bunkers not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, BunkerPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {
               {{3, 2}, "1"},
               {{4, 2}, "2"},
               {{2, 4}, "3"},
               {{5, 4}, "4"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "bunker",
                                  {.x_coordinate = 3.0, .y_coordinate = 6.0, .z_coordinate = 2.0, .rotation = 90.0}),
           Building(1, "bunker", {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 2.0, .rotation = 180.0}),
           Building(1, "bunker", {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 270.0}),
           Building(1, "bunker", {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 359.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, BunkerPlacedInCenterOfABorderForAllProvincesOfStateIfDefaultsNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "bunker",
                                  {.x_coordinate = 3.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1, "bunker", {.x_coordinate = 4.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1, "bunker", {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}),
           Building(1, "bunker", {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, CoastalBunkerPlacedInCenterOfSeaBorderForAllProvincesOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 1},
                               {2, 1},
                               {3, 1},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 1},
                               {5, 1},
                               {6, 1},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 6},
                               {2, 6},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 6},
                               {5, 6},
                               {6, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "coastal_bunker",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoCoastalBunkersWhenNoCoastalProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   for (const auto building: buildings.GetBuildings())
   {
      EXPECT_NE(building.GetType(), "coastal_bunker");
   }
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoCoastalBunkersWhenCoastalProvincesNotInStates)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})}, {}),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not find state for province 1. Coastal bunker not set."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not find state for province 2. Coastal bunker not set."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not find state for province 3. Coastal bunker not set."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Could not find state for province 4. Coastal bunker not set."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, CoastalBunkersNotPlacedInProvincesWithNoBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 1 did not have any border points. Coastal bunkers not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 2 did not have any border points. Coastal bunkers not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 3 did not have any border points. Coastal bunkers not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 4 did not have any border points. Coastal bunkers not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, CoastalBunkerPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 1},
                               {2, 1},
                               {3, 1},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 1},
                               {5, 1},
                               {6, 1},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 6},
                               {2, 6},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 6},
                               {5, 6},
                               {6, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {
               {{2, 1}, "1"},
               {{5, 1}, "2"},
               {{2, 6}, "3"},
               {{5, 6}, "4"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "coastal_bunker",
                                  {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 180.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 270.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 359.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, CoastalBunkerPlacedInCenterOfSeaBorderForAllProvincesOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 1},
                               {2, 1},
                               {3, 1},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 1},
                               {5, 1},
                               {6, 1},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 6},
                               {2, 6},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 6},
                               {5, 6},
                               {6, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "coastal_bunker",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0}),
           Building(1,
               "coastal_bunker",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, DockyardPlacedInSeaBorderCenterOfFirstCoastalProvinceOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "dockyard",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 0.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoDockyardInStateWithNoCoastalProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   for (const auto building : buildings.GetBuildings())
   {
       EXPECT_NE(building.GetType(), "dockyard");
   }
}


TEST(Hoi4worldMapBuildingsCreatorTests, DockyardNotPlacedInProvincesWithNoBorderPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   for (const auto building: buildings.GetBuildings())
   {
      EXPECT_NE(building.GetType(), "dockyard");
   }
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. Dockyard may not be set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 2 did not have any points. Dockyard may not be set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 3 did not have any points. Dockyard may not be set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 4 did not have any points. Dockyard may not be set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, DockyardPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {
               {{2, 1}, "1"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "dockyard",
           {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, DockyardPlacedInSeaBorderCenterOfFirstCoastalProvinceOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "dockyard",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 0.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, FloatingHarborsPlacedInSeaBorderCenterOfFirstCoastalProvinceOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"5",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"6",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"7",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"8",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "floating_harbor",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 0.0, .rotation = 0.0},
           1)}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoFloatingHarborsInStateWithNoCoastalProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"5",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"6",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"7",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"8",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, FloatingHarborsNotPlacedInProvincesWithNoSeaBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({}, {}, {}, maps::ProvinceDefinitions{{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Could not find position for province 1. Floating Harbor not set."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Could not find position for province 2. Floating Harbor not set."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Could not find position for province 3. Floating Harbor not set."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Could not find position for province 4. Floating Harbor not set."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, FloatingHarborsPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"5",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"6",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"7",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"8",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {{{2, 0}, "5"}}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "floating_harbor",
           {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 0.0, .rotation = 90.0},
           1)}));
}


TEST(Hoi4worldMapBuildingsCreatorTests,
    FloatingHarborsPlacedInSeaBorderCenterOfFirstCoastalProvinceOfStateIfDefaultNotInActualProvince)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"5",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"6",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"7",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"8",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           maps::ProvinceDefinitions{{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "floating_harbor",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 0.0, .rotation = 0.0},
           1)}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, IndustrialComplexesPlacedInCenterOfFirstSixProvincesOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5, 6, 7})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
               {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
               {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
               {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "industrial_complex",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoIndustrialComplexesInStateWithNoProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
               {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
               {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
               {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, IndustrialComplexesNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5, 6, 7})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 1 did not have any points. industrial_complex not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 2 did not have any points. industrial_complex not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 3 did not have any points. industrial_complex not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 4 did not have any points. industrial_complex not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, IndustrialComplexesPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5, 6, 7})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
               {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
               {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
               {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
           },
           {{}, {}, {}, {}},
           {
               {{4, 1}, "2"},
               {{1, 4}, "3"},
               {{4, 4}, "4"},
               {{7, 1}, "5"},
               {{7, 4}, "6"},
               {{1, 7}, "7"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "industrial_complex",
                                  {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 45.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 90.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 135.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 7.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 180.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 7.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 225.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 7.0, .rotation = 270.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, IndustrialComplexesPlacedInCenterOfFirstSixProvincesOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5, 6, 7})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                                 {6, 1},
                                                 {7, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
               {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
               {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
               {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "arms_factory",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "industrial_complex",
               {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NavalBasesPlacedInCenterOfSeaBorderForAllProvincesOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 1},
                               {2, 1},
                               {3, 1},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 1},
                               {5, 1},
                               {6, 1},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 6},
                               {2, 6},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 6},
                               {5, 6},
                               {6, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "naval_base",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0},
                                  5),
           Building(1,
               "naval_base",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0},
               6),
           Building(1,
               "naval_base",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0},
               7),
           Building(1,
               "naval_base",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0},
               8)}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoNavalBasesWhenNoCoastalProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 0},
                               {2, 0},
                               {3, 0},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 0},
                               {5, 0},
                               {6, 0},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 7},
                               {2, 7},
                               {3, 7},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 7},
                               {5, 7},
                               {6, 7},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   for (const auto building: buildings.GetBuildings())
   {
      EXPECT_NE(building.GetType(), "naval_base");
   }
}


TEST(Hoi4worldMapBuildingsCreatorTests, NavalBasesNotPlacedInProvincesWithNoBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 1 did not have any border points. Naval bases not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 2 did not have any border points. Naval bases not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 3 did not have any border points. Naval bases not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 4 did not have any border points. Naval bases not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NavalBasesPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 1},
                               {2, 1},
                               {3, 1},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 1},
                               {5, 1},
                               {6, 1},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 6},
                               {2, 6},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 6},
                               {5, 6},
                               {6, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {
               {{2, 1}, "1"},
               {{5, 1}, "2"},
               {{2, 6}, "3"},
               {{5, 6}, "4"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "naval_base",
                                  {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0},
                                  5),
           Building(1,
               "naval_base",
               {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 180.0},
               6),
           Building(1,
               "naval_base",
               {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 270.0},
               7),
           Building(1,
               "naval_base",
               {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 359.0},
               8)}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NavalBasesPlacedInCenterOfSeaBorderForAllProvincesOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                       {"5",
                           maps::BorderPoints{
                               {1, 1},
                               {2, 1},
                               {3, 1},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                       {"6",
                           maps::BorderPoints{
                               {4, 1},
                               {5, 1},
                               {6, 1},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                       {"7",
                           maps::BorderPoints{
                               {1, 6},
                               {2, 6},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                       {"8",
                           maps::BorderPoints{
                               {4, 6},
                               {5, 6},
                               {6, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "naval_base",
                                  {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0},
                                  5),
           Building(1,
               "naval_base",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0},
               6),
           Building(1,
               "naval_base",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0},
               7),
           Building(1,
               "naval_base",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0},
               8)}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NuclearReactorPlacedInCenterOfFirstProvinceOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "nuclear_reactor",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoNuclearReactorInStateWithNoProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, NuclearReactorNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. nuclear_reactor not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 2 did not have any points. nuclear_reactor not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 3 did not have any points. nuclear_reactor not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 4 did not have any points. nuclear_reactor not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NuclearReactorPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {
               {{4, 1}, "2"},
               {{1, 4}, "3"},
               {{4, 4}, "4"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "nuclear_reactor",
           {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NuclearReactorPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "nuclear_reactor",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SupplyNodesPlacedInCenterOfABorderForAllProvincesOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "supply_node",
                                  {.x_coordinate = 3.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 4.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoSupplyNodesInStateWithNoProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})}, {}),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, SupplyNodesNotPlacedInProvincesWithNoBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 1 did not have any border points. Supply nodes not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 2 did not have any border points. Supply nodes not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 3 did not have any border points. Supply nodes not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 4 did not have any border points. Supply nodes not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SupplyNodePlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {
               {{3, 2}, "1"},
               {{4, 2}, "2"},
               {{2, 4}, "3"},
               {{5, 4}, "4"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "supply_node",
                                  {.x_coordinate = 3.0, .y_coordinate = 6.0, .z_coordinate = 2.0, .rotation = 90.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 2.0, .rotation = 180.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 270.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 359.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SupplyNodePlacedInCenterOfABorderForAllProvincesOfStateIfDefaultsNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {
               {"1",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {3, 1},
                               {3, 2},
                               {3, 3},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {1, 3},
                               {2, 3},
                               {3, 3},
                           }},
                   }},
               {"2",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {4, 1},
                               {4, 2},
                               {4, 3},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {4, 3},
                               {5, 3},
                               {6, 3},
                           }},
                   }},
               {"3",
                   maps::BordersWith{
                       {"1",
                           maps::BorderPoints{
                               {1, 4},
                               {2, 4},
                               {3, 4},
                           }},
                       {"4",
                           maps::BorderPoints{
                               {3, 4},
                               {3, 5},
                               {3, 6},
                           }},
                   }},
               {"4",
                   maps::BordersWith{
                       {"2",
                           maps::BorderPoints{
                               {4, 4},
                               {5, 4},
                               {6, 4},
                           }},
                       {"3",
                           maps::BorderPoints{
                               {4, 4},
                               {4, 5},
                               {4, 6},
                           }},
                   }},
           },
           {},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
                                  "supply_node",
                                  {.x_coordinate = 3.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 4.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}),
           Building(1,
               "supply_node",
               {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SyntheticRefineryPlacedInCenterOfFirstProvinceOfState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "synthetic_refinery",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoSyntheticRefineryInStateWithNoProvinces)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, SyntheticRefineryNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({}, {}, {}, {{}, {}, {}, {}}, {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 1 did not have any points. synthetic_refinery not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 2 did not have any points. synthetic_refinery not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 3 did not have any points. synthetic_refinery not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 4 did not have any points. synthetic_refinery not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SyntheticRefineryPlacementOverridenByDefaultLocations)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {
               {{4, 1}, "2"},
               {{1, 4}, "3"},
               {{4, 4}, "4"},
           }),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "synthetic_refinery",
           {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SyntheticRefineryPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   Buildings buildings = ImportBuildings(States({State(1, std::nullopt, {1, 2, 3, 4, 5})},
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
       CoastalProvinces({}),
       maps::MapData({},
           {},
           {
               {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
               {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
               {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
           },
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "synthetic_refinery",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0})}));
}

}  // namespace hoi4