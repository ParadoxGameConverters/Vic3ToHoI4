#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/map/strategic_region.h"



namespace hoi4
{

TEST(Hoi4WorldMapStrategicRegionTests, NewProvincesDefaultsToEmpty)
{
   const StrategicRegion strategic_region({.id = 1});

   EXPECT_TRUE(strategic_region.GetNewProvinces().empty());
}


TEST(Hoi4WorldMapStrategicRegionTests, NewProvincesCanBeAdded)
{
   StrategicRegion strategic_region({.id = 1});
   strategic_region.AddNewProvince(144);
   strategic_region.AddNewProvince(169);

   EXPECT_THAT(strategic_region.GetNewProvinces(), testing::UnorderedElementsAre(144, 169));
}

}  // namespace hoi4