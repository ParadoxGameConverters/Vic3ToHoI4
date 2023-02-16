#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/mappers/technology/tech_mappings_importer.h"



TEST(MappersTechnologyTechmappingsimporterTests, TechMappingsAreImported)
{
   const std::vector<mappers::TechMapping> tech_mappings = mappers::ImportTechMappings();

   EXPECT_THAT(tech_mappings,
       testing::ElementsAre(mappers::TechMapping{std::set<std::string>{"source_tech"},
                                std::nullopt,
                                std::set<std::string>{"dest_tech_one", "dest_tech_two"}},
           mappers::TechMapping{std::set<std::string>{"source_tech_two", "source_tech_three"},
               std::optional{R"(not = { has_dlc = "Test DLC" })"},
               std::set<std::string>{"dest_tech_three"}}));
}