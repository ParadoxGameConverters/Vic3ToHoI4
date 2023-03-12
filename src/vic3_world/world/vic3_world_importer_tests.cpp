#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/world/vic3_world_importer.h"



namespace vic3
{

TEST(Vic3worldWorldVic3worldimporter, ExceptionForMissingSave)
{
   EXPECT_THROW(ImportWorld(configuration::Configuration{.save_game = "test_files/vic3_world/world/missing_save.vic3"}),
       std::runtime_error);
}


TEST(Vic3worldWorldVic3worldimporter, DefaultsAreCorrect)
{
   const auto world = ImportWorld(configuration::Configuration{.vic3_directory = "test_files/vic3_world/empty_world",
       .vic3_steam_mod_path = "test_files/vic3_world/documents/mod",
       .vic3_mod_path = "test_files/vic3_world/documents/mod",
       .save_game = "test_files/vic3_world/world/empty_save.vic3"});

   EXPECT_TRUE(world.GetCountries().empty());
   EXPECT_TRUE(world.GetStates().empty());
   EXPECT_TRUE(world.GetProvinceDefinitions().GetProvinceDefinitions().empty());
   EXPECT_TRUE(world.GetAcquiredTechnologies().empty());
   EXPECT_EQ(world.GetLocalizations().size(), 0);
}


TEST(Vic3worldWorldVic3worldimporter, WorldCanBeImported)
{
   const auto world = ImportWorld(configuration::Configuration{.vic3_directory = "test_files/vic3_world/world",
       .vic3_steam_mod_path = "test_files/vic3_world/documents/mod",
       .vic3_mod_path = "test_files/vic3_world/documents/mod",
       .save_game = "test_files/vic3_world/world/test_save.vic3"});

   EXPECT_THAT(world.GetCountries(),
       testing::UnorderedElementsAre(
           testing::Pair(1, Country({.tag = "TAG", .color = commonItems::Color(std::array{1, 2, 3})})),
           testing::Pair(3, Country({.tag = "TWO", .color = commonItems::Color(std::array{2, 4, 6})}))));
   EXPECT_THAT(world.GetStates(),
       testing::UnorderedElementsAre(testing::Pair(0, State({.provinces = {1, 2, 3}})),
           testing::Pair(1, State({.owner_number = 3, .owner_tag = "TWO", .provinces = {10, 11, 12}})),
           testing::Pair(2, State({.owner_number = 133, .provinces = {20}}))));
   EXPECT_THAT(world.GetStateRegions(),
       testing::UnorderedElementsAre(testing::Pair("STATE_LOMBARDY",
                                         StateRegion(std::map<std::string, std::string>{{"xD04060", "city"},
                                                         {"x867A90", "farm"},
                                                         {"x3F1E38", "mine"},
                                                         {"x4713EE", "wood"}},
                                             {"x3F1E38", "x4713EE"})),
           testing::Pair("STATE_PIEDMONT",
               StateRegion(std::map<std::string, std::string>{{"xA9397D", "city"},
                               {"x904061", "port"},
                               {"x11C061", "farm"},
                               {"x1140E0", "mine"},
                               {"xD080E0", "wood"}},
                   {"x1140E0", "x11C061"})),
           testing::Pair("STATE_SVEALAND",
               StateRegion(std::map<std::string, std::string>{{"x9686A5", "city"},
                               {"x93C3BC", "port"},
                               {"xF48646", "farm"},
                               {"x6F40EC", "mine"},
                               {"x4C9918", "wood"}},
                   {"x0974E5", "x216569"}))));
   EXPECT_THAT(world.GetAcquiredTechnologies(),
       testing::UnorderedElementsAre(testing::Pair(1, std::set<std::string>{"technology_one", "technology_two"}),
           testing::Pair(3, std::set<std::string>{"technology_three"})));
   EXPECT_EQ(world.GetLocalizations().size(), 1);
   ASSERT_TRUE(world.GetLocalizations().HasLocalization("test_localisation"));
   EXPECT_EQ(world.GetLocalizations().GetLocalizationBlock("test_localisation")->GetLocalization("english"),
       "testing testing 1 2 3");
}


TEST(Vic3worldWorldVic3worldimporter, StateWithInvalidOwnerIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto world = ImportWorld(configuration::Configuration{.vic3_directory = "test_files/vic3_world/world",
       .vic3_steam_mod_path = "test_files/vic3_world/documents/mod",
       .vic3_mod_path = "test_files/vic3_world/documents/mod",
       .save_game = "test_files/vic3_world/world/test_save.vic3"});

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] State 2 had an owner with no definition.)"));
}


TEST(Vic3worldWorldVic3worldimporter, ModsInSaveAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto world = ImportWorld(configuration::Configuration{.vic3_directory = "test_files/vic3_world/world",
       .vic3_steam_mod_path = "test_files/vic3_world/workshop/529340",
       .vic3_mod_path = "test_files/vic3_world/documents/mod",
       .save_game = "test_files/vic3_world/world/test_save.vic3"});

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[INFO] \t\t->> Found potentially useful [Test Mod]: test_files/vic3_world/documents/mod/test_mod/"));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[INFO] \t\t->> Found potentially useful [Test Mod Two]: "
                          "test_files/vic3_world/workshop/529340/test_mod_two/"));
}


}  // namespace vic3