#include <sstream>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/map/strategic_regions_importer.h"



namespace hoi4
{

TEST(Hoi4WorldMapStrategicRegionsImporter, DefaultsAreEmpty)
{
   const commonItems::ModFilesystem mod_filesystem("", {});
   const auto strategic_regions = ImportStrategicRegions(mod_filesystem);

   EXPECT_TRUE(strategic_regions.GetStrategicRegions().empty());
   EXPECT_TRUE(strategic_regions.GetProvinceToStrategicRegionMap().empty());
}


TEST(Hoi4WorldMapStrategicRegionsImporter, StrategicRegionsCanBeImported)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/hoi4_world", {});
   const auto strategic_regions = ImportStrategicRegions(mod_filesystem);

   EXPECT_THAT(strategic_regions.GetStrategicRegions(),
       testing::UnorderedElementsAre(testing::Pair(42,
           StrategicRegion("test_strategic_region.txt",
               42,
               "STRATEGICREGION_42",
               {144, 169},
               {{"test_modifier", "always"}, {"test_modifier_two", "always"}},
               "test_naval_terrain",
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
               "\t}"))));
   EXPECT_THAT(strategic_regions.GetProvinceToStrategicRegionMap(),
       testing::UnorderedElementsAre(testing::Pair(144, 42), testing::Pair(169, 42)));
}

}  // namespace hoi4