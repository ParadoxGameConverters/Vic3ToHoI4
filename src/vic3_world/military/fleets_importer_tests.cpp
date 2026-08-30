#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <optional>
#include <sstream>

#include "src/vic3_world/military/fleets_importer.hpp"



namespace vic3
{

TEST(Vic3worldMilitaryFleetsImporter, ShipsDefaultToEmpty)  // NOLINT(cert-err58-cpp)
{
   std::stringstream input;

   const auto ships = ImportShips(input);
   EXPECT_TRUE(ships.empty());
}


TEST(Vic3worldMilitaryFleetsImporter, ShipsCanBeImported)  // NOLINT(cert-err58-cpp)
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


TEST(Vic3worldMilitaryFleetsImporter, MissingFleetMeansNoShip)  // NOLINT(cert-err58-cpp)
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


TEST(Vic3worldMilitaryFleetsImporter, MissingTypeMeansNoShip)  // NOLINT(cert-err58-cpp)
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


TEST(Vic3worldMilitaryFleetsImporter, FleetsCanHaveShipTypesAdded)  // NOLINT(cert-err58-cpp)
{
   const std::map<int64_t, std::vector<std::string>> fleets_with_types =
       AddShipTypes({{1, "type_one"}, {2, "type_two"}}, {{1, {1, 1, 2}}, {2, {3, 2, 1}}});
   EXPECT_THAT(fleets_with_types,
       testing::UnorderedElementsAre(testing::Pair(1, std::vector<std::string>{"type_one", "type_one", "type_two"}),
           testing::Pair(2, std::vector<std::string>{"type_two", "type_one"})));
}

}  // namespace vic3