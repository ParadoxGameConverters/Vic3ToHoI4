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
       .vic3_mod_path = "test_files/vic3_world/documents",
       .save_game = "test_files/vic3_world/world/empty_save.vic3"});

   EXPECT_TRUE(world.GetCountries().empty());
   EXPECT_TRUE(world.GetStates().empty());
   EXPECT_TRUE(world.GetProvinceDefinitions().GetProvinceDefinitions().empty());
}


TEST(Vic3worldWorldVic3worldimporter, WorldCanBeImported)
{
   const auto world = ImportWorld(configuration::Configuration{.vic3_directory = "test_files/vic3_world/world",
       .vic3_mod_path = "test_files/vic3_world/documents",
       .save_game = "test_files/vic3_world/world/test_save.vic3"});

   EXPECT_THAT(world.GetCountries(),
       testing::UnorderedElementsAre(
           testing::Pair(1, Country({.tag = "TAG", .color = commonItems::Color(std::array{1, 2, 3})})),
           testing::Pair(3, Country({.tag = "TWO", .color = commonItems::Color(std::array{2, 4, 6})}))));
   EXPECT_THAT(world.GetStates(),
       testing::UnorderedElementsAre(testing::Pair(0, State({.provinces = {1, 2, 3}})),
           testing::Pair(1, State({.owner_number = 3, .owner_tag = "TWO", .provinces = {10, 11, 12}})),
           testing::Pair(2, State({.owner_number = 133, .provinces = {20}}))));
}


TEST(Vic3worldWorldVic3worldimporter, StateWithInvalidOwnerIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto world = ImportWorld(configuration::Configuration{.vic3_directory = "test_files/vic3_world/world",
       .vic3_mod_path = "test_files/vic3_world/documents",
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
       .vic3_mod_path = "test_files/vic3_world/documents",
       .save_game = "test_files/vic3_world/world/test_save.vic3"});

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr(R"([INFO] 		->> Found potentially useful [Test Mod]: mod/test_mod/)"));
}


}  // namespace vic3