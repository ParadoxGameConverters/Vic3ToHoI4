#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/mappers/infrastructure/infrastructure_mapper.h"


namespace mappers
{
TEST(MappersInfrastructureInfrastructureMapperTests, Default1Infrastructure)
{
   InfrastructureMapper infrastructure_mapper(0);
   EXPECT_EQ(infrastructure_mapper.map(100), 1);
}

TEST(MappersInfrastructureInfrastructureMapperTests, Minimum1Infrastructure)
{
   InfrastructureMapper infrastructure_mapper(1);
   EXPECT_EQ(infrastructure_mapper.map(0), 1);
}

TEST(MappersInfrastructureInfrastructureMapperTests, Maximum5Infrastructure)
{
   InfrastructureMapper infrastructure_mapper(1);
   EXPECT_EQ(infrastructure_mapper.map(1000), 5);
}

TEST(MappersInfrastructureInfrastructureMapperTests, RoundingWorks)
{
   InfrastructureMapper infrastructure_mapper(1);
   EXPECT_EQ(infrastructure_mapper.map(0.5), 2);
   EXPECT_EQ(infrastructure_mapper.map(0.49), 1);
}

TEST(MappersInfrastructureInfrastructureMapperTests, FudgeFactorWorksPositive)
{
   InfrastructureMapper infrastructure_mapper(1,0.5F);
   EXPECT_EQ(infrastructure_mapper.map(0.4F), 2);
   EXPECT_EQ(infrastructure_mapper.map(5), 5);
}

TEST(MappersInfrastructureInfrastructureMapperTests, FudgeFactorWorksNegative)
{
   InfrastructureMapper infrastructure_mapper(1, -0.5F);
   EXPECT_EQ(infrastructure_mapper.map(0), 1);
   EXPECT_EQ(infrastructure_mapper.map(2.9F), 3);
}


}  // namespace mappers