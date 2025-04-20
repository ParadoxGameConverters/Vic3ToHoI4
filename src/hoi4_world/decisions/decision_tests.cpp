#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/decisions/decision.h"



namespace hoi4
{

TEST(Hoi4worldDecisionsDecisionTests, EqualDecisionsAreEqual)
{
   const Decision decision{.name = "decision_name"};
   const Decision second_decision{.name = "decision_name"};

   ASSERT_TRUE(decision == second_decision);
}


TEST(Hoi4worldDecisionsDecisionTests, UnequalDecisionsAreUnequal)
{
   const Decision decision{.name = "decision_name"};
   const Decision second_decision{.name = "decision_name_two"};

   ASSERT_FALSE(decision == second_decision);
}

}  // namespace hoi4