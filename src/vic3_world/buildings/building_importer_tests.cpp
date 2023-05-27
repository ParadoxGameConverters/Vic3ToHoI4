#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/buildings/building_importer.h"



namespace vic3
{

TEST(Vic3WorldBuildingsBuildingImporterTests, DefaultsAreDefaulted)
{
   std::stringstream input;

   const Building building = BuildingImporter{}.ImportBuilding(input);

   EXPECT_TRUE(building.GetType().empty());
   EXPECT_EQ(building.GetStateNumber(), std::nullopt);
   EXPECT_NEAR(building.GetGoodsSalesVales(), 0.0F, 0.001F);
}


TEST(Vic3WorldBuildingsBuildingImporterTests, ItemsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "building = building_iron_mine\n";
   input << "state = 119\n";
   input << "goods_sales = 44263.82079\n";
   input << "}";
   const Building building = BuildingImporter{}.ImportBuilding(input);

   EXPECT_EQ(building.GetType(), "building_iron_mine");
   EXPECT_EQ(building.GetStateNumber().value_or(0), 119);
   EXPECT_NEAR(building.GetGoodsSalesVales(), 44263.82079F, 0.001F);
}

}  // namespace vic3