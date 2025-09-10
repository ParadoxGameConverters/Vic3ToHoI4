#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/mappers/resources/resource_mapper.h"

constexpr float kTolerance = 0.0001F;

namespace mappers
{

TEST(MappersResourcesResourceMapperTests, EmptyMapperNoInfo)
{
   std::map<std::string, ResourceScore> empty{};
   ResourceMapper mapper(empty);

   std::vector<vic3::Building> buildings{
       {"building_iron_mine", 1, vic3::GoodsSalesValue{1.0F}, vic3::StaffingLevel{1.0F}, {}}};
   EXPECT_NEAR(mapper.CalculateScore("some_resource", buildings), 0.0F, kTolerance);
   EXPECT_NEAR(mapper.WorldTotal("some_resource"), 0.0F, kTolerance);
}

TEST(MappersResourcesResourceMapperTests, ScoresAreCalculated)
{
   std::map<std::string, ResourceScore> steel{
       {"steel", {.base = {{"building_iron_mine", 1.0F}}, .bonus_buildings = {}, .bonus = 0.0F, .total = 1.0F}},
   };
   ResourceMapper mapper(steel);

   std::vector<vic3::Building> buildings{
       {"building_iron_mine", 1, vic3::GoodsSalesValue{1.0F}, vic3::StaffingLevel{1.0F}, {}}};
   EXPECT_NEAR(mapper.CalculateScore("steel", buildings), 1.0F, kTolerance);
   EXPECT_NEAR(mapper.CalculateScore("oil", buildings), 0.0F, kTolerance);
   EXPECT_NEAR(mapper.WorldTotal("steel"), 1.0F, kTolerance);
   EXPECT_NEAR(mapper.WorldTotal("oil"), 0.0F, kTolerance);
}

TEST(MappersResourcesResourceMapperTests, BonusIsAppliedAndCapped)
{
   std::map<std::string, ResourceScore> scores{
       {
           "steel",
           {
               .base = {{"building_iron_mine", 1.0F}},
               .bonus_buildings = {"building_coal_mine"},
               .bonus = 0.1F,
               .total = 0.0F,
           },
       },
       {
           "oil",
           {
               .base = {{"building_oil_rig", 1.0F}},
               .bonus_buildings = {"building_coal_mine"},
               .bonus = 0.1F,
               .total = 0.0F,
           },
       },
   };
   ResourceMapper mapper(scores);

   std::vector<vic3::Building> buildings{
       {"building_iron_mine", 1, vic3::GoodsSalesValue{1.0F}, vic3::StaffingLevel{1.0F}, {}},
       {"building_oil_rig", 1, vic3::GoodsSalesValue{0.1F}, vic3::StaffingLevel{1.0F}, {}},
       {
           "building_coal_mine",
           1,
           vic3::GoodsSalesValue{0.5F},
           vic3::StaffingLevel{1.0F},
           {},
       },
   };
   EXPECT_NEAR(mapper.CalculateScore("steel", buildings), 1.05F, kTolerance);
   EXPECT_NEAR(mapper.CalculateScore("oil", buildings), 0.11F, kTolerance);
   EXPECT_NEAR(mapper.CalculateScore("tungsten", buildings), 0.0F, kTolerance);
}

}  // namespace mappers
