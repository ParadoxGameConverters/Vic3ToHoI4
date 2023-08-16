#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/unit/unit_mapper.h"
#include "src/mappers/unit/unit_mapper_importer.h"



namespace mappers
{

TEST(MappersUnitUnitMapperImporterTests, RulesCanBeImported)
{
   const UnitMapper unit_mapper = ImportUnitMapper("test_files/mappers/unit/unit_mappings.txt");

   const auto unit_points = unit_mapper.CalculateUnitPoints({
       {"pm_trench_infantry", 2.0F},
       {"pm_cavalry_scouts", 1.0F},
       {"some_other_thing", 11.321F},
   });
   EXPECT_NEAR(unit_points.at("infantry"), 0.5F, 0.001F);
   EXPECT_NEAR(unit_points.at("cavalry"), 0.2F, 0.001F);
}

}  // namespace mappers
