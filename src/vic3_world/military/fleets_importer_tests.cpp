#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <optional>
#include <sstream>

#include "src/vic3_world/military/fleets_importer.hpp"



namespace vic3
{

TEST(Vic3worldMilitaryFleetsImporter, ShipsDefaultToEmpty)
{
   std::stringstream input;

   const auto ships = ImportShips(input);
   EXPECT_TRUE(ships.empty());
}


TEST(Vic3worldMilitaryFleetsImporter, ShipsCanBeImported)
{
   std::stringstream input;
   input << "= {\n";
   input << "\tdatabase={\n";
   input << "\t\t1={\n";
   input << "\t\t\tfleet=10\n";
   input << "\t\t\tversion=1\n";
   input << "\t\t}\n";
   input << "\t\t2={\n";
   input << "\t\t\tfleet=10\n";
   input << "\t\t\tversion=1\n";
   input << "\t\t}\n";
   input << "\t\t3={\n";
   input << "\t\t\tfleet=30\n";
   input << "\t\t\tversion=2\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "}";


   const auto ships = ImportShips(input);
   EXPECT_THAT(ships,
       testing::UnorderedElementsAre(testing::Pair(10, std::vector<int64_t>{1, 1}),
           testing::Pair(30, std::vector<int64_t>{2})));
}


TEST(Vic3worldMilitaryFleetsImporter, MissingFleetMeansNoShip)
{
   std::stringstream input;
   input << "= {\n";
   input << "\tdatabase={\n";
   input << "\t\t1={\n";
   input << "\t\t\tversion=1\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "}";


   const auto ships = ImportShips(input);
   EXPECT_TRUE(ships.empty());
}


TEST(Vic3worldMilitaryFleetsImporter, MissingTypeMeansNoShip)
{
   std::stringstream input;
   input << "= {\n";
   input << "\tdatabase={\n";
   input << "\t\t1={\n";
   input << "\t\t\tfleet=10\n";
   input << "\t\t}\n";
   input << "\t}\n";
   input << "}";


   const auto ships = ImportShips(input);
   EXPECT_TRUE(ships.empty());
}

}  // namespace vic3