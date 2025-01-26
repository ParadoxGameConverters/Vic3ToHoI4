#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/culture/culture_graphics_mapper.h"


namespace mappers
{
namespace
{
GraphicsBlock block0{{
                         {"army", {"army0"}},
                         {"navy", {"navy0"}},
                         {"leader_communism", {"c_leader0"}},
                         {"leader_democratic", {"d_leader0"}},
                         {"leader_fascism", {"f_leader0"}},
                         {"leader_neutrality", {"n_leader0"}},
                         {"female_leader", {"fem_leader0"}},
                         {"advisor_communism", {"c_minister0"}},
                         {"advisor_democratic", {"d_minister0"}},
                         {"advisor_fascism", {"f_minister0"}},
                         {"advisor_neutrality", {"n_minister0"}},
                         {"operative_male", {"m_op0"}},
                         {"operative_female", {"f_op0"}},
                         {"monarch_male", {"king0"}},
                         {"monarch_female", {"queen0"}},
                         {"council", {"council0"}},
                     },
    "west",
    "west_2d"};
GraphicsBlock block1{{
                         {"army", {"army1"}},
                         {"navy", {"navy1"}},
                         {"leader_communism", {"c_leader1"}},
                         {"leader_democratic", {"d_leader1"}},
                         {"leader_fascism", {"f_leader1"}},
                         {"leader_neutrality", {"n_leader1"}},
                         {"female_leader", {"fem_leader1"}},
                         {"advisor_communism", {"c_minister1"}},
                         {"advisor_democratic", {"d_minister1"}},
                         {"advisor_fascism", {"f_minister1"}},
                         {"advisor_neutrality", {"n_minister1"}},
                         {"operative_male", {"m_op1"}},
                         {"operative_female", {"f_op1"}},
                         {"monarch_male", {"king1"}},
                         {"monarch_female", {"queen1"}},
                         {"council", {"council1"}},
                     },
    "west",
    "west_2d"};
GraphicsBlock block2{{
                         {"army", {"army2"}},
                         {"navy", {"navy2"}},
                         {"leader_communism", {"c_leader2"}},
                         {"leader_democratic", {"d_leader2"}},
                         {"leader_fascism", {"f_leader2"}},
                         {"leader_neutrality", {"n_leader2"}},
                         {"female_leader", {"fem_leader2"}},
                         {"advisor_communism", {"c_minister2"}},
                         {"advisor_democratic", {"d_minister2"}},
                         {"advisor_fascism", {"f_minister2"}},
                         {"advisor_neutrality", {"n_minister2"}},
                         {"operative_male", {"m_op2"}},
                         {"operative_female", {"f_op2"}},
                         {"monarch_male", {"king2"}},
                         {"monarch_female", {"queen2"}},
                         {"council", {"council2"}},
                     },
    "east",
    "east_2d"};
GraphicsBlock blocks1_2{{
                            {"army", {"army1", "army2"}},
                            {"navy", {"navy1", "navy2"}},
                            {"leader_communism", {"c_leader1", "c_leader2"}},
                            {"leader_democratic", {"d_leader1", "d_leader2"}},
                            {"leader_fascism", {"f_leader1", "f_leader2"}},
                            {"leader_neutrality", {"n_leader1", "n_leader2"}},
                            {"female_leader", {"fem_leader1", "fem_leader2"}},
                            {"advisor_communism", {"c_minister1", "c_minister2"}},
                            {"advisor_democratic", {"d_minister1", "d_minister2"}},
                            {"advisor_fascism", {"f_minister1", "f_minister2"}},
                            {"advisor_neutrality", {"n_minister1", "n_minister2"}},
                            {"operative_male", {"m_op1", "m_op2"}},
                            {"operative_female", {"f_op1", "f_op2"}},
                            {"monarch_male", {"king1", "king2"}},
                            {"monarch_female", {"queen1", "queen2"}},
                            {"council", {"council1", "council2"}},
                        },
    "west",
    "west_2d"};
GraphicsBlock block_incomplete{{}, "asian", "asian_2d"};
GraphicsBlock block3{{{"army", {"army3"}}}, "", ""};

std::vector<CultureGraphicsMapping> mappings{{{"cul0", "cul1"}, {}, {}, block0},
    {{}, {"trait0", "trait3"}, {}, block1},
    {{}, {"trait1"}, {"eth0"}, block2},
    {{"cul3"}, {}, {}, block_incomplete},
    {{"cul4"}, {}, {}, block3}};

std::map<std::string, vic3::CultureDefinition> culture_defs{
    {"cul2", vic3::CultureDefinition({"cul2"}, {}, {"trait0"}, {"eth2"})},
    {"cul20", vic3::CultureDefinition({"cul20"}, {}, {"trait2"}, {"eth0"})}};
}  // namespace


TEST(MappersCultureCultureGraphicsMapperTests, NoMatchMeanNoPortraits)
{
   const CultureGraphicsMapper culture_graphics_mapper({});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition("", {}, {}, {})),
       GraphicsBlock({{}, "western_european_gfx", "western_european_2d"}));
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
   EXPECT_EQ(
       culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({"cul3"}, {}, {"trait2"}, {"eth0"})),
       GraphicsBlock(block2.portrait_paths, "asian", "asian_2d"));
}

TEST(MappersCultureCultureGraphicsMapperTests, MissingUnitGraphicsGivesWarning)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   EXPECT_EQ(
       culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({"cul4"}, {}, {"trait2"}, {"eth2"})),
       GraphicsBlock(block3.portrait_paths, "western_european_gfx", "western_european_2d"));

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] Culture cul4 lacks unit graphics. Defaulting to western.)"));
}
}  // namespace mappers