#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/hoi4_world/military/convoy_distributor.h"



namespace hoi4
{

TEST(Hoi4MilitaryConvoyDistributorTests, EmptyWorldNoConvoys)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   ConvoyDistributor convoy(100, {{"pm_port", 1.0F}});
   convoy.CalculateStateWeights(source_world);
   EXPECT_EQ(convoy.ConvoysFromState(1), 0);
}

TEST(Hoi4MilitaryConvoyDistributorTests, HandlesUnstaffedPort)
{
   const std::map<int, vic3::State> states{
       {1, vic3::State({.id = 1})},
   };
   vic3::Buildings buildings(std::map<int, std::vector<vic3::Building>>{
       {1,
           {vic3::Building(vic3::kBuildingTypePort,
               1,
               vic3::GoodsSalesValue{0},
               vic3::StaffingLevel{0},
               std::vector<std::string>{"dummy", "pm_port_1"})}},
   });
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.states = states, .buildings = buildings});
   ConvoyDistributor convoy(100, {{"pm_port", 1.0F}});
   convoy.CalculateStateWeights(source_world);
   EXPECT_EQ(convoy.ConvoysFromState(1), 0);
}

TEST(Hoi4MilitaryConvoyDistributorTests, WeightByProductionMethod)
{
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
   ConvoyDistributor convoy(111, {{"pm_port_1", 1.0F}, {"pm_port_2", 10.0F}, {"pm_port_3", 100.0F}});
   convoy.CalculateStateWeights(source_world);
   EXPECT_EQ(convoy.ConvoysFromState(1), 1);
   EXPECT_EQ(convoy.ConvoysFromState(2), 10);
   EXPECT_EQ(convoy.ConvoysFromState(3), 100);
}

TEST(Hoi4MilitaryConvoyDistributorTests, WeightByStaffingLevel)
{
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
               vic3::StaffingLevel{10},
               std::vector<std::string>{"pm_port_1"})}},
       {3,
           {vic3::Building(vic3::kBuildingTypePort,
               3,
               vic3::GoodsSalesValue{0},
               vic3::StaffingLevel{100},
               std::vector<std::string>{"pm_port_1"})}},
   });
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.states = states, .buildings = buildings});
   ConvoyDistributor convoy(111, {{"pm_port_1", 1.0F}, {"pm_port_2", 10.0F}, {"pm_port_3", 100.0F}});
   convoy.CalculateStateWeights(source_world);
   EXPECT_EQ(convoy.ConvoysFromState(1), 1);
   EXPECT_EQ(convoy.ConvoysFromState(2), 10);
   EXPECT_EQ(convoy.ConvoysFromState(3), 100);
}


TEST(Hoi4MilitaryConvoyDistributorTests, UnknownStateNoConvoys)
{
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
               vic3::StaffingLevel{10},
               std::vector<std::string>{"dummy", "pm_port_1"})}},
   });
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.states = states, .buildings = buildings});
   ConvoyDistributor convoy(111, {{"pm_port_1", 1.0F}, {"pm_port_2", 10.0F}, {"pm_port_3", 100.0F}});
   convoy.CalculateStateWeights(source_world);
   EXPECT_EQ(convoy.ConvoysFromState(5), 0);
}



}  // namespace hoi4
