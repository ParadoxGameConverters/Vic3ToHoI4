#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/hoi4_world/map/hoi4_province_definition_importer.h"



namespace hoi4
{

TEST(Hoi4worldMapHoi4provincedefinitionimporter, ExceptionThrownForNoDefinitions)
{
   const commonItems::ModFilesystem mod_filesystem("", {});

   EXPECT_THROW([[maybe_unused]] const maps::ProvinceDefinitions province_definitions =
                    ImportProvinceDefinitions(mod_filesystem),
       std::runtime_error);
}


TEST(Hoi4worldMapHoi4provincedefinitionimporter, DefinitionsCanBeImported)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/hoi4_world", {});

   const maps::ProvinceDefinitions province_definitions = ImportProvinceDefinitions(mod_filesystem);

   EXPECT_THAT(province_definitions.GetLandProvinces(),
       testing::UnorderedElementsAre("10", "20", "30", "40", "50", "60"));
}


TEST(Hoi4worldMapHoi4provincedefinitionimporter, SeaProvincesAreDetected)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/hoi4_world", {});

   const maps::ProvinceDefinitions province_definitions = ImportProvinceDefinitions(mod_filesystem);

   EXPECT_TRUE(province_definitions.IsSeaProvince("2"));
}


TEST(Hoi4worldMapHoi4provincedefinitionimporter, ContinentsAreDetected)
{
   const commonItems::ModFilesystem mod_filesystem("test_files/hoi4_world", {});

   const maps::ProvinceDefinitions province_definitions = ImportProvinceDefinitions(mod_filesystem);

   EXPECT_EQ(province_definitions.GetContinent("10"), "europe");
   EXPECT_EQ(province_definitions.GetContinent("20"), "north_america");
   EXPECT_EQ(province_definitions.GetContinent("30"), "south_america");
   EXPECT_EQ(province_definitions.GetContinent("40"), "australia");
   EXPECT_EQ(province_definitions.GetContinent("50"), std::nullopt);
   EXPECT_EQ(province_definitions.GetContinent("60"), std::nullopt);
}

}  // namespace hoi4