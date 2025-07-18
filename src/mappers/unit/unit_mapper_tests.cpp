#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

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
       {"trench_infantry", BattalionTemplate(50, {{"infantry", 0.25F}})},
       {"cavalry_scouts", BattalionTemplate(0, {{"cavalry", 0.20F}})},
       {"field_hospitals", BattalionTemplate(10, {})},
   };
   const UnitMapper unit_mapper(templates);
   EXPECT_THAT(unit_mapper.MakeBattalions({"trench_infantry", "cavalry_scouts", "field_hospitals"}, 2),
       testing::UnorderedElementsAre(hoi4::Battalion("infantry", 60, 0.50F), hoi4::Battalion("cavalry", 60, 0.40F)));
}


TEST(MappersUnitUnitMapperTests, BattalionsAreMadeFromFormations)
{
   TemplateMap templates{
       {"trench_infantry", BattalionTemplate(50, {{"infantry", 0.25F}})},
       {"cavalry_scouts", BattalionTemplate(0, {{"cavalry", 0.20F}})},
       {"field_hospitals", BattalionTemplate(10, {})},
   };
   const UnitMapper unit_mapper(templates);
   EXPECT_THAT(unit_mapper.MakeBattalions(vic3::MilitaryFormation{
                   .units = {{"trench_infantry", 2}},
                   .combat_units = {{.current_manpower = 2000, .type = "cavalry_scouts"},
                       {.current_manpower = 2000, .type = "field_hospitals"}},
               }),
       testing::UnorderedElementsAre(hoi4::Battalion("infantry", 60, 0.50F), hoi4::Battalion("cavalry", 60, 0.40F)));
}


}  // namespace mappers
