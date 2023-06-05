#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/states/hoi4_state.h"



namespace hoi4
{

TEST(Hoi4worldStatesHoi4state, HighestVictoryPointValueCanBeChanged)
{
   hoi4::State test_state(1, {.victory_points = {{1, 1}, {2, 2}}});
   test_state.SetHighestVictoryPointValue(42);

   EXPECT_THAT(test_state.GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(1, 1), testing::Pair(2, 42)));
}


TEST(Hoi4worldStatesHoi4state, HighestVictoryPointValueCanBeChangedForOneVictoryPoint)
{
   hoi4::State test_state(1, {.victory_points = {{1, 1}}});
   test_state.SetHighestVictoryPointValue(42);

   EXPECT_THAT(test_state.GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(1, 42)));
}


TEST(Hoi4worldStatesHoi4state, HighestVictoryPointValueLeavesEmptyVictoryPointsAlone)
{
   hoi4::State test_state(1, {});
   test_state.SetHighestVictoryPointValue(42);

   EXPECT_TRUE(test_state.GetVictoryPoints().empty());
}

}  // namespace hoi4