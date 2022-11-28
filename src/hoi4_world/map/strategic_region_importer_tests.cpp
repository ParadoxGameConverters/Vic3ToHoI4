#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/strategic_region.h"
#include "src/hoi4_world/map/strategic_region_importer.h"



namespace hoi4
{

TEST(Hoi4WorldMapStrategicRegionImporter, FilenameIsFromFilename)
{
   const auto strategic_region = StrategicRegionImporter{}.ImportStrategicRegion(
       "test_files/hoi4_world/map/strategicregions/empty_strategic_region.txt");

   EXPECT_EQ(strategic_region.GetFilename(), "empty_strategic_region.txt");
}


TEST(Hoi4WorldMapStrategicRegionImporter, DefaultsAreSet)
{
   const auto strategic_region = StrategicRegionImporter{}.ImportStrategicRegion(
       "test_files/hoi4_world/map/strategicregions/empty_strategic_region.txt");

   EXPECT_EQ(strategic_region.GetID(), 0);
   EXPECT_TRUE(strategic_region.GetName().empty());
   EXPECT_TRUE(strategic_region.GetOldProvinces().empty());
   EXPECT_FALSE(strategic_region.hasStaticModifiers());
   EXPECT_TRUE(strategic_region.GetStaticModifiers().empty());
   EXPECT_FALSE(strategic_region.GetNavalTerrain().has_value());
   EXPECT_TRUE(strategic_region.GetWeather().empty());
}


TEST(Hoi4WorldMapStrategicRegionImporter, ItemsCanBeImported)
{
   const auto strategic_region = StrategicRegionImporter{}.ImportStrategicRegion(
       "test_files/hoi4_world/map/strategicregions/test_strategic_region.txt");

   EXPECT_EQ(strategic_region.GetID(), 42);
   EXPECT_EQ(strategic_region.GetName(), "STRATEGICREGION_42");
   EXPECT_THAT(strategic_region.GetOldProvinces(), testing::UnorderedElementsAre(144, 169));
   EXPECT_TRUE(strategic_region.hasStaticModifiers());
   EXPECT_THAT(strategic_region.GetStaticModifiers(),
       testing::UnorderedElementsAre(testing::Pair("test_modifier", "always"),
           testing::Pair("test_modifier_two", "always")));
   ASSERT_TRUE(strategic_region.GetNavalTerrain().has_value());
   EXPECT_EQ(strategic_region.GetNavalTerrain().value(), "test_naval_terrain");
   EXPECT_EQ(strategic_region.GetWeather(),
       "= {\n"
       "\t\tperiod={\n"
       "\t\t\tbetween={ 0.0 30.0 }\n"
       "\t\t\ttemperature={ -6.0 12.0 }\n"
       "\t\t\tno_phenomenon=0.500\n"
       "\t\t\train_light=1.000\n"
       "\t\t\train_heavy=0.150\n"
       "\t\t\tsnow=0.200\n"
       "\t\t\tblizzard=0.000\n"
       "\t\t\tarctic_water=0.000\n"
       "\t\t\tmud=0.300\n"
       "\t\t\tsandstorm=0.000\n"
       "\t\t\tmin_snow_level=0.000\n"
       "\t\t}\n"
       "\t\tperiod={\n"
       "\t\t\tbetween={ 0.1 27.1 }\n"
       "\t\t\ttemperature={ -7.0 12.0 }\n"
       "\t\t\tno_phenomenon=0.500\n"
       "\t\t\train_light=1.000\n"
       "\t\t\train_heavy=0.150\n"
       "\t\t\tsnow=0.200\n"
       "\t\t\tblizzard=0.050\n"
       "\t\t\tarctic_water=0.000\n"
       "\t\t\tmud=0.300\n"
       "\t\t\tsandstorm=0.000\n"
       "\t\t\tmin_snow_level=0.000\n"
       "\t\t}\n"
       "\t}");
}

}  // namespace hoi4