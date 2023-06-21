#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/vic3_world/characters/character_importer.h"

namespace vic3
{


TEST(Vic3WorldCharactersCharacterImporter, DefaultsDefaultToDefault)
{
   CharacterImporter character_importer;

   std::stringstream input;
   const auto character = character_importer.ImportCharacter(0, input);

   EXPECT_EQ(character, Character({}));
}


TEST(Vic3WorldCharactersCharacterImporter, CharacterCanBeImported)
{
   CharacterImporter character_importer;

   std::stringstream input;
   input << "={\n";
   input << "\tfirst_name = \"Cabdi\"\n";
   input << "\tlast_name = \"Wala\"\n";
   input << "\tculture = 1\n";
   input << "\trole = politician\n";
   input << "\trole = agitator\n";
   input << "\tideology = \"ideology_0\"\n";
   input << "\trank = commander_rank_5\n";
   input << "\ttraits = { \"trait_0\" \"trait_1\" }\n";
   input << "}\n";

   const auto character = character_importer.ImportCharacter(1, input);

   EXPECT_EQ(character,
       Character({.id = 1,
           .first_name = "Cabdi",
           .last_name = "Wala",
           .culture_id = 1,
           .roles = {"politician", "agitator"},
           .rank = 5,
           .ideology = "ideology_0",
           .traits = {"trait_0", "trait_1"}}));
}


TEST(Vic3WorldCharactersCharacterImporter, RulerCommanderIsFirstRank)
{
   CharacterImporter character_importer;

   std::stringstream input;
   input << "={\n";
   input << "\trank = commander_rank_ruler\n";
   input << "}\n";

   const auto character = character_importer.ImportCharacter(1, input);

   EXPECT_EQ(character, Character({.id = 1, .rank = 1}));
}


TEST(Vic3WorldCharactersCharacterImporter, InvalidRankIsLogged)
{
   CharacterImporter character_importer;

   std::stringstream input;
   input << "={\n";
   input << "\trank = commander_rank_modded\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto character = character_importer.ImportCharacter(1, input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_EQ(character, Character({.id = 1, .rank = 0}));
   EXPECT_THAT(log.str(),
       testing::HasSubstr("[WARNING] Failed to read rank: commander_rank_modded."));
}
}  // namespace vic3