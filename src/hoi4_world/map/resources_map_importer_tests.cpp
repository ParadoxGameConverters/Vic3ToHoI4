#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/resources_map.h"
#include "src/hoi4_world/map/resources_map_importer.h"



namespace hoi4
{

TEST(Hoi4worldMapResourcesmapImporter, MissingFileThrowsException)
{
   const ResourcesMap resources_map = ImportResources("missing_file.txt");

   EXPECT_THAT(resources_map, testing::UnorderedElementsAre());
}


TEST(Hoi4worldMapResourcesmapImporter, ResourceMapCanBeImported)
{
   const ResourcesMap resources_map =
       ImportResources("test_files/hoi4_world/map/resources_map_importer/resource_map_can_be_imported.txt");

   EXPECT_THAT(resources_map,
       testing::UnorderedElementsAre(testing::Pair(10, hoi4::Resources{{"aluminium", 2}, {"oil", 2}, {"steel", 1}}),
           testing::Pair(20, hoi4::Resources{})));
}


}  // namespace hoi4