#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/military/task_force_template.h"
#include "src/hoi4_world/military/task_force_templates_builder.h"



namespace hoi4
{

TEST(Hoi4worldMilitaryTaskForceTemplatesBuilderTests, IgnoresBadTaskForceTemplates)
{
   const std::vector<TaskForceTemplate> task_forces =
       ImportTaskForceTemplates("test_files/hoi4_world/military/IgnoresBadTaskForceTemplates.txt");
   EXPECT_TRUE(task_forces.empty());
}

TEST(Hoi4worldMilitaryTaskForceTemplatesBuilderTests, ImportsTaskForceTemplates)
{
   const std::vector<TaskForceTemplate> task_forces =
       ImportTaskForceTemplates("test_files/hoi4_world/military/ImportsTaskForceTemplates.txt");
   EXPECT_EQ(3, task_forces.size());

   std::vector<Ship> ships;
   std::map<std::string, float> naval_pms{{"pm_basic_bigship", 25.0F},
       {"pm_basic_smallship", 50.0F},
       {"pm_1936_bigship", 21.0F},
       {"pm_1936_smallship", 11.0F}};
   std::map<std::string, int> name_counts;
   for (const auto& tmpl: task_forces)
   {
      tmpl.AddShipsIfPossible(ships, name_counts, naval_pms);
   }

   EXPECT_THAT(ships,
       testing::ElementsAre(Ship(ShipOptions{ "Big Ship 1", "test1", "test1", "legacy_test1", "Basic Big Ship" }),
           Ship(ShipOptions{ "Small Ship 1", "test1", "test1", "legacy_test1", "Basic Small Ship"}),
           Ship(ShipOptions{ "Small Ship 2", "test1", "test1", "legacy_test1", "Basic Small Ship"}),
           Ship(ShipOptions{ "Big Ship 2", "test1", "test1", "legacy_test1", "Basic Big Ship"}),
           Ship(ShipOptions{ "Small Ship 3", "test1", "test1", "legacy_test1", "Basic Small Ship"}),
           Ship(ShipOptions{ "Small Ship 4", "test1", "test1", "legacy_test1", "Basic Small Ship"}),
           Ship(ShipOptions{ "Big Ship 3", "test2", "test2", "legacy_test2", "1936 Big Ship"}),
           Ship(ShipOptions{ "Small Ship 5", "test2", "test2", "legacy_test2", "1936 Small Ship"}),
           Ship(ShipOptions{ "Small Ship 6", "test2", "test2", "legacy_test2", "1936 Small Ship"})));
   EXPECT_THAT(naval_pms,
       testing::ContainerEq(std::map<std::string, float>{{"pm_basic_bigship", 5.0F},
           {"pm_basic_smallship", 30.0F},
           {"pm_1936_bigship", 11.0F},
           {"pm_1936_smallship", 1.0F},
           {"pm_humongous", 0.0F}}));
}


}  // namespace hoi4
