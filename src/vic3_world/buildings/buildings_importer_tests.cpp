#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/buildings/buildings_importer.h"



namespace vic3
{

TEST(Vic3WorldBuildingsBuildingsImporterTests, BuildingsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0={\n";
   input << "building = building_iron_mine\n";
   input << "state = 1\n";
   input << "goods_sales = 1.0\n";
   input << "}\n";
   input << "1={\n";
   input << "building = building_iron_mine\n";
   input << "state = 1\n";
   input << "goods_sales = 0.5\n";
   input << "}\n";
   input << "2={\n";
   input << "building = building_iron_mine\n";
   input << "state = 2\n";
   input << "goods_sales = 0.25\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const Buildings buildings = ImportBuildings(input);

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInWorld(), 1.75F);
   EXPECT_EQ(buildings.GetTotalGoodSalesValueInState(1), 1.5F);
   EXPECT_EQ(buildings.GetTotalGoodSalesValueInState(2), 0.25F);
   EXPECT_TRUE(buildings.GetBuildingInState(2, "building_iron_mine").has_value());
   EXPECT_FALSE(buildings.GetBuildingInState(2, "nonesuch").has_value());
}


TEST(Vic3WorldBuildingsBuildingsImporterTests, BuildingsWithNoStateAreNotImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0={\n";
   input << "building = building_iron_mine\n";
   input << "goods_sales = 1.0\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const Buildings buildings = ImportBuildings(input);

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInWorld(), 0.0F);
}


TEST(Vic3WorldBuildingsBuildingsImporterTests, BuildingsCanBeSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0={\n";
   input << "building = building_iron_mine\n";
   input << "state = 1\n";
   input << "goods_sales = 1.0\n";
   input << "}\n";
   input << "1={\n";
   input << "building = building_iron_mine\n";
   input << "state = 1\n";
   input << "goods_sales = 0.5\n";
   input << "}\n";
   input << "2=none\n";
   input << "\t}\n";
   input << "}\n";

   const Buildings buildings = ImportBuildings(input);

   EXPECT_EQ(buildings.GetTotalGoodSalesValueInWorld(), 1.5F);
}

}  // namespace vic3
