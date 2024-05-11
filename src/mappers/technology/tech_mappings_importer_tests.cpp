#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/mappers/technology/tech_mappings_importer.h"



namespace mappers
{

TEST(MappersTechnologyTechmappingsimporterTests, TechMappingsAreImported)
{
   const std::vector<TechMapping> tech_mappings = ImportTechMappings();

   EXPECT_THAT(tech_mappings,
       testing::ElementsAre(TechMapping{std::set<std::string>{"source_tech"},
                                std::nullopt,
                                std::set<std::string>{"dest_tech_one", "dest_tech_two"}},
           TechMapping{std::set<std::string>{"source_tech_two", "source_tech_three"},
               std::optional{R"(not = { has_dlc = "Test DLC" })"},
               std::set<std::string>{"dest_tech_three"}}));
}

}  // namespace mappers