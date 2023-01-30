#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/hoi4_province_definition_importer.h"



TEST(Hoi4worldMapHoi4provincedefinitionimporter, ExceptionThrownForNoDefinitions)
{
   commonItems::ModFilesystem mod_filesystem("", {});

   EXPECT_THROW(const maps::ProvinceDefinitions province_definitions = hoi4::ImportProvinceDefinitions(mod_filesystem),
       std::runtime_error);
}


TEST(Hoi4worldMapHoi4provincedefinitionimporter, DefinitionsCanBeImported)
{
   commonItems::ModFilesystem mod_filesystem("test_files/hoi4_world", {});

   const maps::ProvinceDefinitions province_definitions = hoi4::ImportProvinceDefinitions(mod_filesystem);

   EXPECT_THAT(province_definitions.GetLandProvinces(),
       testing::UnorderedElementsAre("10", "20", "30", "40", "50", "60"));
}


TEST(Hoi4worldMapHoi4provincedefinitionimporter, SeaProvincesAreDetected)
{
   commonItems::ModFilesystem mod_filesystem("test_files/hoi4_world", {});

   const maps::ProvinceDefinitions province_definitions = hoi4::ImportProvinceDefinitions(mod_filesystem);

   EXPECT_TRUE(province_definitions.IsSeaProvince("2"));
}