#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/world/vic3_world_importer.h"



namespace vic3
{

TEST(Vic3worldWorldVic3worldimporter, ExceptionForMissingSave)
{
   EXPECT_THROW(ImportWorld(
                    configuration::Configuration{
                        .save_game = "test_files/vic3_world/world/missing_save.vic3",
                    },
                    commonItems::ConverterVersion()),
       std::runtime_error);
}


TEST(Vic3worldWorldVic3worldimporter, DefaultsAreCorrect)
{
   const auto world = ImportWorld(
       configuration::Configuration{
           .vic3_directory = "test_files/vic3_world/empty_world",
           .vic3_steam_mod_path = "test_files/vic3_world/documents/mod",
           .vic3_mod_path = "test_files/vic3_world/documents/mod",
           .save_game = "test_files/vic3_world/world/empty_save.vic3",
       },
       commonItems::ConverterVersion());

   EXPECT_TRUE(world.GetCountries().empty());
   EXPECT_TRUE(world.GetStates().empty());
   EXPECT_FALSE(world.GetProvinceDefinitions().GetProvinceDefinitions().empty());
   EXPECT_TRUE(world.GetAcquiredTechnologies().empty());
   EXPECT_EQ(world.GetLocalizations().size(), 0);
   EXPECT_TRUE(world.GetCultureDefinitions().empty());
   EXPECT_TRUE(world.GetCharacters().empty());
   EXPECT_TRUE(world.GetInterestGroups().empty());
}


TEST(Vic3worldWorldVic3worldimporter, WorldCanBeImported)
{
   const auto world = ImportWorld(
       configuration::Configuration{
           .vic3_directory = "test_files/vic3_world/world",
           .vic3_steam_mod_path = "test_files/vic3_world/documents/mod",
           .vic3_mod_path = "test_files/vic3_world/documents/mod",
           .save_game = "test_files/vic3_world/world/test_save.vic3",
       },
       commonItems::ConverterVersion());

   EXPECT_THAT(world.GetCountries(),
       testing::UnorderedElementsAre(testing::Pair(1,
                                         Country(
                                             {
                                                 .number = 1,
                                                 .tag = "TAG",
                                                 .color = commonItems::Color(std::array{1, 2, 3}),
                                                 .active_laws = {"law_monarchy"},
                                                 .primary_culture_ids = {0},
                                                 .primary_cultures = {"welsh"},
                                                 .head_of_state_id = 1,
                                                 .character_ids = {1, 2, 4},
                                                 .ig_ids = {1, 2},
                                                 .puppets = {3},
                                                 .army_formations =
                                                     {
                                                         {1234,
                                                             MilitaryFormation{
                                                                 .country = 1,
                                                                 .type = MilitaryFormationType::kArmy,
                                                                 .name = "Formation Name",
                                                                 .ordinal_number = 2,
                                                                 .units =
                                                                     {
                                                                         {"combat_unit_type_shrapnel_artillery", 2},
                                                                         {"combat_unit_type_dragoons", 3},
                                                                         {"combat_unit_type_trench_infantry", 24},
                                                                         {"combat_unit_type_siege_artillery", 2},
                                                                         {"combat_unit_type_skirmish_infantry", 8},
                                                                     },
                                                             }}},
                                             })),
           testing::Pair(3,
               Country({
                   .number = 3,
                   .tag = "TWO",
                   .color = commonItems::Color(std::array{2, 4, 6}),
                   .primary_culture_ids = {1},
                   .primary_cultures = {"scottish"},
                   .head_of_state_id = 5,
                   .character_ids = {5},
                   .ig_ids = {3},
                   .overlord = 1,
                   .navy_formations = {{5678,
                       MilitaryFormation{
                           .country = 3,
                           .type = MilitaryFormationType::kFleet,
                       }}},
               }))));
   EXPECT_THAT(world.GetStates(),
       testing::UnorderedElementsAre(testing::Pair(0, State({.provinces = {1, 2, 3}})),
           testing::Pair(1, State({.id = 1, .owner_number = 3, .owner_tag = "TWO", .provinces = {10, 11, 12}})),
           testing::Pair(2, State({.id = 2, .owner_number = 133, .provinces = {20}}))));
   EXPECT_THAT(world.GetStateRegions(),
       testing::UnorderedElementsAre(testing::Pair("STATE_LOMBARDY",
                                         StateRegion(
                                             std::map<std::string, std::string>{
                                                 {"xD04060", "city"},
                                                 {"x867A90", "farm"},
                                                 {"x3F1E38", "mine"},
                                                 {"x4713EE", "wood"},
                                             },
                                             {"x3F1E38", "x4713EE"})),
           testing::Pair("STATE_PIEDMONT",
               StateRegion(
                   std::map<std::string, std::string>{
                       {"xA9397D", "city"},
                       {"x904061", "port"},
                       {"x11C061", "farm"},
                       {"x1140E0", "mine"},
                       {"xD080E0", "wood"},
                   },
                   {"x1140E0", "x11C061"})),
           testing::Pair("STATE_SVEALAND",
               StateRegion(
                   std::map<std::string, std::string>{
                       {"x9686A5", "city"},
                       {"x93C3BC", "port"},
                       {"xF48646", "farm"},
                       {"x6F40EC", "mine"},
                       {"x4C9918", "wood"},
                   },
                   {"x0974E5", "x216569"}))));
   EXPECT_THAT(world.GetAcquiredTechnologies(),
       testing::UnorderedElementsAre(testing::Pair(1, std::set<std::string>{"technology_one", "technology_two"}),
           testing::Pair(3, std::set<std::string>{"technology_three"})));
   EXPECT_EQ(world.GetBuildings().GetTotalGoodSalesValueInWorld(), 1.0F);
   EXPECT_THAT(world.GetCountryRankings().GetGreatPowers(), testing::UnorderedElementsAre(1));
   EXPECT_THAT(world.GetCountryRankings().GetMajorPowers(), testing::UnorderedElementsAre(9));
   EXPECT_EQ(world.GetLocalizations().size(), 1);
   ASSERT_TRUE(world.GetLocalizations().HasLocalization("test_localisation"));
   EXPECT_EQ(world.GetLocalizations().GetLocalizationBlock("test_localisation")->GetLocalization("english"),
       "testing testing 1 2 3");
   EXPECT_THAT(world.GetCharacters(),
       testing::UnorderedElementsAre(testing::Pair(1,
                                         Character({
                                             .id = 1,
                                             .first_name = "Valery",
                                             .last_name = "Konev",
                                             .culture_id = 0,
                                             .culture = "welsh",
                                             .ig_id = 1,
                                             .roles = {"politician"},
                                             .ideology = "ideology_abolitionist",
                                             .traits = {"basic_political_operator", "imperious"},
                                         })),
           testing::Pair(2,
               Character({
                   .id = 2,
                   .first_name = "Malobe",
                   .last_name = "Mande",
                   .culture_id = 0,
                   .culture = "welsh",
                   .ig_id = 2,
                   .roles = {"general"},
                   .rank = 2,
                   .formation_id = 1234,
               })),
           testing::Pair(4,
               Character({
                   .id = 4,
                   .first_name = "Allois",
                   .last_name = "Ullrich",
                   .culture_id = 1,
                   .culture = "scottish",
                   .ig_id = 1,
                   .roles = {"agitator"},
                   .origin_tag = "TWO",
                   .origin_country_id = 3,
               })),
           testing::Pair(5,
               Character({
                   .id = 5,
                   .first_name = "Petru",
                   .last_name = "Mergulet",
                   .culture_id = 1,
                   .culture = "scottish",
                   .ig_id = 3,
                   .roles = {"general", "politician"},
                   .rank = 1,
                   .formation_id = 1234,
               })),
           testing::Pair(6,
               Character({
                   .id = 6,
                   .first_name = "Martin",
                   .last_name = "Cassel",
                   .culture_id = 0,
                   .culture = "welsh",
                   .roles = {"agitator"},
                   .origin_tag = "TAG",
                   .origin_country_id = 1,
               }))));
   EXPECT_THAT(world.GetInterestGroups(),
       testing::UnorderedElementsAre(testing::Pair(1, InterestGroup("ig_devout", 1, 1, 0.34881F, true, {})),
           testing::Pair(2, InterestGroup("ig_landowners", 1, 2, 0.15406F, false, {})),
           testing::Pair(3, InterestGroup("ig_devout", 3, 5, 0.34652F, true, {}))));
   EXPECT_EQ(world.GetPlaythroughId(), 2311);
}


TEST(Vic3worldWorldVic3worldimporter, ConversionDateIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto _ = ImportWorld(
       configuration::Configuration{
           .vic3_directory = "test_files/vic3_world/world",
           .vic3_steam_mod_path = "test_files/vic3_world/documents/mod",
           .vic3_mod_path = "test_files/vic3_world/documents/mod",
           .save_game = "test_files/vic3_world/world/test_save.vic3",
       },
       commonItems::ConverterVersion());

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([INFO] Converting at 1923.6.10.)"));
}


TEST(Vic3worldWorldVic3worldimporter, StateWithInvalidOwnerIsLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto _ = ImportWorld(
       configuration::Configuration{
           .vic3_directory = "test_files/vic3_world/world",
           .vic3_steam_mod_path = "test_files/vic3_world/documents/mod",
           .vic3_mod_path = "test_files/vic3_world/documents/mod",
           .save_game = "test_files/vic3_world/world/test_save.vic3",
       },
       commonItems::ConverterVersion());

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] State 2 had an owner with no definition.)"));
}


TEST(Vic3worldWorldVic3worldimporter, ModsInSaveAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto _ = ImportWorld(
       configuration::Configuration{
           .vic3_directory = "test_files/vic3_world/world",
           .vic3_steam_mod_path = "test_files/vic3_world/workshop/529340",
           .vic3_mod_path = "test_files/vic3_world/documents/mod",
           .save_game = "test_files/vic3_world/world/test_save.vic3",
       },
       commonItems::ConverterVersion());

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(),
       testing::HasSubstr(
           "[INFO] \t\t->> Found potentially useful [Test Mod]: test_files/vic3_world/documents/mod/test_mod/"));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[INFO] \t\t->> Found potentially useful [Test Mod Two]: "
                          "test_files/vic3_world/workshop/529340/test_mod_two/"));
}

TEST(Vic3worldWorldVic3worldimporter, PactsBecomeSubjectsAndOverlords)
{
   const auto world = ImportWorld(
       configuration::Configuration{
           .vic3_directory = "test_files/vic3_world/world",
           .vic3_steam_mod_path = "test_files/vic3_world/documents/mod",
           .vic3_mod_path = "test_files/vic3_world/documents/mod",
           .save_game = "test_files/vic3_world/world/test_save.vic3",
       },
       commonItems::ConverterVersion());
   Country v1 = world.GetCountries().at(1);
   Country v3 = world.GetCountries().at(3);
   EXPECT_EQ(v1.GetPuppets(), std::set<int>({3}));
   EXPECT_EQ(v1.GetOverlord(), std::nullopt);
   EXPECT_EQ(v3.GetPuppets().size(), 0);
   EXPECT_EQ(v3.GetOverlord(), std::make_optional(1));
}


}  // namespace vic3