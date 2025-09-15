#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/hoi4_world/military/convoy_distributor.h"
#include "src/hoi4_world/military/convoy_distributor_builder.h"
#include "src/vic3_world/world/vic3_world.h"


namespace hoi4
{

TEST(Hoi4worldMilitaryConvoyDistributorBuilderTests, CanBuildConvoyDistributor)
{
   auto distributor = BuildConvoyDistributor("test_files/configurables/convoy_config.txt");
   const std::map<int, vic3::State> states{
       {1, vic3::State({.id = 1})},
       {2, vic3::State({.id = 2})},
       {3, vic3::State({.id = 3})},
   };
   vic3::Buildings buildings(std::map<int, std::vector<vic3::Building>>{
       {1,
           {vic3::Building(vic3::kBuildingTypePort,
               1,
               vic3::GoodsSalesValue{0},
               vic3::StaffingLevel{1},
               std::vector<std::string>{"dummy", "pm_port_1"})}},
       {2,
           {vic3::Building(vic3::kBuildingTypePort,
               2,
               vic3::GoodsSalesValue{0},
               vic3::StaffingLevel{1},
               std::vector<std::string>{"pm_port_2"})}},
       {3,
           {vic3::Building(vic3::kBuildingTypePort,
               3,
               vic3::GoodsSalesValue{0},
               vic3::StaffingLevel{1},
               std::vector<std::string>{"pm_port_3"})}},
   });
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.states = states, .buildings = buildings});
   distributor.CalculateStateWeights(source_world);
   EXPECT_EQ(distributor.ConvoysFromState(1), 1);
   EXPECT_EQ(distributor.ConvoysFromState(2), 10);
   EXPECT_EQ(distributor.ConvoysFromState(3), 100);
}

}  // namespace hoi4
