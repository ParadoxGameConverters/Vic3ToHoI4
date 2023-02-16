#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/mappers/technology/tech_mapping_importer.h"



TEST(MappersTechnologyTechmappingimporterTests, DefaultsAreEmpty)
{
   std::stringstream input;
   const auto mapping = mappers::TechMappingImporter().ImportTechMapping(input);

   EXPECT_TRUE(mapping.GetVic3Requirements().empty());
   EXPECT_FALSE(mapping.GetLimit().has_value());
   EXPECT_TRUE(mapping.GetTechs().empty());
}


TEST(MappersTechnologyTechmappingimporterTests, ItemsCanBeSet)
{
   std::stringstream input;
   input << "vic3 = test_item\n";
   input << "vic3 = test_item_two\n";
   input << "limit = test_limit\n";
   input << "hoi4 = test_tech\n";
   input << "hoi4 = test_tech2";
   const auto mapping = mappers::TechMappingImporter().ImportTechMapping(input);

   EXPECT_THAT(mapping.GetVic3Requirements(), testing::UnorderedElementsAre("test_item", "test_item_two"));
   ASSERT_TRUE(mapping.GetLimit().has_value());
   EXPECT_EQ(*mapping.GetLimit(), "test_limit");
   EXPECT_THAT(mapping.GetTechs(), testing::UnorderedElementsAre("test_tech", "test_tech2"));
}