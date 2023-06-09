#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper_importer.h"



namespace mappers
{
namespace
{
GraphicsBlock block{{{"general0.dds", "general1.dds"},
                        {"admiral0.dds", "admiral1.dds"},
                        {{"communism0.dds"}, {"democratic0.dds"}, {"fascism0.dds"}, {"neutrality0.dds"}},
                        {"female_leader.dds"},
                        {{"communism1.dds"}, {"democratic1.dds"}, {"fascism1.dds"}, {"neutrality1.dds"}},
                        {"spy_m0.dds", "spy_m1.dds"},
                        {"spy_f0.dds", "spy_f1.dds"},
                        {"king.dds"},
                        {"queen.dds"}},
    {"west"},
    {"west_2d"}};
}
TEST(MappersCultureCultureGraphicsMapperImporterTests, MappingsCanBeImported)
{
   const CultureGraphicsMapper ideology_mapper =
       ImportCultureGraphicsMapper("test_files/mappers/culture/culture_graphics.txt");

   EXPECT_EQ(ideology_mapper.MatchCultureToGraphics(vic3::CultureDefinition{"swedish", {}, {}, {}}), block);
   EXPECT_EQ(ideology_mapper.MatchCultureToGraphics(vic3::CultureDefinition{{}, {}, {"european_heritage"}, {}}), block);
   EXPECT_EQ(ideology_mapper.MatchCultureToGraphics(vic3::CultureDefinition{{}, {}, {}, {"caucasian"}}), block);
}

}  // namespace mappers