#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/strategic_region.h"



namespace hoi4
{

TEST(Hoi4WorldMapStrategicRegionTests, NewProvincesDefaultsToEmpty)
{
   const StrategicRegion strategic_region("", 1, "", {}, {}, std::nullopt, "");

   EXPECT_TRUE(strategic_region.GetNewProvinces().empty());
}


TEST(Hoi4WorldMapStrategicRegionTests, NewProvincesCanBeAdded)
{
   StrategicRegion strategic_region("", 1, "", {}, {}, std::nullopt, "");
   strategic_region.AddNewProvince(144);
   strategic_region.AddNewProvince(169);

   EXPECT_THAT(strategic_region.GetNewProvinces(), testing::UnorderedElementsAre(144, 169));
}

}  // namespace hoi4