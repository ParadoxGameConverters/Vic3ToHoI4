#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/military/ship.h"
#include "src/hoi4_world/military/task_force_template.h"


namespace hoi4
{

TEST(Hoi4MilitaryTaskForceTemplateTests, TracksRequiredVariants)
{
   const TaskForceTemplate tmpl1({}, {Ship("Test", "test1", "test1", "legacy_test1", "Basic Ship")});
   const TaskForceTemplate tmpl2({}, {Ship("Test", "test2", "test2", "legacy_test2", "1936 Ship")});
   EXPECT_TRUE(tmpl1.AllVariantsActive({"Basic Ship", "test1", "legacy_test1"}));
   EXPECT_TRUE(tmpl2.AllVariantsActive({"1936 Ship", "test2", "legacy_test2"}));
   EXPECT_TRUE(tmpl1.AllVariantsActive({"Basic Ship", "Some Other Ship", "test1", "legacy_test1"}));
   EXPECT_TRUE(tmpl2.AllVariantsActive({"1936 Ship", "Some Other Ship", "test2", "legacy_test2"}));

   EXPECT_FALSE(tmpl1.AllVariantsActive({"1936 Ship", "Some Other Ship", "test2", "legacy_test2"}));
   EXPECT_FALSE(tmpl2.AllVariantsActive({"Basic Ship", "Some Other Ship", "test1", "legacy_test1"}));

   EXPECT_FALSE(tmpl1.AllVariantsActive({"Some Other Ship", "test1", "legacy_test1"}));
   EXPECT_FALSE(tmpl1.AllVariantsActive({"Basic Ship", "Some Other Ship", "legacy_test1"}));
   EXPECT_FALSE(tmpl1.AllVariantsActive({"Basic Ship", "Some Other Ship", "test1"}));

   EXPECT_FALSE(tmpl2.AllVariantsActive({"Some Other Ship", "test2", "legacy_test2"}));
   EXPECT_FALSE(tmpl2.AllVariantsActive({"1936 Ship", "Some Other Ship", "legacy_test2"}));
   EXPECT_FALSE(tmpl2.AllVariantsActive({"1936 Ship", "Some Other Ship", "test2"}));
}

TEST(Hoi4MilitaryTaskForceTemplateTests, AddsShipsAndPaysCosts)
{
   const TaskForceTemplate tmpl1({{"pm_basic_bigship", 10}, {"pm_basic_smallship", 10}},
       {
           Ship("Big Ship", "test1", "test1", "legacy_test1", "Basic Big Ship"),
           Ship("Small Ship", "test1", "test1", "legacy_test1", "Basic Small Ship"),
           Ship("Small Ship", "test1", "test1", "legacy_test1", "Basic Small Ship"),
       });
   const TaskForceTemplate tmpl2({{"pm_1936_bigship", 10}, {"pm_1936_smallship", 10}},
       {
           Ship("Big Ship", "test2", "test2", "legacy_test2", "1936 Big Ship"),
           Ship("Small Ship", "test2", "test2", "legacy_test2", "1936 Small Ship"),
           Ship("Small Ship", "test2", "test2", "legacy_test2", "1936 Small Ship"),
       });
   const TaskForceTemplate tmpl3({{"pm_humongous", 1}},
       {
           Ship("Humongous Ship", "test3", "test3", "legacy_test3", "1936 Humongous Ship"),
       });

   std::vector<Ship> ships;
   std::map<std::string, float> naval_pms{{"pm_basic_bigship", 25},
       {"pm_basic_smallship", 50},
       {"pm_1936_bigship", 21},
       {"pm_1936_smallship", 11}};
   std::map<std::string, int> nameCounts;
   tmpl3.AddShipsIfPossible(ships, nameCounts, naval_pms);
   tmpl2.AddShipsIfPossible(ships, nameCounts, naval_pms);
   tmpl1.AddShipsIfPossible(ships, nameCounts, naval_pms);

   EXPECT_THAT(ships,
       testing::ElementsAre(Ship("Big Ship 1", "test2", "test2", "legacy_test2", "1936 Big Ship"),
           Ship("Small Ship 1", "test2", "test2", "legacy_test2", "1936 Small Ship"),
           Ship("Small Ship 2", "test2", "test2", "legacy_test2", "1936 Small Ship"),
           Ship("Big Ship 2", "test1", "test1", "legacy_test1", "Basic Big Ship"),
           Ship("Small Ship 3", "test1", "test1", "legacy_test1", "Basic Small Ship"),
           Ship("Small Ship 4", "test1", "test1", "legacy_test1", "Basic Small Ship"),
           Ship("Big Ship 3", "test1", "test1", "legacy_test1", "Basic Big Ship"),
           Ship("Small Ship 5", "test1", "test1", "legacy_test1", "Basic Small Ship"),
           Ship("Small Ship 6", "test1", "test1", "legacy_test1", "Basic Small Ship")));
   EXPECT_THAT(naval_pms,
       testing::ContainerEq(std::map<std::string, float>{{"pm_basic_bigship", 5},
           {"pm_basic_smallship", 30},
           {"pm_1936_bigship", 11},
           {"pm_1936_smallship", 1},
           {"pm_humongous", 0}}));
}

}  // namespace hoi4
