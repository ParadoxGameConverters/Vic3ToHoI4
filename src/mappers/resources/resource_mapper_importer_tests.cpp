#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/resources/resource_mapper.h"
#include "src/mappers/resources/resource_mapper_importer.h"

constexpr float kTolerance = 0.0001F;

namespace mappers
{

TEST(MappersResourcesResourceMapperImporterTests, RulesCanBeImported)
{
   const ResourceMapper mapper = ImportResourceMapper("test_files/configurables/resource_mappings.txt");
   std::vector<vic3::Building> buildings{
       {"building_iron_mine", 1, vic3::GoodsSalesValue{1.0F}, vic3::StaffingLevel{1.0F}, {}},
       {"building_oil_rig", 1, vic3::GoodsSalesValue{0.8F}, vic3::StaffingLevel{1.0F}, {}},
       {"building_coal_mine", 1, vic3::GoodsSalesValue{0.2F}, vic3::StaffingLevel{1.0F}, {}},
       {"building_steel_factory", 1, vic3::GoodsSalesValue{0.8F}, vic3::StaffingLevel{1.0F}, {}},
   };
   EXPECT_NEAR(mapper.CalculateScore("steel", buildings), 1.1F, kTolerance);
   EXPECT_NEAR(mapper.CalculateScore("oil", buildings), 0.48F, kTolerance);
   EXPECT_NEAR(mapper.CalculateScore("tungsten", buildings), 0.0F, kTolerance);
   EXPECT_NEAR(mapper.WorldTotal("steel"), 1000.0F, kTolerance);
   EXPECT_NEAR(mapper.WorldTotal("oil"), 250.0F, kTolerance);
   EXPECT_NEAR(mapper.WorldTotal("tungsten"), 0.0F, kTolerance);
}

}  // namespace mappers
