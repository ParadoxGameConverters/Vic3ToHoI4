#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/military/battalion.h"
#include "src/mappers/unit/unit_mapper.h"
#include "src/mappers/unit/unit_mapper_importer.h"


namespace mappers
{

TEST(MappersUnitUnitMapperImporterTests, RulesCanBeImported)
{
   const UnitMapper unit_mapper = ImportUnitMapper("test_files/mappers/unit/unit_mappings.txt");
   const auto battalions =
       unit_mapper.MakeBattalions({"pm_trench_infantry", "pm_cavalry_scouts", "some_other_thing"}, 1);
   EXPECT_THAT(battalions,
       testing::UnorderedElementsAre(hoi4::Battalion("infantry", 50, 0.25), hoi4::Battalion("cavalry", 50, 0.2)));
}

}  // namespace mappers
