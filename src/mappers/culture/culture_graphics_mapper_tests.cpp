#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/culture/culture_graphics_mapper.h"


namespace mappers
{
namespace
{
GraphicsBlock block0{{{"army0"},
                         {"navy0"},
                         {{"c_leader0"}, {"d_leader0"}, {"f_leader0"}, {"n_leader0"}},
                         {"fem_leader0"},
                         {{"c_minister0"}, {"d_minister0"}, {"f_minister0"}, {"n_minister0"}},
                         {"m_op0"},
                         {"f_op0"},
                         {"king0"},
                         {"queen0"},
                         {"council0"}},
    "west",
    "west_2d"};
GraphicsBlock block1{{{"army1"},
                         {"navy1"},
                         {{"c_leader1"}, {"d_leader1"}, {"f_leader1"}, {"n_leader1"}},
                         {"fem_leader1"},
                         {{"c_minister1"}, {"d_minister1"}, {"f_minister1"}, {"n_minister1"}},
                         {"m_op1"},
                         {"f_op1"},
                         {"king1"},
                         {"queen1"},
                         {"council1"}},
    "west",
    "west_2d"};
GraphicsBlock block2{{{"army2"},
                         {"navy2"},
                         {{"c_leader2"}, {"d_leader2"}, {"f_leader2"}, {"n_leader2"}},
                         {"fem_leader2"},
                         {{"c_minister2"}, {"d_minister2"}, {"f_minister2"}, {"n_minister2"}},
                         {"m_op2"},
                         {"f_op2"},
                         {"king2"},
                         {"queen2"},
                         {"council2"}},
    "east",
    "east_2d"};
GraphicsBlock blocks1_2{{{"army1", "army2"},
                            {"navy1", "navy2"},
                            {{"c_leader1", "c_leader2"},
                                {"d_leader1", "d_leader2"},
                                {"f_leader1", "f_leader2"},
                                {"n_leader1", "n_leader2"}},
                            {"fem_leader1", "fem_leader2"},
                            {{"c_minister1", "c_minister2"},
                                {"d_minister1", "d_minister2"},
                                {"f_minister1", "f_minister2"},
                                {"n_minister1", "n_minister2"}},
                            {"m_op1", "m_op2"},
                            {"f_op1", "f_op2"},
                            {"king1", "king2"},
                            {"queen1", "queen2"},
                            {"council1", "council2"}},
    "west",
    "west_2d"};
GraphicsBlock block_incomplete{{}, "asian", "asian_2d"};

std::vector<CultureGraphicsMapping> mappings{{{"cul0", "cul1"}, {}, {}, block0},
    {{}, {"trait0", "trait3"}, {}, block1},
    {{}, {"trait1"}, {"eth0"}, block2},
    {{"cul3"}, {}, {}, block_incomplete}};

std::map<std::string, vic3::CultureDefinition> culture_defs{
    {"cul2", vic3::CultureDefinition({"cul2"}, {}, {"trait0"}, {"eth2"})},
    {"cul20", vic3::CultureDefinition({"cul20"}, {}, {"trait2"}, {"eth0"})}};
}  // namespace


TEST(MappersCultureCultureGraphicsMapperTests, NoMatchMeanNoPortraits)
{
   const CultureGraphicsMapper culture_graphics_mapper({});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({}, {}, {}, {})), GraphicsBlock{});
}

TEST(MappersCultureCultureGraphicsMapperTests, GraphicsSelectedOnCulture)
{
   const CultureGraphicsMapper culture_graphics_mapper(mappings);

   EXPECT_EQ(
       culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({"cul0"}, {}, {"trait0"}, {"eth0"})),
       block0);
}

TEST(MappersCultureCultureGraphicsMapperTests, GraphicsFallbackToTraits)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(
       culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({"cul2"}, {}, {"trait0"}, {"eth0"})),
       block1);
}

TEST(MappersCultureCultureGraphicsMapperTests, GraphicsFallbackBackToEthnicities)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(
       culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({"cul2"}, {}, {"trait2"}, {"eth0"})),
       block2);
}

TEST(MappersCultureCultureGraphicsMapperTests, NoMatchGivesWarning)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({"cul2"}, {}, {"trait2"}, {"eth2"}));

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] Culture cul2 has no matching portrait set.)"));
}

TEST(MappersCultureCultureGraphicsMapperTests, BlocksCombineOnMultiMatch)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({"cul2"}, {}, {"trait0", "trait1"}, {"eth0"})),
       blocks1_2);
}

TEST(MappersCultureCultureGraphicsMapperTests, MultiMatchDoesntDuplicate)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({"cul2"}, {}, {"trait0", "trait3"}, {"eth0"})),
       block1);
}

TEST(MappersCultureCultureGraphicsMapperTests, MatchPrimaryCulturesMatchesMultiple)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchPrimaryCulturesToGraphics({"cul2", "cul20"}, culture_defs), blocks1_2);
}

TEST(MappersCultureCultureGraphicsMapperTests, IncompleteSectionsFillFromMoreGeneralMatches)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({"cul3"}, {}, {"trait0", "trait3"}, {"eth0"})),
       GraphicsBlock(block1.portrait_paths, "asian", "asian_2d"));
   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({"cul3"}, {}, {"trait2"}, {"eth0"})),
       GraphicsBlock(block2.portrait_paths, "asian", "asian_2d"));
}
}  // namespace mappers