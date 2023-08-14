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
   EXPECT_NEAR(building.GetStaffingLevel(), 0.0F, 0.001F);
   EXPECT_TRUE(building.GetProductionMethods().empty());
}


TEST(Vic3WorldBuildingsBuildingImporterTests, ItemsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "building = building_iron_mine\n";
   input << "state = 119\n";
   input << "goods_sales = 44263.82079\n";
   input << "staffing = 2.10803\n";
   input << "production_methods = { \"pm_rail_transport_mine\" \"pm_steam_donkey_mine\" }\n";
   input << "}";
   const Building building = BuildingImporter{}.ImportBuilding(input);

   EXPECT_EQ(building.GetType(), "building_iron_mine");
   EXPECT_EQ(building.GetStateNumber().value_or(0), 119);
   EXPECT_NEAR(building.GetGoodsSalesVales(), 44263.82079F, 0.001F);
   EXPECT_NEAR(building.GetStaffingLevel(), 2.10803F, 0.001F);
   EXPECT_THAT(building.GetProductionMethods(), testing::ElementsAre("pm_rail_transport_mine", "pm_steam_donkey_mine"));
}

}  // namespace vic3
