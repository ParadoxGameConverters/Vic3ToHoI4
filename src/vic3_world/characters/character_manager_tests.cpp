#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/characters/character_manager.h"

namespace vic3
{


TEST(Vic3WorldCharacterManagerImporter, DefaultsDefaultToDefault)
{
   std::stringstream input;

   CharacterManager character_manager(input);

   EXPECT_TRUE(character_manager.GetCharacters().empty());
   EXPECT_TRUE(character_manager.GetCountryCharacterMap().empty());
}


TEST(Vic3WorldCharacterManagerImporter, CharactersCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "database={\n";
   input << "1 = {\n";
   input << "first_name = \"Cabdi\"\n";
   input << "last_name = \"Wala\"\n";
   input << "culture = 1\n";
   input << "role = politician\n";
   input << "role = agitator\n";
   input << "ideology = \"ideology_0\"\n";
   input << "rank = commander_rank_5\n";
   input << "traits = { \"trait_0\" \"trait_1\" }\n";
   input << "}\n";
   input << "2 = none\n";
   input << "3 ={ \n";
   input << "first_name = \"Hswe\"\n";
   input << "last_name = \"Hmu\"\n";
   input << "culture = 2\n";
   input << "role = general\n";
   input << "ideology = \"ideology_1\"\n";
   input << "rank = commander_rank_2\n";
   input << "traits = { \"trait_2\" \"trait_3\" }\n";
   input << "}\n";
   input << "}\n";
   input << "}\n";

   CharacterManager character_manager(input);

   EXPECT_THAT(character_manager.GetCharacters(),
       testing::UnorderedElementsAre(testing::Pair(1,
                                         Character({.id = 1,
                                             .first_name = "Cabdi",
                                             .last_name = "Wala",
                                             .culture_id = 1,
                                             .roles = {"politician", "agitator"},
                                             .rank = 5,
                                             .ideology = "ideology_0",
                                             .traits = {"trait_0", "trait_1"}})),
           testing::Pair(3,
               Character({.id = 3,
                   .first_name = "Hswe",
                   .last_name = "Hmu",
                   .culture_id = 2,
                   .roles = {"general"},
                   .rank = 2,
                   .ideology = "ideology_1",
                   .traits = {"trait_2", "trait_3"}}))));
}


TEST(Vic3WorldCharacterManagerImporter, CountryCharacterMapCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "country_character_map={\n";
   input << "max=535\n";
   input << "1 = { 1 }\n";
   input << "2 = { 3 }\n";
   input << "}\n";
   input << "}\n";

   CharacterManager character_manager(input);

   EXPECT_THAT(character_manager.GetCountryCharacterMap(),
       testing::UnorderedElementsAre(testing::Pair(1, std::vector{1}), testing::Pair(2, std::vector{3})));
}


TEST(Vic3WorldCharacterManagerImporter, IgsAreAssigned)
{
   std::stringstream input;
   input << "={\n";
   input << "database={\n";
   input << "1 = {\n";
   input << "}\n";
   input << "3 ={ \n";
   input << "}\n";
   input << "}\n";
   input << "character_ig_map={\n";
   input << "max=6729\n";
   input << "1 = 1\n";
   input << "3 = 2\n";
   input << "}\n";

   CharacterManager character_manager(input);

   EXPECT_THAT(character_manager.GetCharacters(),
       testing::UnorderedElementsAre(testing::Pair(1, Character({.id = 1, .ig_id = 1})),
           testing::Pair(3, Character({.id = 3, .ig_id = 2}))));
}


TEST(Vic3WorldCharacterManagerImporter, OriginTagsAreAssignedForExiles)
{
   std::stringstream input;
   input << "={\n";
   input << "database={\n";
   input << "1 = {\n";
   input << "role = agitator\n";
   input << "}\n";
   input << "3 ={ \n";
   input << "}\n";
   input << "}\n";
   input << "exile_country_map={\n";
   input << "max=6552\n";
   input << "1 = TAG\n";
   input << "}\n";
   input << "}\n";

   CharacterManager character_manager(input);

   EXPECT_THAT(character_manager.GetCharacters(),
       testing::UnorderedElementsAre(testing::Pair(1, Character({.id = 1, .roles = {"agitator"}, .origin_tag = "TAG"})),
           testing::Pair(3, Character({.id = 3}))));
}


TEST(Vic3WorldCharacterManagerImporter, WanderingAgitatorsAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "exile_pool={\n";
   input << "max=6552\n";
   input << "1 = { interest_group = ig_example }\n";
   input << "2 = { interest_group = ig_example }\n";
   input << "}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   CharacterManager _(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tFound 2 wandering agitators.\n"));
}


TEST(Vic3WorldCharacterManagerImporter, IgsMatchesAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "database={\n";
   input << "1 = {\n";
   input << "}\n";
   input << "3 ={ \n";
   input << "}\n";
   input << "}\n";
   input << "character_ig_map={\n";
   input << "max=6729\n";
   input << "1 = 1\n";
   input << "3 = 2\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   CharacterManager _(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tFound IGs for 2 characters.\n"));
}


TEST(Vic3WorldCharacterManagerImporter, MissingIgMatchesAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "database={\n";
   input << "1 = {\n";
   input << "}\n";
   input << "2 = none\n";
   input << "3 ={ \n";
   input << "first_name = \"Hswe\"\n";
   input << "last_name = \"Hmu\"\n";
   input << "}\n";
   input << "}\n";
   input << "character_ig_map={\n";
   input << "max=6729\n";
   input << "1 = 1\n";
   input << "}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   CharacterManager _(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Character Hswe Hmu with ID: 3 has no IG.\n"));
}


TEST(Vic3WorldCharacterManagerImporter, WanderingAgitatorsAreNotLoggedAsMissingIgs)
{
   std::stringstream input;
   input << "={\n";
   input << "database={\n";
   input << "1 = {\n";
   input << "}\n";
   input << "2 = {\n";  // Homeless agitator
   input << "first_name = \"No\"\n";
   input << "last_name = \"Interest Group\"\n";
   input << "}\n";
   input << "3 ={ \n";
   input << "first_name = \"Hswe\"\n";
   input << "last_name = \"Hmu\"\n";
   input << "}\n";
   input << "}\n";
   input << "character_ig_map={\n";
   input << "max=6729\n";
   input << "1 = 1\n";
   input << "}\n";
   input << "exile_pool={\n";
   input << "max=6552\n";
   input << "2 = { ig_example }\n";
   input << "}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   CharacterManager _(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Character Hswe Hmu with ID: 3 has no IG.\n"));
   EXPECT_THAT(log.str(),
       testing::Not(testing::HasSubstr("[WARNING] Character No Interest Group with ID: 2 has no IG.\n")));
}


TEST(Vic3WorldCharacterManagerImporter, MissingOriginTagMatchesAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "database={\n";
   input << "1 = {\n";
   input << "first_name = \"Missing\"\n";
   input << "last_name = \"Home\"\n";
   input << "role = agitator\n";
   input << "}\n";
   input << "3 ={ \n";
   input << "}\n";
   input << "}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   CharacterManager _(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Agitator Missing Home with ID: 1 has no home country.\n"));
}


TEST(Vic3WorldCharacterManagerImporter, OriginTagsMatchesAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "database={\n";
   input << "1 = {\n";
   input << "role = agitator\n";
   input << "}\n";
   input << "3 ={ \n";
   input << "}\n";
   input << "}\n";
   input << "exile_country_map={\n";
   input << "max=6552\n";
   input << "1 = TAG\n";
   input << "}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   CharacterManager _(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tFound home countries of 1 exiles.\n"));
}
}  // namespace vic3