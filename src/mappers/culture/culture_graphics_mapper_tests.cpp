#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/culture/culture_graphics_mapper.h"


namespace mappers
{
namespace
{
GraphicsBlock block0{
    .portrait_paths =
        {
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
    .graphical_culture = "west",
    .graphical_culture_2d = "west_2d",
};
GraphicsBlock block1{
    .portrait_paths =
        {
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
    .graphical_culture = "west",
    .graphical_culture_2d = "west_2d",
};
GraphicsBlock block2{
    .portrait_paths =
        {
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
    .graphical_culture = "east",
    .graphical_culture_2d = "east_2d",
};
GraphicsBlock block3{
    .portrait_paths =
        {
            {"army", {"army3"}},
            {"navy", {"navy3"}},
            {"leader_communism", {"c_leader3"}},
            {"leader_democratic", {"d_leader3"}},
            {"leader_fascism", {"f_leader3"}},
            {"leader_neutrality", {"n_leader3"}},
            {"female_leader", {"fem_leader3"}},
            {"advisor_communism", {"c_minister3"}},
            {"advisor_democratic", {"d_minister3"}},
            {"advisor_fascism", {"f_minister3"}},
            {"advisor_neutrality", {"n_minister3"}},
            {"operative_male", {"m_op3"}},
            {"operative_female", {"f_op3"}},
            {"monarch_male", {"king3"}},
            {"monarch_female", {"queen3"}},
            {"council", {"council3"}},
        },
    .graphical_culture = "east",
    .graphical_culture_2d = "east_2d",
};
GraphicsBlock blocks2_3{
    .portrait_paths =
        {
            {"army", {"army2", "army3"}},
            {"navy", {"navy2", "navy3"}},
            {"leader_communism", {"c_leader2", "c_leader3"}},
            {"leader_democratic", {"d_leader2", "d_leader3"}},
            {"leader_fascism", {"f_leader2", "f_leader3"}},
            {"leader_neutrality", {"n_leader2", "n_leader3"}},
            {"female_leader", {"fem_leader2", "fem_leader3"}},
            {"advisor_communism", {"c_minister2", "c_minister3"}},
            {"advisor_democratic", {"d_minister2", "d_minister3"}},
            {"advisor_fascism", {"f_minister2", "f_minister3"}},
            {"advisor_neutrality", {"n_minister2", "n_minister3"}},
            {"operative_male", {"m_op2", "m_op3"}},
            {"operative_female", {"f_op2", "f_op3"}},
            {"monarch_male", {"king2", "king3"}},
            {"monarch_female", {"queen2", "queen3"}},
            {"council", {"council2", "council3"}},
        },
    .graphical_culture = "east",
    .graphical_culture_2d = "east_2d",
};
GraphicsBlock block_incomplete{.graphical_culture = "asian", .graphical_culture_2d = "asian_2d"};
GraphicsBlock block4{.portrait_paths = {{"army", {"army4"}}}};

std::vector<CultureGraphicsMapping> mappings{
    {.cultures = {"cul0", "cul1"}, .graphics_block = block0},
    {.heritages = {"heritage0", "heritage3"}, .graphics_block = block1},
    {.traits = {"trait0", "trait3"}, .graphics_block = block2},
    {.traits = {"trait1"}, .ethnicities = {"eth0"}, .graphics_block = block3},
    {.cultures = {"cul3"}, .graphics_block = block_incomplete},
    {.cultures = {"cul4"}, .graphics_block = block4},
};

std::map<std::string, vic3::CultureDefinition> culture_defs{
    {"cul2",
        vic3::CultureDefinition(
            {.name = "cul2", .traits = {"trait0"}, .heritage = "heritage0", .ethnicities = {"eth2"}})},
    {"cul20",
        vic3::CultureDefinition(
            {.name = "cul20", .traits = {"trait2"}, .heritage = "heritage2", .ethnicities = {"eth0"}})}};
}  // namespace


TEST(MappersCultureCultureGraphicsMapperTests, NoMatchMeanNoPortraits)
{
   const CultureGraphicsMapper culture_graphics_mapper({});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({})),
       GraphicsBlock({{}, "western_european_gfx", "western_european_2d"}));
}


TEST(MappersCultureCultureGraphicsMapperTests, GraphicsSelectedOnCulture)
{
   const CultureGraphicsMapper culture_graphics_mapper(mappings);

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
                 {.name = "cul0", .traits = {"trait0"}, .heritage = "heritage0", .ethnicities = {"eth0"}})),
       block0);
}


TEST(MappersCultureCultureGraphicsMapperTests, GraphicsFallbackToHeritages)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
                 {.name = "cul2", .traits = {"trait0"}, .heritage = "heritage0", .ethnicities = {"eth0"}})),
       block1);
}


TEST(MappersCultureCultureGraphicsMapperTests, GraphicsFallbackToTraits)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
                 {.name = "cul2", .traits = {"trait0"}, .heritage = "heritage2", .ethnicities = {"eth0"}})),
       block2);
}


TEST(MappersCultureCultureGraphicsMapperTests, GraphicsFallbackBackToEthnicities)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
                 {.name = "cul2", .traits = {"trait2"}, .heritage = "heritage2", .ethnicities = {"eth0"}})),
       block3);
}


TEST(MappersCultureCultureGraphicsMapperTests, NoMatchGivesWarning)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   [[maybe_unused]] auto _ = culture_graphics_mapper.MatchCultureToGraphics(
       vic3::CultureDefinition({.name = "cul2", .traits = {"trait2"}, .ethnicities = {"eth2"}}));

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] Culture cul2 has no matching portrait set.)"));
}


TEST(MappersCultureCultureGraphicsMapperTests, BlocksCombineOnMultiMatch)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul2", .traits = {"trait0", "trait1"}, .ethnicities = {"eth0"}})),
       blocks2_3);
}


TEST(MappersCultureCultureGraphicsMapperTests, MultiMatchDoesntDuplicate)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul2", .traits = {"trait0", "trait3"}, .ethnicities = {"eth0"}})),
       block2);
}


TEST(MappersCultureCultureGraphicsMapperTests, MatchPrimaryCulturesMatchesMultiple)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchPrimaryCulturesToGraphics({"cul2", "cul20"}, culture_defs), blocks2_3);
}


TEST(MappersCultureCultureGraphicsMapperTests, IncompleteSectionsFillFromMoreGeneralMatches)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul3", .traits = {"trait0", "trait3"}, .ethnicities = {"eth0"}})),
       GraphicsBlock(block2.portrait_paths, "asian", "asian_2d"));
   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul3", .traits = {"trait2"}, .ethnicities = {"eth0"}})),
       GraphicsBlock(block3.portrait_paths, "asian", "asian_2d"));
}


TEST(MappersCultureCultureGraphicsMapperTests, MissingUnitGraphicsGivesWarning)
{
   const CultureGraphicsMapper culture_graphics_mapper({mappings});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul4", .traits = {"trait2"}, .ethnicities = {"eth2"}})),
       GraphicsBlock(block4.portrait_paths, "western_european_gfx", "western_european_2d"));

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] Culture cul4 lacks unit graphics. Defaulting to western.)"));
}
}  // namespace mappers