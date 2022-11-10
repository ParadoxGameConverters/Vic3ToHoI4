#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/provinces/province_mapping.h"
#include "src/mappers/provinces/province_mapping_importer.h"



TEST(MappersProvincesProvinceMappingImporterTests, Vic2ProvincesDefaultsToZero)
{
   std::stringstream input;

   const auto mapping = mappers::ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_TRUE(mapping.vic3_provinces.empty());
}


TEST(MappersProvincesProvinceMappingImporterTests, Vic2ProvincesCanBeAdded)
{
   std::stringstream input;
   input << "= { vic3 = 42 vic3 = 144 }";

   const auto mapping = mappers::ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_THAT(mapping.vic3_provinces, testing::ElementsAre(42, 144));
}


TEST(MappersProvincesProvinceMappingImporterTests, HoI4ProvincesDefaultsToZero)
{
   std::stringstream input;

   const auto mapping = mappers::ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_TRUE(mapping.hoi4_provinces.empty());
}


TEST(MappersProvincesProvinceMappingImporterTests, HoI4ProvincesCanBeAdded)
{
   std::stringstream input;
   input << "= { hoi4 = 42 hoi4 = 144 }";

   const auto mapping = mappers::ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_THAT(mapping.hoi4_provinces, testing::ElementsAre(42, 144));
}