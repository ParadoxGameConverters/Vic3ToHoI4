#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/buildings/buildings.h"



namespace vic3
{

TEST(Vic3WorldBuildingsBuildingsImporterTests, NoBuildingsNoGoodsValue)
{
   const Buildings buildings;

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInWorld(), 0.0F);
}


TEST(Vic3WorldBuildingsBuildingsImporterTests, TotalGoodsValueInWorldAreReported)
{
   const Buildings buildings({
       {1,
           {
               Building("", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("", std::nullopt, 0.5F),
               Building("", std::nullopt, 0.25F),
           }},
   });

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInWorld(), 1.75F);
}


TEST(Vic3WorldBuildingsBuildingsImporterTests, NonGoodsValueInMissingState)
{
   const Buildings buildings({
       {1,
           {
               Building("", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("", std::nullopt, 0.5F),
               Building("", std::nullopt, 0.25F),
           }},
   });

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInState(3), 0.0F);
}


TEST(Vic3WorldBuildingsBuildingsImporterTests, NonGoodsValueInStateWithNoBuildings)
{
   const Buildings buildings({{1,
                                  {
                                      Building("", std::nullopt, 1.0F),
                                  }},
       {2,
           {
               Building("", std::nullopt, 0.5F),
               Building("", std::nullopt, 0.25F),
           }},
       {3, {}}});

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInState(3), 0.0F);
}


TEST(Vic3WorldBuildingsBuildingsTests, TotalGoodsValueInStateAreReported)
{
   const Buildings buildings({
       {1,
           {
               Building("", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("", std::nullopt, 0.5F),
               Building("", std::nullopt, 0.25F),
           }},
   });

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInState(2), 0.75F);
}

TEST(Vic3WorldBuildingsBuildingsTests, BuildingsInNonexistentStateIsEmptyList)
{
   const Buildings buildings({
       {1,
           {
               Building("", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("", std::nullopt, 0.5F),
               Building("", std::nullopt, 0.25F),
           }},
   });

   EXPECT_TRUE(buildings.GetBuildingsInState(100).empty());
}

TEST(Vic3WorldBuildingsBuildingsTests, BuildingsCanBeReturnedByState)
{
   const Buildings buildings({
       {1,
           {
               Building("", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("", std::nullopt, 0.5F),
               Building("", std::nullopt, 0.25F),
           }},
   });

   EXPECT_THAT(buildings.GetBuildingsInState(2),
       testing::UnorderedElementsAre(Building("", std::nullopt, 0.5F), Building("", std::nullopt, 0.25F)));
}

}  // namespace vic3