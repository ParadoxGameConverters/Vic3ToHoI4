#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/provinces/province_mapping.h"
#include "src/mappers/provinces/province_mapping_importer.h"



namespace mappers
{

TEST(MappersProvincesProvinceMappingImporterTests, Vic2ProvincesDefaultsToZero)
{
   std::stringstream input;

   const auto mapping = ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_TRUE(mapping.vic3_provinces.empty());
}


TEST(MappersProvincesProvinceMappingImporterTests, Vic2ProvincesCanBeAdded)
{
   std::stringstream input;
   input << "= { vic3 = 0x000042 vic3 = 0x001440 }";

   const auto mapping = ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_THAT(mapping.vic3_provinces, testing::ElementsAre("x000042", "x001440"));
}


TEST(MappersProvincesProvinceMappingImporterTests, HoI4ProvincesDefaultsToZero)
{
   std::stringstream input;

   const auto mapping = ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_TRUE(mapping.hoi4_provinces.empty());
}


TEST(MappersProvincesProvinceMappingImporterTests, HoI4ProvincesCanBeAdded)
{
   std::stringstream input;
   input << "= { hoi4 = 42 hoi4 = 144 }";

   const auto mapping = ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_THAT(mapping.hoi4_provinces, testing::ElementsAre(42, 144));
}


TEST(MappersProvincesProvinceMappingImporterTests, CommentDefaultsToNullopt)
{
   std::stringstream input;

   const auto mapping = ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_FALSE(mapping.comment.has_value());
}


TEST(MappersProvincesProvinceMappingImporterTests, CommentCanBeSet)
{
   std::stringstream input;
   input << R"(= { comment = "test_comment" })";

   const auto mapping = ProvinceMappingImporter{}.ImportProvinceMapping(input);

   EXPECT_EQ(mapping.comment.value_or(""), "test_comment");
}

}  // namespace mappers