#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/mappers/infrastructure/infrastructure_mapper.h"


namespace mappers
{
TEST(MappersInfrastructureInfrastructureMapperTests, FindRootWorks)
{
   // linear function. zero should be -20/3
   const auto func = [](float input) {
      return 0.6F * input + 4;
   };

   EXPECT_FLOAT_EQ((find_root<float, float>(
                       func,
                       0.0F,
                       [](float f) {
                          return -1.0F;
                       },
                       5)),
       -20.0F / 3.0F);
}

TEST(MappersInfrastructureInfrastructureMapperTests, Default1Infrastructure)
{
   InfrastructureMapper infrastructure_mapper(0, 0);
   EXPECT_EQ(infrastructure_mapper.Map(100), 1);
}

TEST(MappersInfrastructureInfrastructureMapperTests, Minimum1Infrastructure)
{
   InfrastructureMapper infrastructure_mapper(1, 0);
   EXPECT_EQ(infrastructure_mapper.Map(0), 1);
}

TEST(MappersInfrastructureInfrastructureMapperTests, Maximum5Infrastructure)
{
   InfrastructureMapper infrastructure_mapper(1, 0);
   EXPECT_EQ(infrastructure_mapper.Map(1000), 5);
}

TEST(MappersInfrastructureInfrastructureMapperTests, RoundingWorks)
{
   InfrastructureMapper infrastructure_mapper(1, 0);
   EXPECT_EQ(infrastructure_mapper.Map(0.5), 2);
   EXPECT_EQ(infrastructure_mapper.Map(0.49), 1);
}

TEST(MappersInfrastructureInfrastructureMapperTests, FudgeFactorWorksPositive)
{
   InfrastructureMapper infrastructure_mapper(1, 0.5F);
   EXPECT_EQ(infrastructure_mapper.Map(0.4F), 2);
   EXPECT_EQ(infrastructure_mapper.Map(5), 5);
}

TEST(MappersInfrastructureInfrastructureMapperTests, FudgeFactorWorksNegative)
{
   InfrastructureMapper infrastructure_mapper(1, -0.5F);
   EXPECT_EQ(infrastructure_mapper.Map(0), 1);
   EXPECT_EQ(infrastructure_mapper.Map(2.9F), 3);
}


}  // namespace mappers