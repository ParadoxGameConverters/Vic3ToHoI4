#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/unit/unit_mapper.h"



namespace mappers
{

TEST(MappersUnitUnitMapperTests, NoRulesMeansNoUnits)
{
   TemplateMap templates;
   const UnitMapper unit_mapper(templates);
   EXPECT_TRUE(unit_mapper.MakeBattalions({"pm_trench_infantry", "pm_cavalry_scouts"}, 10).empty());
}


TEST(MappersUnitUnitMapperTests, BattalionsAreMade)
{
   TemplateMap templates{
       {"trench_infantry", BattalionTemplate(50, {{"infantry", 0.25}})},
       {"cavalry_scouts", BattalionTemplate(0, {{"cavalry", 0.20}})},
       {"field_hospitals", BattalionTemplate(10, {})},
   };
   const UnitMapper unit_mapper(templates);
   EXPECT_THAT(unit_mapper.MakeBattalions({"trench_infantry", "cavalry_scouts", "field_hospitals"}, 2),
       testing::UnorderedElementsAre(hoi4::Battalion("infantry", 60, 0.50), hoi4::Battalion("cavalry", 60, 0.40)));
}


}  // namespace mappers
