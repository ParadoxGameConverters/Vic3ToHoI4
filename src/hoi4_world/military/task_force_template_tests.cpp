#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/hoi4_world/military/ship.h"
#include "src/hoi4_world/military/task_force_template.h"


namespace hoi4
{

TEST(Hoi4MilitaryTaskForceTemplateTests, TracksRequiredVariants)
{
   const TaskForceTemplate tmpl1({},
       {Ship(ShipOptions{
           .name = "Test",
           .definition = "test1",
           .equipment = "test1",
           .legacy_equipment = "legacy_test1",
           .version = "Basic Ship",
       })});
   const TaskForceTemplate tmpl2({},
       {Ship(ShipOptions{
           .name = "Test",
           .definition = "test2",
           .equipment = "test2",
           .legacy_equipment = "legacy_test2",
           .version = "1936 Ship",
       })});
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
   const TaskForceTemplate tmpl1({{"pm_basic_bigship", 10.0F}, {"pm_basic_smallship", 10.0F}},
       {
           Ship(ShipOptions{
               .name = "Big Ship",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Big Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Small Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Small Ship",
           }),
       });
   const TaskForceTemplate tmpl2({{"pm_1936_bigship", 10.0F}, {"pm_1936_smallship", 10.0F}},
       {
           Ship(ShipOptions{
               .name = "Big Ship",
               .definition = "test2",
               .equipment = "test2",
               .legacy_equipment = "legacy_test2",
               .version = "1936 Big Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship",
               .definition = "test2",
               .equipment = "test2",
               .legacy_equipment = "legacy_test2",
               .version = "1936 Small Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship",
               .definition = "test2",
               .equipment = "test2",
               .legacy_equipment = "legacy_test2",
               .version = "1936 Small Ship",
           }),
       });
   const TaskForceTemplate tmpl3({{"pm_humongous", 1.0F}},
       {
           Ship(ShipOptions{
               .name = "Humongous Ship",
               .definition = "test3",
               .equipment = "test3",
               .legacy_equipment = "legacy_test3",
               .version = "1936 Humongous Ship",
           }),
       });

   std::vector<Ship> ships;
   std::map<std::string, float> naval_pms{{"pm_basic_bigship", 25.0F},
       {"pm_basic_smallship", 50.0F},
       {"pm_1936_bigship", 21.0F},
       {"pm_1936_smallship", 11.0F}};
   std::map<std::string, int> name_counts;
   tmpl3.AddShipsIfPossible(ships, name_counts, naval_pms);
   tmpl2.AddShipsIfPossible(ships, name_counts, naval_pms);
   tmpl1.AddShipsIfPossible(ships, name_counts, naval_pms);

   EXPECT_THAT(ships,
       testing::ElementsAre(Ship(ShipOptions{
                                .name = "Big Ship 1",
                                .definition = "test2",
                                .equipment = "test2",
                                .legacy_equipment = "legacy_test2",
                                .version = "1936 Big Ship",
                            }),
           Ship(ShipOptions{
               .name = "Small Ship 1",
               .definition = "test2",
               .equipment = "test2",
               .legacy_equipment = "legacy_test2",
               .version = "1936 Small Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship 2",
               .definition = "test2",
               .equipment = "test2",
               .legacy_equipment = "legacy_test2",
               .version = "1936 Small Ship",
           }),
           Ship(ShipOptions{
               .name = "Big Ship 2",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Big Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship 3",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Small Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship 4",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Small Ship",
           }),
           Ship(ShipOptions{
               .name = "Big Ship 3",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Big Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship 5",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Small Ship",
           }),
           Ship(ShipOptions{
               .name = "Small Ship 6",
               .definition = "test1",
               .equipment = "test1",
               .legacy_equipment = "legacy_test1",
               .version = "Basic Small Ship",
           })));
   EXPECT_THAT(naval_pms,
       testing::ContainerEq(std::map<std::string, float>{{"pm_basic_bigship", 5.0F},
           {"pm_basic_smallship", 30.0F},
           {"pm_1936_bigship", 11.0F},
           {"pm_1936_smallship", 1.0F},
           {"pm_humongous", 0.0F}}));
}

}  // namespace hoi4
