#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/military/battalion.h"



namespace hoi4
{

TEST(Hoi4MilitaryBattalionTests, ConstructorValuesPassedThrough)
{
   const Battalion battalion("test", 1, 1.0F);
   EXPECT_EQ(battalion.GetType(), "test");
   EXPECT_EQ(battalion.GetEquipmentScale(), 1);
   EXPECT_NEAR(battalion.GetStrength(), 1.0F, 0.001F);
   EXPECT_FALSE(battalion.GetLocation().has_value());
}

TEST(Hoi4MilitaryBattalionTests, CanSetStrength)
{
   Battalion battalion("test", 1, 1.0F);
   battalion.AddStrength(1.0F);
   EXPECT_NEAR(battalion.GetStrength(), 2.0F, 0.001F);
}

TEST(Hoi4MilitaryBattalionTests, CanSetLocation)
{
   Battalion battalion("test", 1, 1.0F);
   battalion.SetLocation(11666);
   EXPECT_TRUE(battalion.GetLocation().has_value());
   EXPECT_EQ(battalion.GetLocation().value(), 11666);
}



}  // namespace hoi4
