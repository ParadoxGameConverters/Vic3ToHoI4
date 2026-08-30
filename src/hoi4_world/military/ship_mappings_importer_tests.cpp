#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/military/ship.h"
#include "src/hoi4_world/military/ship_mappings_importer.hpp"



namespace hoi4
{

TEST(Hoi4worldMilitaryShipmappingsimporterTests, RulesCanBeImported)  // NOLINT(cert-err58-cpp)
{
   const std::map<std::string, Ship> mappings = ImportShipMappings("test_files/configurables/ship_mappings.txt");

   EXPECT_THAT(mappings,
       testing::UnorderedElementsAre(testing::Pair("ship_type_test",
                                         Ship{
                                             .name = "test name",
                                             .definition = "test definition",
                                             .equipment = "test equipment",
                                             .legacy_equipment = "test legacy equipment",
                                             .version = "test version",
                                         }),
           testing::Pair("ship_type_test2",
               Ship{
                   .name = "test name2",
                   .definition = "test definition2",
                   .equipment = "test equipment2",
                   .legacy_equipment = "test legacy equipment2",
                   .version = "test version2",
               })));
}

}  // namespace hoi4
