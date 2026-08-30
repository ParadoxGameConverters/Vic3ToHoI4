#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/mappers/culture/culture_graphics_mapper.h"


namespace mappers
{
namespace
{

GraphicsBlock GetBlock0()
{
   static const GraphicsBlock kBlock0{
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
   return kBlock0;
}


GraphicsBlock GetBlock1()
{
   static const GraphicsBlock kBlock1{
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
   return kBlock1;
}


GraphicsBlock GetBlock2()
{
   static const GraphicsBlock kBlock2{
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
   return kBlock2;
}


GraphicsBlock GetBlock3()
{
   static const GraphicsBlock kBlock3{
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
   return kBlock3;
}


GraphicsBlock GetBlocks13()
{
   static const GraphicsBlock kBlocks13{
       .portrait_paths =
           {
               {"army", {"army1", "army3"}},
               {"navy", {"navy1", "navy3"}},
               {"leader_communism", {"c_leader1", "c_leader3"}},
               {"leader_democratic", {"d_leader1", "d_leader3"}},
               {"leader_fascism", {"f_leader1", "f_leader3"}},
               {"leader_neutrality", {"n_leader1", "n_leader3"}},
               {"female_leader", {"fem_leader1", "fem_leader3"}},
               {"advisor_communism", {"c_minister1", "c_minister3"}},
               {"advisor_democratic", {"d_minister1", "d_minister3"}},
               {"advisor_fascism", {"f_minister1", "f_minister3"}},
               {"advisor_neutrality", {"n_minister1", "n_minister3"}},
               {"operative_male", {"m_op1", "m_op3"}},
               {"operative_female", {"f_op1", "f_op3"}},
               {"monarch_male", {"king1", "king3"}},
               {"monarch_female", {"queen1", "queen3"}},
               {"council", {"council1", "council3"}},
           },
       .graphical_culture = "west",
       .graphical_culture_2d = "west_2d",
   };
   return kBlocks13;
}


GraphicsBlock GetBlocks23()
{
   static const GraphicsBlock kBlocks23{
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
   return kBlocks23;
}


GraphicsBlock GetBlockIncomplete()
{
   static const GraphicsBlock kBlockIncomplete{.graphical_culture = "asian", .graphical_culture_2d = "asian_2d"};
   return kBlockIncomplete;
}


GraphicsBlock GetBlock4()
{
   static const GraphicsBlock kBlock4{.portrait_paths = {{"army", {"army4"}}}};
   return kBlock4;
}


std::vector<CultureGraphicsMapping> GetMappings()
{
   static const std::vector<CultureGraphicsMapping> kMappings{
       {.cultures = {"cul0", "cul1"}, .graphics_block = GetBlock0()},
       {.heritages = {"heritage0", "heritage3"}, .graphics_block = GetBlock1()},
       {.traits = {"trait0", "trait3"}, .graphics_block = GetBlock2()},
       {.traits = {"trait1"}, .ethnicities = {"eth0"}, .graphics_block = GetBlock3()},
       {.cultures = {"cul3"}, .graphics_block = GetBlockIncomplete()},
       {.cultures = {"cul4"}, .graphics_block = GetBlock4()},
   };
   return kMappings;
}


std::map<std::string, vic3::CultureDefinition> GetCultureDefs()
{
   static const std::map<std::string, vic3::CultureDefinition> kCultureDefs{
       {
           "cul2",
           vic3::CultureDefinition({
               .name = "cul2",
               .traits = {"trait0"},
               .heritage = "heritage0",
               .ethnicities = {"eth2"},
           }),
       },
       {
           "cul20",
           vic3::CultureDefinition({
               .name = "cul20",
               .traits = {"trait2"},
               .heritage = "heritage2",
               .ethnicities = {"eth0"},
           }),
       },
   };
   return kCultureDefs;
}

}  // namespace


TEST(MappersCultureCultureGraphicsMapperTests, NoMatchMeanNoPortraits)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition({})),
       GraphicsBlock({{}, "western_european_gfx", "western_european_2d"}));
}


TEST(MappersCultureCultureGraphicsMapperTests, GraphicsSelectedOnCulture)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper(GetMappings());

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
                 {.name = "cul0", .traits = {"trait0"}, .heritage = "heritage0", .ethnicities = {"eth0"}})),
       GetBlock0());
}


TEST(MappersCultureCultureGraphicsMapperTests, GraphicsFallbackToHeritages)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
                 {.name = "cul2", .traits = {"trait0"}, .heritage = "heritage0", .ethnicities = {"eth0"}})),
       GetBlock1());
}


TEST(MappersCultureCultureGraphicsMapperTests, GraphicsFallbackToTraits)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
                 {.name = "cul2", .traits = {"trait0"}, .heritage = "heritage2", .ethnicities = {"eth0"}})),
       GetBlock2());
}


TEST(MappersCultureCultureGraphicsMapperTests, GraphicsFallbackBackToEthnicities)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
                 {.name = "cul2", .traits = {"trait2"}, .heritage = "heritage2", .ethnicities = {"eth0"}})),
       GetBlock3());
}


TEST(MappersCultureCultureGraphicsMapperTests, NoMatchGivesWarning)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   [[maybe_unused]] auto _ = culture_graphics_mapper.MatchCultureToGraphics(vic3::CultureDefinition(
       {.name = "cul2", .traits = {"trait2"}, .heritage = "heritage2", .ethnicities = {"eth2"}}));

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] Culture cul2 has no matching portrait set.)"));
}


TEST(MappersCultureCultureGraphicsMapperTests, BlocksCombineOnMultiMatch)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul2", .traits = {"trait0", "trait1"}, .ethnicities = {"eth0"}})),
       GetBlocks23());
}


TEST(MappersCultureCultureGraphicsMapperTests, MultiMatchDoesntDuplicate)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul2", .traits = {"trait0", "trait3"}, .ethnicities = {"eth0"}})),
       GetBlock2());
}


TEST(MappersCultureCultureGraphicsMapperTests, MatchPrimaryCulturesMatchesMultiple)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   EXPECT_EQ(culture_graphics_mapper.MatchPrimaryCulturesToGraphics({"cul2", "cul20"}, GetCultureDefs()),
       GetBlocks13());
}


TEST(MappersCultureCultureGraphicsMapperTests, IncompleteSectionsFillFromMoreGeneralMatches)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul3", .traits = {"trait0", "trait3"}, .ethnicities = {"eth0"}})),
       GraphicsBlock(GetBlock2().portrait_paths, "asian", "asian_2d"));
   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul3", .traits = {"trait2"}, .ethnicities = {"eth0"}})),
       GraphicsBlock(GetBlock3().portrait_paths, "asian", "asian_2d"));
}


TEST(MappersCultureCultureGraphicsMapperTests, MissingUnitGraphicsGivesWarning)  // NOLINT(cert-err58-cpp)
{
   const CultureGraphicsMapper culture_graphics_mapper({GetMappings()});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   EXPECT_EQ(culture_graphics_mapper.MatchCultureToGraphics(
                 vic3::CultureDefinition({.name = "cul4", .traits = {"trait2"}, .ethnicities = {"eth2"}})),
       GraphicsBlock(GetBlock4().portrait_paths, "western_european_gfx", "western_european_2d"));

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] Culture cul4 lacks unit graphics. Defaulting to western.)"));
}
}  // namespace mappers