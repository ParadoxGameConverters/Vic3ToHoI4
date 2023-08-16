#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/unit/unit_mapper.h"



namespace mappers
{

TEST(MappersUnitUnitMapperTests, NoRulesMeansNoPoints)
{
   const UnitMapper unit_mapper({});
   EXPECT_TRUE(unit_mapper.CalculateUnitPoints({{"test", 0.25F}}).empty());
}


TEST(MappersUnitUnitMapperTests, UnitPointsAreCalculated)
{
   const UnitMapper unit_mapper(
       {
           {"trench_infantry",
               {
                   {"infantry", 0.25F},
                   {"artillery", 0.1F},
               }},
           {"skirmish_infantry",
               {
                   {"infantry", 0.1F},
                   {"cavalry", 0.1F},
               }},
       });

   EXPECT_THAT(unit_mapper.CalculateUnitPoints({{"trench_infantry", 3.0F}, {"skirmish_infantry", 6.0F}}),
       testing::UnorderedElementsAre(testing::Pair("infantry", 1.35F),
           testing::Pair("artillery", 0.3F),
           testing::Pair("cavalry", 0.6F)));
}


}  // namespace mappers
