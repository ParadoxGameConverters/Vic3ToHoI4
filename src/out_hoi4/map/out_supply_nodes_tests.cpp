#include <fstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/out_hoi4/map/out_supply_nodes.h"



namespace out
{

TEST(Outhoi4MapOutsupplynodesTests, ExceptionThrownOnBadPath)
{
   ASSERT_THROW(OutputSupplyNodes("OutHoI4Tests/ExceptionThrownOnBadPath", std::set<int>{}), std::runtime_error);
}


TEST(Outhoi4MapOutsupplynodesTests, SupplyNodesAreOutput)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/Outhoi4MapOutsupplynodesTests");
   commonItems::TryCreateFolder("output/Outhoi4MapOutsupplynodesTests/SupplyNodesAreOutput");
   commonItems::TryCreateFolder("output/Outhoi4MapOutsupplynodesTests/SupplyNodesAreOutput/map");

   OutputSupplyNodes("Outhoi4MapOutsupplynodesTests/SupplyNodesAreOutput", {1, 2, 3});

   std::ifstream file("output/Outhoi4MapOutsupplynodesTests/SupplyNodesAreOutput/map/supply_nodes.txt");
   const std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

   std::stringstream expected;
   expected << "1 1\n";
   expected << "1 2\n";
   expected << "1 3\n";
   EXPECT_EQ(content, expected.str());
}

}  // namespace out