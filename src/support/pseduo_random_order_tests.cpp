#include <external/Fronter/external/commonItems/external/json/single_include/nlohmann/json.hpp>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/support/pseduo_random_order.h"

TEST(SupportPseduoRandomPseduoRandomOrderTests, EmptyContainerMeansNoOutput)
{
   constexpr std::vector<int> empty;
   constexpr int seed = 0;
   EXPECT_EQ(PseduoRandomizeOrder(seed, empty), empty);
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, UnaryContainerOutputOnlyIndex)
{
   const std::vector unary{47};
   constexpr int seed = 0;
   EXPECT_EQ(PseduoRandomizeOrder(seed, unary), std::vector{0});
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, BinaryContainerOutputsBothIndicies)
{
   const std::vector binary{47, 4};
   constexpr int seed = 0;
   EXPECT_EQ(PseduoRandomizeOrder(seed, binary), std::vector({0, 1}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, TrinaryContainerOutputsAllIndicies)
{
   const std::vector ternary{47, 4, 78};
   constexpr int seed = 0;
   EXPECT_EQ(PseduoRandomizeOrder(0, ternary), std::vector({0, 1, 2}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, OddBitsScrambleIndicies)
{
   const std::vector ternary{47, 4, 78};
   constexpr int seed = 1;
   EXPECT_EQ(PseduoRandomizeOrder(seed, ternary), std::vector({0, 2, 1}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, EvenBitsShiftIndicies)
{
   const std::vector ternary{47, 4, 78};
   constexpr int seed = 2;
   EXPECT_EQ(PseduoRandomizeOrder(seed, ternary), std::vector({1, 2, 0}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, EvenAndOddBitsShiftAndScrambleIndicies)
{
   const std::vector ternary{47, 4, 78};
   constexpr int seed = 3;
   EXPECT_EQ(PseduoRandomizeOrder(seed, ternary), std::vector({2, 1, 0}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, AllPermutationsUnderThreeAreHit)
{
   const std::vector binary{47, 4};
   const std::vector ternary{47, 4, 78};

   constexpr int seed_bi_shift = 2;
   constexpr int seed_ter_double_shift = 10;
   constexpr int seed_ter_scramble_double_shift = 11;
   EXPECT_EQ(PseduoRandomizeOrder(seed_bi_shift, binary), std::vector({1, 0}));
   EXPECT_EQ(PseduoRandomizeOrder(seed_ter_double_shift, ternary), std::vector({2, 0, 1}));
   EXPECT_EQ(PseduoRandomizeOrder(seed_ter_scramble_double_shift, ternary), std::vector({1, 0, 2}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, IntMaxIsSafe)
{
   const std::vector ternary{47, 4, 78};

   constexpr int seed = INT_MAX;
   EXPECT_EQ(PseduoRandomizeOrder(seed, ternary), std::vector({0, 2, 1}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, IntMinIsSafe)
{
   const std::vector ternary{47, 4, 78};

   constexpr int seed = INT_MIN;
   EXPECT_EQ(PseduoRandomizeOrder(seed, ternary), std::vector({0, 1, 2}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, ArbitraryNegativeIsSafe)
{
   const std::vector ternary{47, 4, 78};

   constexpr int seed = -3156357;
   EXPECT_EQ(PseduoRandomizeOrder(seed, ternary), std::vector({1, 0, 2}));
}

TEST(SupportPseduoRandomPseduoRandomOrderTests, ArbitraryLargerValuesAreSafe)
{
   const std::vector ternary{47, 4, 78};
   const std::vector septenary{47, 4, 78, 41, 526, 7, 9};
   const std::vector duodenary{47, 4, 78, 41, 526, 7, 9, 1, 51, 3, 10, 12};

   // Odd bits = 17
   // Even bits = 40 / 2 = 20
   constexpr int seed = 57;
   EXPECT_EQ(PseduoRandomizeOrder(seed, ternary), std::vector({1, 0, 2}));
   EXPECT_EQ(PseduoRandomizeOrder(seed, septenary), std::vector({1, 0, 6, 5, 4, 3, 2}));
   EXPECT_EQ(PseduoRandomizeOrder(seed, duodenary), std::vector({4, 9, 2, 7, 0, 5, 10, 3, 8, 1, 6, 11}));
}