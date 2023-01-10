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
           {{"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})}},
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
           {{"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})}},
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
           {{"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
               {"2", maps::ProvincePoints({{5, 7}, {5, 8}, {5, 9}, {6, 7}, {6, 8}, {6, 9}, {7, 7}, {7, 8}, {7, 9}})}},
           {{}, {}, {}, {}},
           {{{5, 7}, "2"}}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "air_base",
           {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 7.0, .rotation = 90.0})}));
   EXPECT_THAT(buildings.GetAirportLocations(), testing::UnorderedElementsAre(testing::Pair(1, 2)));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
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
           {{"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})}},
           {{}, {}, {}, {}},
           {}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building(1,
           "air_base",
           {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0})}));
   EXPECT_THAT(buildings.GetAirportLocations(), testing::UnorderedElementsAre(testing::Pair(1, 1)));
}

}  // namespace hoi4