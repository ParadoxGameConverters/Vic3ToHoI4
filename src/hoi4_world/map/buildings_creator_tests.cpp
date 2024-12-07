#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/buildings_creator.h"



namespace hoi4
{

TEST(Hoi4worldMapBuildingsCreatorTests, ExceptionForMissingBuildingsDotTxt)
{
   EXPECT_THROW(ImportBuildings(States(),
                    CoastalProvinces(),
                    maps::MapData(),
                    commonItems::ModFilesystem{"ExceptionForMissingBuildingsDotTxt", {}}),
       std::runtime_error);
}


TEST(Hoi4worldMapBuildingsCreatorTests, DefaultsToNoBuildings)
{
   const Buildings buildings = ImportBuildings(States(),
       CoastalProvinces(),
       maps::MapData(),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportPlacedInCenterOfFirstProvinceOfState)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                           },
                       .points_to_provinces =
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
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "air_base",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoAirportInStateWithNoProvinces)
{
   // clang-format off
   const Buildings
       buildings =
           ImportBuildings(States({
                               .states = {State(1, {})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   },
                           }),
               CoastalProvinces(),
               maps::MapData(
                   {
                       .the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints({
                                       {1, 1},
                                       {1, 2},
                                       {1, 3},
                                       {2, 1},
                                       {2, 2},
                                       {2, 3},
                                       {3, 1},
                                       {3, 2},
                                       {3, 3},
                                   })},
                               {"2",
                                   maps::ProvincePoints({
                                       {4, 1},
                                       {4, 2},
                                       {4, 3},
                                       {5, 1},
                                       {5, 2},
                                       {5, 3},
                                       {6, 1},
                                       {6, 2},
                                       {6, 3},
                                   })},
                           },
                       .points_to_provinces =
                           {
                               {maps::Point{2, 2}, "1"},
                           },
                   }),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});
   // clang-format on

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportNotPlacedInProvinceWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. air_base not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportPlacementOverridenByDefaultLocation)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                           },
                       .points_to_provinces =
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
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "air_base",
           .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AirportPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                           },
                       .points_to_provinces =
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
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "air_base",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirPlacedInCenterOfFirstThreeProvincesOfState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "anti_air_building",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "anti_air_building",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "anti_air_building",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoAntiAirInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
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
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                               {"3",
                                   maps::ProvincePoints(
                                       {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                           },
                       .points_to_provinces =
                           {
                               {{4, 1}, "2"},
                               {{1, 4}, "3"},
                               {{4, 4}, "4"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "anti_air_building",
                .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}}),
               Building({.state_id = 1,
                   .type = "anti_air_building",
                   .position = {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 180.0}}),
               Building({.state_id = 1,
                   .type = "anti_air_building",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 270.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, AntiAirPlacedInCenterOfFirstThreeProvincesOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "anti_air_building",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "anti_air_building",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "anti_air_building",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, ArmsFactoriesPlacedInCenterOfFirstSixProvincesOfState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5, 6, 7}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                           {6, 1},
                                                           {7, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                       {"5",
                           maps::ProvincePoints(
                               {{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
                       {"6",
                           maps::ProvincePoints(
                               {{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
                       {"7",
                           maps::ProvincePoints(
                               {{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "arms_factory",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoArmsFactoriesInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                           {6, 1},
                                                           {7, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                       {"5",
                           maps::ProvincePoints(
                               {{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
                       {"6",
                           maps::ProvincePoints(
                               {{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
                       {"7",
                           maps::ProvincePoints(
                               {{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, ArmsFactoriesNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5, 6, 7}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                           {6, 1},
                                                           {7, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
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
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5, 6, 7}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                       {6, 1},
                                       {7, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                               {"3",
                                   maps::ProvincePoints(
                                       {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                               {"5",
                                   maps::ProvincePoints(
                                       {{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
                               {"6",
                                   maps::ProvincePoints(
                                       {{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
                               {"7",
                                   maps::ProvincePoints(
                                       {{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
                           },
                       .points_to_provinces =
                           {
                               {{4, 1}, "2"},
                               {{1, 4}, "3"},
                               {{4, 4}, "4"},
                               {{7, 1}, "5"},
                               {{7, 4}, "6"},
                               {{1, 7}, "7"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "arms_factory",
                .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 45.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 90.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 135.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 7.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 180.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 7.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 225.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 7.0, .rotation = 270.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, ArmsFactoriesPlacedInCenterOfFirstSixProvincesOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5, 6, 7}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                           {6, 1},
                                                           {7, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                       {"5",
                           maps::ProvincePoints(
                               {{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
                       {"6",
                           maps::ProvincePoints(
                               {{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
                       {"7",
                           maps::ProvincePoints(
                               {{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "arms_factory",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "arms_factory",
                   .position = {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, BunkerPlacedInCenterOfABorderForAllProvincesOfState)
{
   const States states = {.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }};

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       maps::MapData({.borders = borders}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "bunker",
                .position = {.x_coordinate = 3.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoBunkersInStateWithNoProvinces)
{
   const States states({.states = {State(1, {})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   for (const auto& building: buildings.GetBuildings())
   {
      EXPECT_NE(building.GetType(), "bunker");
   }
}


TEST(Hoi4worldMapBuildingsCreatorTests, BunkersNotPlacedInProvincesWithNoBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
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
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders,
       .points_to_provinces = {
           {{3, 2}, "1"},
           {{4, 2}, "2"},
           {{2, 4}, "3"},
           {{5, 4}, "4"},
       }});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "bunker",
                .position = {.x_coordinate = 3.0, .y_coordinate = 6.0, .z_coordinate = 2.0, .rotation = 90.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 2.0, .rotation = 180.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 270.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 359.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, BunkerPlacedInCenterOfABorderForAllProvincesOfStateIfDefaultsNotInState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "bunker",
                .position = {.x_coordinate = 3.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, CoastalBunkerPlacedInCenterOfSeaBorderForAllProvincesOfState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "coastal_bunker",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoCoastalBunkersWhenNoCoastalProvinces)
{
   const States states({.states = {State(1, {})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   for (const auto& building: buildings.GetBuildings())
   {
      EXPECT_NE(building.GetType(), "coastal_bunker");
   }
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoCoastalBunkersWhenCoastalProvincesNotInStates)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const States states({.states = {State(1, {})}, .province_to_state_id_map = {}});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
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

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                       }}),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData(),
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
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders,
       .points_to_provinces = {
           {{2, 1}, "1"},
           {{5, 1}, "2"},
           {{2, 6}, "3"},
           {{5, 6}, "4"},
       }});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "coastal_bunker",
                .position = {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 180.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 270.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 359.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, CoastalBunkerPlacedInCenterOfSeaBorderForAllProvincesOfStateIfDefaultNotInState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "coastal_bunker",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "coastal_bunker",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, DockyardPlacedInSeaBorderCenterOfFirstCoastalProvinceOfState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders{
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "dockyard",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 0.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoDockyardInStateWithNoCoastalProvinces)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   for (const auto& building: buildings.GetBuildings())
   {
      EXPECT_NE(building.GetType(), "dockyard");
   }
}


TEST(Hoi4worldMapBuildingsCreatorTests, DockyardNotPlacedInProvincesWithNoBorderPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const State state(1, {.provinces = {1, 2, 3, 4}});

   const std::map<int, int> province_to_state_id_map = {
       {1, 1},
       {2, 1},
       {3, 1},
       {4, 1},
   };
   const States states({.states = {state}, .province_to_state_id_map = province_to_state_id_map});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const commonItems::ModFilesystem mod_filesystem("test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings",
       {});

   const Buildings buildings = ImportBuildings(states, coastal_provinces, map_data, mod_filesystem);

   std::cout.rdbuf(cout_buffer);

   for (const auto& building: buildings.GetBuildings())
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
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders,
       .points_to_provinces = {
           {{2, 1}, "1"},
       }});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "dockyard",
           .position = {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, DockyardPlacedInSeaBorderCenterOfFirstCoastalProvinceOfStateIfDefaultNotInState)
{
   States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "dockyard",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 0.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, FloatingHarborsPlacedInSeaBorderCenterOfFirstCoastalProvinceOfState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "floating_harbor",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 0.0, .rotation = 0.0},
           .connecting_sea_province = 1})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoFloatingHarborsInStateWithNoCoastalProvinces)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, FloatingHarborsNotPlacedInProvincesWithNoSeaBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                       }}),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData(),
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
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders, .points_to_provinces = {{{2, 0}, "5"}}});

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "floating_harbor",
           .position = {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 0.0, .rotation = 90.0},
           .connecting_sea_province = 1})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests,
    FloatingHarborsPlacedInSeaBorderCenterOfFirstCoastalProvinceOfStateIfDefaultNotInActualProvince)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       maps::MapData({.borders = borders}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "floating_harbor",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 0.0, .rotation = 0.0},
           .connecting_sea_province = 1})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, IndustrialComplexesPlacedInCenterOfFirstSixProvincesOfState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5, 6, 7}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
           {6, 1},
           {7, 1},
       }});

   const std::unordered_map<std::string, maps::ProvincePoints> the_province_points = {
       {"1", maps::ProvincePoints({{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
       {"2", maps::ProvincePoints({{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
       {"3", maps::ProvincePoints({{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
       {"4", maps::ProvincePoints({{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
       {"5", maps::ProvincePoints({{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
       {"6", maps::ProvincePoints({{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
       {"7", maps::ProvincePoints({{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
   };

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       maps::MapData({.the_province_points = the_province_points}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "industrial_complex",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoIndustrialComplexesInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                           {6, 1},
                                                           {7, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                       {"5",
                           maps::ProvincePoints(
                               {{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
                       {"6",
                           maps::ProvincePoints(
                               {{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
                       {"7",
                           maps::ProvincePoints(
                               {{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, IndustrialComplexesNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5, 6, 7}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                           {6, 1},
                                                           {7, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
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
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5, 6, 7}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                       {6, 1},
                                       {7, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                               {"3",
                                   maps::ProvincePoints(
                                       {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                               {"5",
                                   maps::ProvincePoints(
                                       {{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
                               {"6",
                                   maps::ProvincePoints(
                                       {{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
                               {"7",
                                   maps::ProvincePoints(
                                       {{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
                           },
                       .points_to_provinces =
                           {
                               {{4, 1}, "2"},
                               {{1, 4}, "3"},
                               {{4, 4}, "4"},
                               {{7, 1}, "5"},
                               {{7, 4}, "6"},
                               {{1, 7}, "7"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "industrial_complex",
                .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 45.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 90.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 135.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 7.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 180.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 7.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 225.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 1.0, .y_coordinate = 6.0, .z_coordinate = 7.0, .rotation = 270.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, IndustrialComplexesPlacedInCenterOfFirstSixProvincesOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5, 6, 7}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                           {6, 1},
                                                           {7, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                       {"5",
                           maps::ProvincePoints(
                               {{7, 1}, {7, 2}, {7, 3}, {8, 1}, {8, 2}, {8, 3}, {9, 1}, {9, 2}, {9, 3}})},
                       {"6",
                           maps::ProvincePoints(
                               {{7, 4}, {7, 5}, {7, 6}, {8, 4}, {8, 5}, {8, 6}, {9, 4}, {9, 5}, {9, 6}})},
                       {"7",
                           maps::ProvincePoints(
                               {{1, 7}, {1, 8}, {1, 9}, {2, 7}, {2, 8}, {2, 9}, {3, 7}, {3, 8}, {3, 9}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "arms_factory",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "industrial_complex",
                   .position = {.x_coordinate = 8.0, .y_coordinate = 11.0, .z_coordinate = 5.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NavalBasesPlacedInCenterOfSeaBorderForAllProvincesOfState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       maps::MapData({.borders = borders}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "naval_base",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0},
                .connecting_sea_province = 5}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0},
                   .connecting_sea_province = 6}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0},
                   .connecting_sea_province = 7}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0},
                   .connecting_sea_province = 8})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoNavalBasesWhenNoCoastalProvinces)
{
   const States states({.states = {State(1, {})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   for (const auto& building: buildings.GetBuildings())
   {
      EXPECT_NE(building.GetType(), "naval_base");
   }
}


TEST(Hoi4worldMapBuildingsCreatorTests, NavalBasesNotPlacedInProvincesWithNoBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                       }}),
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       maps::MapData(),
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
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});


   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders,
       .points_to_provinces = {
           {{2, 1}, "1"},
           {{5, 1}, "2"},
           {{2, 6}, "3"},
           {{5, 6}, "4"},
       }});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}}),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "naval_base",
                .position = {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0},
                .connecting_sea_province = 5}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 180.0},
                   .connecting_sea_province = 6}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 270.0},
                   .connecting_sea_province = 7}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 359.0},
                   .connecting_sea_province = 8})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NavalBasesPlacedInCenterOfSeaBorderForAllProvincesOfStateIfDefaultNotInState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});

   const CoastalProvinces coastal_provinces({{1, {5}}, {2, {6}}, {3, {7}}, {4, {8}}});

   const std::map<std::string, maps::BordersWith> borders = {{"1",
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
           }}};

   const Buildings buildings = ImportBuildings(states,
       coastal_provinces,
       maps::MapData({.borders = borders}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "naval_base",
                .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0},
                .connecting_sea_province = 5}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 1.0, .rotation = 0.0},
                   .connecting_sea_province = 6}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0},
                   .connecting_sea_province = 7}),
               Building({.state_id = 1,
                   .type = "naval_base",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 6.0, .rotation = 0.0},
                   .connecting_sea_province = 8})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NuclearReactorPlacedInCenterOfFirstProvinceOfState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "nuclear_reactor",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoNuclearReactorInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, NuclearReactorNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
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
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   },
               .points_to_provinces{
                   {{4, 1}, "2"},
                   {{1, 4}, "3"},
                   {{4, 4}, "4"},
               }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "nuclear_reactor",
           .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NuclearReactorPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "nuclear_reactor",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SupplyNodesPlacedInCenterOfABorderForAllProvincesOfState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {{"1",
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
           }}};

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       maps::MapData({.borders = borders}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "supply_node",
                .position = {.x_coordinate = 3.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoSupplyNodesInStateWithNoProvinces)
{
   const States states({.states = {State(1, {})}, .province_to_state_id_map = {}});

   const std::map<std::string, maps::BordersWith> borders = {{"1",
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
           }}};

   const maps::MapData map_data({.borders = borders});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, SupplyNodesNotPlacedInProvincesWithNoBorders)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
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
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {
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
   };

   const maps::MapData map_data({.borders = borders,
       .points_to_provinces = {
           {{3, 2}, "1"},
           {{4, 2}, "2"},
           {{2, 4}, "3"},
           {{5, 4}, "4"},
       }});

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       map_data,
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "supply_node",
                .position = {.x_coordinate = 3.0, .y_coordinate = 6.0, .z_coordinate = 2.0, .rotation = 90.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 2.0, .rotation = 180.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 270.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 6.0, .z_coordinate = 4.0, .rotation = 359.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SupplyNodePlacedInCenterOfABorderForAllProvincesOfStateIfDefaultsNotInState)
{
   const States states({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
       .province_to_state_id_map = {
           {1, 1},
           {2, 1},
           {3, 1},
           {4, 1},
           {5, 1},
       }});

   const std::map<std::string, maps::BordersWith> borders = {{"1",
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
           }}};

   const Buildings buildings = ImportBuildings(states,
       CoastalProvinces(),
       maps::MapData({.borders = borders}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf(
           {Building({.state_id = 1,
                .type = "supply_node",
                .position = {.x_coordinate = 3.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 4.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}}),
               Building({.state_id = 1,
                   .type = "supply_node",
                   .position = {.x_coordinate = 5.0, .y_coordinate = 11.0, .z_coordinate = 4.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SyntheticRefineryPlacedInCenterOfFirstProvinceOfState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "synthetic_refinery",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoSyntheticRefineryInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, SyntheticRefineryNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
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
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                               {"3",
                                   maps::ProvincePoints(
                                       {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                           },
                       .points_to_provinces =
                           {
                               {{4, 1}, "2"},
                               {{1, 4}, "3"},
                               {{4, 4}, "4"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "synthetic_refinery",
           .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SyntheticRefineryPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "synthetic_refinery",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, RocketSitePlacedInCenterOfFirstProvinceOfState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "rocket_site_spawn",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoRocketSiteInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, RocketSiteNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. rocket_site_spawn not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 2 did not have any points. rocket_site_spawn not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 3 did not have any points. rocket_site_spawn not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 4 did not have any points. rocket_site_spawn not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, RocketSitePlacementOverridenByDefaultLocations)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                               {"3",
                                   maps::ProvincePoints(
                                       {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                           },
                       .points_to_provinces =
                           {
                               {{4, 1}, "2"},
                               {{1, 4}, "3"},
                               {{4, 4}, "4"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "rocket_site_spawn",
           .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, RocketSitePlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "rocket_site_spawn",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, RadarStationPlacedInCenterOfFirstProvinceOfState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "radar_station",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoRadarStationInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, RadarStationNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. radar_station not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 2 did not have any points. radar_station not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 3 did not have any points. radar_station not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 4 did not have any points. radar_station not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, RadarStationPlacementOverridenByDefaultLocations)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                               {"3",
                                   maps::ProvincePoints(
                                       {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                           },
                       .points_to_provinces =
                           {
                               {{4, 1}, "2"},
                               {{1, 4}, "3"},
                               {{4, 4}, "4"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "radar_station",
           .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, RadarStationPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "radar_station",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}

TEST(Hoi4worldMapBuildingsCreatorTests, FuelSiloPlacedInCenterOfFirstProvinceOfState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "fuel_silo",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoFuelSiloInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, FuelSiloNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 1 did not have any points. fuel_silo not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 2 did not have any points. fuel_silo not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 3 did not have any points. fuel_silo not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Province 4 did not have any points. fuel_silo not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, FuelSiloPlacementOverridenByDefaultLocations)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                               {"3",
                                   maps::ProvincePoints(
                                       {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                           },
                       .points_to_provinces =
                           {
                               {{4, 1}, "2"},
                               {{1, 4}, "3"},
                               {{4, 4}, "4"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "fuel_silo",
           .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, FuelSiloPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "fuel_silo",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, SpecialProjectFacilitesPlacedIndefaultLocationsAndOnlyInDefaultLocations)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states =
                                       {
                                           State(1, {.provinces = {1, 2, 3, 4, 5}}),
                                           State(2, {.provinces = {6, 7, 8, 9, 10}}),
                                           State(3, {.provinces = {11, 12, 13, 14, 15}}),
                                       },
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                       {6, 2},
                                       {7, 2},
                                       {8, 2},
                                       {9, 2},
                                       {10, 2},
                                       {11, 3},
                                       {12, 3},
                                       {13, 3},
                                       {14, 3},
                                       {15, 3},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                               {"7",
                                   maps::ProvincePoints(
                                       {{7, 7}, {7, 8}, {7, 9}, {8, 7}, {8, 8}, {8, 9}, {9, 7}, {9, 8}, {9, 9}})},
                           },
                       .points_to_provinces =
                           {
                               {{1, 3}, "1"},
                               {{4, 6}, "4"},
                               {{7, 9}, "7"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({
           Building({.state_id = 1,
               .type = "special_project_facility_spawn",
               .position = {.x_coordinate = 1.0, .y_coordinate = 3.0, .z_coordinate = 3.0, .rotation = 90.0}}),
           Building({.state_id = 1,
               .type = "special_project_facility_spawn",
               .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 135.0}}),
           Building({.state_id = 2,
               .type = "special_project_facility_spawn",
               .position = {.x_coordinate = 7.0, .y_coordinate = 9.0, .z_coordinate = 9.0, .rotation = 180.0}}),
       }));
}


TEST(Hoi4worldMapBuildingsCreatorTests, LandmarksPlacedIndefaultLocationsAndOnlyInDefaultLocations)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states =
                                       {
                                           State(1, {.provinces = {1, 2, 3, 4, 5}}),
                                           State(2, {.provinces = {6, 7, 8, 9, 10}}),
                                           State(3, {.provinces = {11, 12, 13, 14, 15}}),
                                       },
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                       {6, 2},
                                       {7, 2},
                                       {8, 2},
                                       {9, 2},
                                       {10, 2},
                                       {11, 3},
                                       {12, 3},
                                       {13, 3},
                                       {14, 3},
                                       {15, 3},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                               {"7",
                                   maps::ProvincePoints(
                                       {{7, 7}, {7, 8}, {7, 9}, {8, 7}, {8, 8}, {8, 9}, {9, 7}, {9, 8}, {9, 9}})},
                           },
                       .points_to_provinces =
                           {
                               {{1, 3}, "1"},
                               {{4, 6}, "4"},
                               {{7, 9}, "7"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({
           Building({.state_id = 1,
               .type = "landmark_spawn",
               .position = {.x_coordinate = 1.0, .y_coordinate = 3.0, .z_coordinate = 3.0, .rotation = 90.0}}),
           Building({.state_id = 1,
               .type = "landmark_spawn",
               .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 135.0}}),
           Building({.state_id = 2,
               .type = "landmark_spawn",
               .position = {.x_coordinate = 7.0, .y_coordinate = 9.0, .z_coordinate = 9.0, .rotation = 180.0}}),
       }));
}


TEST(Hoi4worldMapBuildingsCreatorTests, StrongholdNetworkPlacedInCenterOfFirstProvinceOfState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "stronghold_network",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, NoStrongholdNetworkInStateWithNoProvinces)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   EXPECT_TRUE(buildings.GetBuildings().empty());
}


TEST(Hoi4worldMapBuildingsCreatorTests, StrongholdNetworkNotPlacedInProvincesWithNoPoints)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsToNoBuildings", {}});

   std::cout.rdbuf(cout_buffer);

   EXPECT_TRUE(buildings.GetBuildings().empty());
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 1 did not have any points. stronghold_network not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 2 did not have any points. stronghold_network not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 3 did not have any points. stronghold_network not fully set in state 1."));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[WARNING] Province 4 did not have any points. stronghold_network not fully set in state 1."));
}


TEST(Hoi4worldMapBuildingsCreatorTests, StrongholdNetworkPlacementOverridenByDefaultLocations)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"2",
                                   maps::ProvincePoints(
                                       {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                               {"3",
                                   maps::ProvincePoints(
                                       {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                           },
                       .points_to_provinces =
                           {
                               {{4, 1}, "2"},
                               {{1, 4}, "3"},
                               {{4, 4}, "4"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "stronghold_network",
           .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 1.0, .rotation = 90.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, StrongholdNetworkPlacedInCenterOfFirstProvinceOfStateIfDefaultNotInState)
{
   const Buildings buildings = ImportBuildings(States({.states = {State(1, {.provinces = {1, 2, 3, 4, 5}})},
                                                   .province_to_state_id_map =
                                                       {
                                                           {1, 1},
                                                           {2, 1},
                                                           {3, 1},
                                                           {4, 1},
                                                           {5, 1},
                                                       }}),
       CoastalProvinces(),
       maps::MapData(
           {.the_province_points =
                   {
                       {"1",
                           maps::ProvincePoints(
                               {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                       {"2",
                           maps::ProvincePoints(
                               {{4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2}, {6, 3}})},
                       {"3",
                           maps::ProvincePoints(
                               {{1, 4}, {1, 5}, {1, 6}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}})},
                       {"4",
                           maps::ProvincePoints(
                               {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                   }}),
       commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({Building({.state_id = 1,
           .type = "stronghold_network",
           .position = {.x_coordinate = 2.0, .y_coordinate = 11.0, .z_coordinate = 2.0, .rotation = 0.0}})}));
}


TEST(Hoi4worldMapBuildingsCreatorTests, DamsPlacedIndefaultLocationsAndOnlyInDefaultLocations)
{
   const Buildings
       buildings =
           ImportBuildings(States({.states =
                                       {
                                           State(1, {.provinces = {1, 2, 3, 4, 5}}),
                                           State(2, {.provinces = {6, 7, 8, 9, 10}}),
                                           State(3, {.provinces = {11, 12, 13, 14, 15}}),
                                       },
                               .province_to_state_id_map =
                                   {
                                       {1, 1},
                                       {2, 1},
                                       {3, 1},
                                       {4, 1},
                                       {5, 1},
                                       {6, 2},
                                       {7, 2},
                                       {8, 2},
                                       {9, 2},
                                       {10, 2},
                                       {11, 3},
                                       {12, 3},
                                       {13, 3},
                                       {14, 3},
                                       {15, 3},
                                   }}),
               CoastalProvinces(),
               maps::MapData(
                   {.the_province_points =
                           {
                               {"1",
                                   maps::ProvincePoints(
                                       {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}})},
                               {"4",
                                   maps::ProvincePoints(
                                       {{4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}})},
                               {"7",
                                   maps::ProvincePoints(
                                       {{7, 7}, {7, 8}, {7, 9}, {8, 7}, {8, 8}, {8, 9}, {9, 7}, {9, 8}, {9, 9}})},
                           },
                       .points_to_provinces =
                           {
                               {{1, 3}, "1"},
                               {{4, 6}, "4"},
                               {{7, 9}, "7"},
                           }}),
               commonItems::ModFilesystem{"test_files/Hoi4worldMapBuildingsCreatorTests/DefaultsExist", {}});

   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({
           Building({.state_id = 1,
               .type = "dam_spawn",
               .position = {.x_coordinate = 1.0, .y_coordinate = 3.0, .z_coordinate = 3.0, .rotation = 90.0}}),
           Building({.state_id = 1,
               .type = "dam_spawn",
               .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 135.0}}),
           Building({.state_id = 2,
               .type = "dam_spawn",
               .position = {.x_coordinate = 7.0, .y_coordinate = 9.0, .z_coordinate = 9.0, .rotation = 180.0}}),
       }));
   EXPECT_THAT(buildings.GetBuildings(),
       testing::IsSupersetOf({
           Building({.state_id = 1,
               .type = "locks_spawn",
               .position = {.x_coordinate = 1.0, .y_coordinate = 3.0, .z_coordinate = 3.0, .rotation = 90.0}}),
           Building({.state_id = 1,
               .type = "locks_spawn",
               .position = {.x_coordinate = 4.0, .y_coordinate = 6.0, .z_coordinate = 6.0, .rotation = 135.0}}),
           Building({.state_id = 2,
               .type = "locks_spawn",
               .position = {.x_coordinate = 7.0, .y_coordinate = 9.0, .z_coordinate = 9.0, .rotation = 180.0}}),
       }));
}

}  // namespace hoi4