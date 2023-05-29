#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/buildings/buildings.h"



namespace vic3
{

TEST(Vic3WorldBuildingsBuildingsTests, NoBuildingsNoGoodsValue)
{
   const Buildings buildings;

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInWorld(), 0.0F);
}


TEST(Vic3WorldBuildingsBuildingsTests, TotalGoodsValueInWorldAreReported)
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


TEST(Vic3WorldBuildingsBuildingsTests, NoGoodsValueInMissingState)
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


TEST(Vic3WorldBuildingsBuildingsTests, NoGoodsValueInStateWithNoBuildings)
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


TEST(Vic3WorldBuildingsBuildingsTests, NoBuildingsNoIndustryGoodsValue)
{
   const Buildings buildings;

   EXPECT_EQ(buildings.GetTotalIndustryGoodSalesValueInWorld(), 0.0F);
}


TEST(Vic3WorldBuildingsBuildingsTests, TotalIndustryGoodsValueInWorldAreReported)
{
   const Buildings buildings({
       {1,
           {
               Building("building_tooling_workshops", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("building_tooling_workshops", std::nullopt, 0.5F),
               Building("building_tooling_workshops", std::nullopt, 0.25F),
           }},
   });

   EXPECT_EQ(buildings.GetTotalIndustryGoodSalesValueInWorld(), 1.75F);
}


TEST(Vic3WorldBuildingsBuildingsTests, NoIndustryGoodsValueForNonIndustrialBuilding)
{
   const Buildings buildings({
       {1,
           {
               Building("building_tooling_workshops", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("a_farm", std::nullopt, 0.5F),
               Building("building_tooling_workshops", std::nullopt, 0.25F),
           }},
   });

   EXPECT_EQ(buildings.GetTotalIndustryGoodSalesValueInWorld(), 1.25F);
}


TEST(Vic3WorldBuildingsBuildingsTests, NoIndustryGoodsValueInMissingState)
{
   const Buildings buildings({
       {1,
           {
               Building("building_tooling_workshops", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("building_tooling_workshops", std::nullopt, 0.5F),
               Building("building_tooling_workshops", std::nullopt, 0.25F),
           }},
   });

   EXPECT_EQ(buildings.GetTotalIndustryGoodSalesValueInState(3), 0.0F);
}


TEST(Vic3WorldBuildingsBuildingsTests, NoIndustryGoodsValueInStateWithNoBuildings)
{
   const Buildings buildings({
       {1,
           {
               Building("building_tooling_workshops", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("building_tooling_workshops", std::nullopt, 0.5F),
               Building("building_tooling_workshops", std::nullopt, 0.25F),
           }},
       {3, {}},
   });

   EXPECT_EQ(buildings.GetTotalIndustryGoodSalesValueInState(3), 0.0F);
}


TEST(Vic3WorldBuildingsBuildingsTests, TotalIndustryGoodsValueInStateAreReported)
{
   const Buildings buildings({
       {1,
           {
               Building("building_tooling_workshops", std::nullopt, 1.0F),
           }},
       {2,
           {
               Building("building_tooling_workshops", std::nullopt, 0.5F),
               Building("building_tooling_workshops", std::nullopt, 0.25F),
           }},
   });

   EXPECT_EQ(buildings.GetTotalIndustryGoodSalesValueInState(2), 0.75F);
}


}  // namespace vic3