#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <vector>

#include "ship.h"
#include "src/hoi4_world/military/ship_converter.hpp"



namespace hoi4
{

TEST(ShipConverterTests, CanConvertShips)
{
   const ShipConverter ship_converter({
       {
           "vic3_ship_one",
           hoi4::Ship{
               .name = "Hoi4 Ship One",
               .definition = "hoi4_ship_one",
               .equipment = "ship_hull_one_1",
               .legacy_equipment = "one_2",
               .version = "1936 One",
           },
       },
       {
           "vic3_ship_two",
           hoi4::Ship{
               .name = "Hoi4 Ship Two",
               .definition = "hoi4_ship_two",
               .equipment = "ship_hull_two_1",
               .legacy_equipment = "two_2",
               .version = "1936 Two",
           },
       },
   });

   const std::vector<std::string> vic3_ships{"vic3_ship_one", "vic3_ship_one", "vic3_ship_two"};
   std::map<std::string, int> name_counts;
   const std::vector<Ship> ships = ship_converter.ConvertShips(vic3_ships, name_counts);

   const std::vector expected_ships{
       hoi4::Ship{
           .name = "Hoi4 Ship One 1",
           .definition = "hoi4_ship_one",
           .equipment = "ship_hull_one_1",
           .legacy_equipment = "one_2",
           .version = "1936 One",
       },
       hoi4::Ship{
           .name = "Hoi4 Ship One 2",
           .definition = "hoi4_ship_one",
           .equipment = "ship_hull_one_1",
           .legacy_equipment = "one_2",
           .version = "1936 One",
       },
       hoi4::Ship{
           .name = "Hoi4 Ship Two 1",
           .definition = "hoi4_ship_two",
           .equipment = "ship_hull_two_1",
           .legacy_equipment = "two_2",
           .version = "1936 Two",
       },
   };
   EXPECT_EQ(ships, expected_ships);
}


TEST(ShipConverterTests, ShipsWithMissingDefinitionsAreSkipped)
{
   const ShipConverter ship_converter({});

   const std::vector<std::string> vic3_ships{"vic3_ship_one"};
   std::map<std::string, int> name_counts;
   const std::vector<Ship> ships = ship_converter.ConvertShips(vic3_ships, name_counts);

   EXPECT_TRUE(ships.empty());
}

}  // namespace hoi4
