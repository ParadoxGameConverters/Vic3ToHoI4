#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper_importer.h"



namespace mappers
{
namespace
{
GraphicsBlock block{
    .portrait_paths =
        {
            {"army", {"general0.dds", "general1.dds"}},
            {"navy", {"admiral0.dds", "admiral1.dds"}},
            {"scientist_female", {"scientist0.dds", "scientist1.dds"}},
            {"scientist_male", {"scientist2.dds", "scientist3.dds"}},
            {"leader_communism", {"communism0.dds"}},
            {"leader_democratic", {"democratic0.dds"}},
            {"leader_fascism", {"fascism0.dds"}},
            {"leader_neutrality", {"neutrality0.dds"}},
            {"female_leader", {"female_leader.dds"}},
            {"advisor_communism", {"communism1.dds"}},
            {"advisor_democratic", {"democratic1.dds"}},
            {"advisor_fascism", {"fascism1.dds"}},
            {"advisor_neutrality", {"neutrality1.dds"}},
            {"operative_female", {"spy_f0.dds", "spy_f1.dds"}},
            {"operative_male", {"spy_m0.dds", "spy_m1.dds"}},
            {"monarch_male", {"king.dds"}},
            {"monarch_female", {"queen.dds"}},
            {"council", {"council.dds"}},
        },
    .graphical_culture = "west",
    .graphical_culture_2d = "west_2d",
};
}


TEST(MappersCultureCultureGraphicsMapperImporterTests, MappingsCanBeImported)
{
   const CultureGraphicsMapper ideology_mapper =
       ImportCultureGraphicsMapper("test_files/mappers/culture/culture_graphics.txt");

   EXPECT_EQ(ideology_mapper.MatchCultureToGraphics(vic3::CultureDefinition{"swedish", {}, {}, {}}), block);
   EXPECT_EQ(ideology_mapper.MatchCultureToGraphics(vic3::CultureDefinition{{}, {}, {"european_heritage"}, {}}), block);
   EXPECT_EQ(ideology_mapper.MatchCultureToGraphics(vic3::CultureDefinition{{}, {}, {}, {"caucasian"}}), block);
}


TEST(MappersCultureCultureGraphicsMapperImporterTests, NumberOfMappingsIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const CultureGraphicsMapper ideology_mapper =
       ImportCultureGraphicsMapper("test_files/mappers/culture/culture_graphics.txt");

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 1 culture graphics mappings."));
}

}  // namespace mappers