#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <sstream>

#include "src/vic3_world/characters/vic3_character_importer.h"

namespace vic3
{


TEST(Vic3worldCharactersVic3characterimporter, DefaultsDefaultToDefault)
{
   CharacterImporter character_importer;

   std::stringstream input;
   const auto character = character_importer.ImportCharacter(0, input);
   EXPECT_EQ(character,
       Character({.id = 0,
           .first_name = "",
           .last_name = "",
           .culture_id = 0,
           .culture = "",
           .is_female = false,
           .ig_id = 0,
           .roles = std::set<std::string>{},
           .rank = 0,
           .ideology = "",
           .traits = std::set<std::string>{},
           .origin_tag = "",
           .origin_country_id = std::nullopt,
           .formation_id = std::nullopt}));
}


TEST(Vic3worldCharactersVic3characterimporter, CharacterCanBeImported)
{
   CharacterImporter character_importer;

   std::stringstream input;
   input << "={\n";
   input << "\tfirst_name = \"Cabdi\"\n";
   input << "\tlast_name = \"Wala\"\n";
   input << "\tculture = 1\n";
   input << "\tis_female = yes\n";
   input << "\trole = politician\n";
   input << "\trole = agitator\n";
   input << "\tideology = \"ideology_0\"\n";
   input << "\trank = commander_rank_5\n";
   input << "\ttraits = { \"trait_0\" \"trait_1\" }\n";
   input << "\tformation = 238\n";
   input << "}\n";

   const auto character = character_importer.ImportCharacter(1, input);

   EXPECT_EQ(character,
       Character({.id = 1,
           .first_name = "Cabdi",
           .last_name = "Wala",
           .culture_id = 1,
           .is_female = true,
           .roles = {"politician", "agitator"},
           .rank = 5,
           .ideology = "ideology_0",
           .traits = {"trait_0", "trait_1"},
           .formation_id = 238}));
}


TEST(Vic3worldCharactersVic3characterimporter, RulerCommanderIsFirstRank)
{
   CharacterImporter character_importer;

   std::stringstream input;
   input << "={\n";
   input << "\trank = commander_rank_ruler\n";
   input << "}\n";

   const auto character = character_importer.ImportCharacter(1, input);

   EXPECT_EQ(character, Character({.id = 1, .rank = 1}));
}


TEST(Vic3worldCharactersVic3characterimporter, InvalidRankIsLogged)
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
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Failed to read rank: commander_rank_modded."));
}


TEST(Vic3worldCharactersVic3characterimporter, CultureCanBeSet)
{
   CharacterImporter character_importer;

   std::stringstream input;
   auto character = character_importer.ImportCharacter(1, input);

   character.SetCulture("scottish");

   EXPECT_EQ(character, Character({.id = 1, .culture = "scottish"}));
}


TEST(Vic3worldCharactersVic3characterimporter, HomeTagCanBeSet)
{
   CharacterImporter character_importer;

   std::stringstream input;
   auto character = character_importer.ImportCharacter(1, input);

   character.SetHomeTag("TAG");

   EXPECT_EQ(character, Character({.id = 1, .origin_tag = "TAG"}));
}


TEST(Vic3worldCharactersVic3characterimporter, IgIdCanBeSet)
{
   CharacterImporter character_importer;

   std::stringstream input;
   auto character = character_importer.ImportCharacter(1, input);

   character.SetIgId(1);

   EXPECT_EQ(character, Character({.id = 1, .ig_id = 1}));
}


TEST(Vic3worldCharactersVic3characterimporter, CommanderCanBeSet)
{
   CharacterImporter character_importer;

   std::stringstream input;
   auto character = character_importer.ImportCharacter(1, input);

   character.SetCommander();

   EXPECT_EQ(character, Character({.id = 1, .formation_id = 0}));
}


}  // namespace vic3