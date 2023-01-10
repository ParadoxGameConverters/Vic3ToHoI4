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
           {}),
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
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
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
       testing::HasSubstr("[WARNING] Province 1 did not have any points. Airport not set in state 1."));
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
           {{{4, 1}, "2"}}),
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
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "air_base",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0})}));
   EXPECT_THAT(buildings.GetAirportLocations(), testing::UnorderedElementsAre(testing::Pair(1, 1)));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirPlacedInCenterOfFirstThreeProvincesOfState)
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
                                                     {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                                                     {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                                                 },
                                                 { {}, {}, {}, {} },
                                                 {}),
                                                 commonItems::ModFilesystem{ "test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {} });

    EXPECT_THAT(buildings.GetBuildings(),
        testing::IsSupersetOf({ Building(1,
            "anti_air_building",
            {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
            Building(1,
            "anti_air_building",
            {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
            Building(1,
            "anti_air_building",
            {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0})
            }));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoAntiAirInStateWithNoProvinces)
{
    Buildings buildings = ImportBuildings(States({ State(1, std::nullopt, {}) },
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
                                                 { {}, {}, {}, {} },
                                                 {}),
                                                 commonItems::ModFilesystem{ "test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {} });

    EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirNotPlacedInProvinceWithNoPoints)
{
    std::stringstream log;
    std::streambuf* cout_buffer = std::cout.rdbuf();
    std::cout.rdbuf(log.rdbuf());

    Buildings buildings = ImportBuildings(States({ State(1, std::nullopt, {1, 2, 3, 4, 5}) },
                                             {
                                                 {1, 1},
                                                 {2, 1},
                                                 {3, 1},
                                                 {4, 1},
                                                 {5, 1},
                                             }),
                                             CoastalProvinces({}),
                                             maps::MapData({}, {}, {}, { {}, {}, {}, {} }, {}),
                                             commonItems::ModFilesystem{ "test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {} });

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
                                                     {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                                                     {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                                                 },
                                                 { {}, {}, {}, {} },
                                                 {
                                                     {{4, 1}, "2"},
                                                     {{1, 4}, "3"},
                                                     {{4, 4}, "4"},
                                                 }),
                                                 commonItems::ModFilesystem{ "test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {} });

    EXPECT_THAT(buildings.GetBuildings(),
        testing::IsSupersetOf({ Building(1,
            "anti_air_building",
            {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}),
            Building(1,
            "anti_air_building",
            {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 180.0}),
            Building(1,
            "anti_air_building",
            {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 270.0})
            }));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
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
                                                     {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                                                     {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                                                 },
                                                 { {}, {}, {}, {} },
                                                 {}),
                                                 commonItems::ModFilesystem{ "test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {} });

    EXPECT_THAT(buildings.GetBuildings(),
        testing::IsSupersetOf({ Building(1,
            "anti_air_building",
            {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
            Building(1,
            "anti_air_building",
            {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}),
            Building(1,
            "anti_air_building",
            {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0})
            }));
}

}  // namespace hoi4