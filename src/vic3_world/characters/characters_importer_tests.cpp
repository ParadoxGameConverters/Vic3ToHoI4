#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/characters/characters_importer.h"

namespace vic3
{


TEST(Vic3WorldCharactersCharactersImporter, NoCharactersOnEmptyInput)
{
   std::stringstream input;
   const std::map<int, vic3::Character> characters = ImportCharacters(input);

   EXPECT_TRUE(characters.empty());
}


TEST(Vic3WorldCharactersCharactersImporter, CharactersCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tfirst_name = \"Cabdi\"\n";
   input << "\tlast_name = \"Wala\"\n";
   input << "\tculture = 1\n";
   input << "\trole = politician\n";
   input << "\trole = agitator\n";
   input << "\tideology = \"ideology_0\"\n";
   input << "\trank = commander_rank_5\n";
   input << "\ttraits = { \"trait_0\" \"trait_1\" }\n";
   input << "}\n";
   input << "1 = none\n";
   input << "2 ={ \n";
   input << "\tfirst_name = \"Hswe\"\n";
   input << "\tlast_name = \"Hmu\"\n";
   input << "\tculture = 2\n";
   input << "\trole = general\n";
   input << "\tideology = \"ideology_1\"\n";
   input << "\trank = commander_rank_2\n";
   input << "\ttraits = { \"trait_2\" \"trait_3\" }\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   const std::map<int, vic3::Character> characters = ImportCharacters(input);

   EXPECT_THAT(characters,
       testing::UnorderedElementsAre(
           testing::Pair(0,
               Character{0, "Cabdi", "Wala", 1, {"politician", "agitator"}, 5, "ideology_0", {"trait_0", "trait_1"}}),
           testing::Pair(1, Character{1, "Hswe", "Hmu", 2, {"general"}, 2, "ideology_1", {"trait_2", "trait_3"}})));
}


TEST(Vic3WorldCharactersCharactersImporter, CharactersSetAsNoneAreSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tfirst_name = \"Cabdi\"\n";
   input << "\tlast_name = \"Wala\"\n";
   input << "\tculture = 1\n";
   input << "\trole = politician\n";
   input << "\trole = agitator\n";
   input << "\tideology = \"ideology_0\"\n";
   input << "\trank = commander_rank_5\n";
   input << "\ttraits = { \"trait_0\" \"trait_1\" }\n";
   input << "}\n";
   input << "1 = none\n";
   input << "2 ={ \n";
   input << "\tfirst_name = \"Hswe\"\n";
   input << "\tlast_name = \"Hmu\"\n";
   input << "\tculture = 2\n";
   input << "\trole = general\n";
   input << "\tideology = \"ideology_1\"\n";
   input << "\trank = commander_rank_2\n";
   input << "\ttraits = { \"trait_2\" \"trait_3\" }\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";


   const std::map<int, vic3::Character> characters = ImportCharacters(input);

   EXPECT_TRUE(characters.empty());
}


TEST(Vic3WorldCharactersCharactersImporter, CharactersImportsAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tfirst_name = \"Cabdi\"\n";
   input << "\tlast_name = \"Wala\"\n";
   input << "\tculture = 1\n";
   input << "\trole = politician\n";
   input << "\trole = agitator\n";
   input << "\tideology = \"ideology_0\"\n";
   input << "\trank = commander_rank_5\n";
   input << "\ttraits = { \"trait_0\" \"trait_1\" }\n";
   input << "}\n";
   input << "1 = none\n";
   input << "2 ={ \n";
   input << "\tfirst_name = \"Hswe\"\n";
   input << "\tlast_name = \"Hmu\"\n";
   input << "\tculture = 2\n";
   input << "\trole = general\n";
   input << "\tideology = \"ideology_1\"\n";
   input << "\trank = commander_rank_2\n";
   input << "\ttraits = { \"trait_2\" \"trait_3\" }\n";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const std::map<int, vic3::Character> _ = ImportCharacters(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tImported 2 living characters."));
}
}  // namespace vic3