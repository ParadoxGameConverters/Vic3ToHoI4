#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/characters/vic3_country_character_map_importer.h"

namespace vic3
{


TEST(Vic3WorldCharactersVic3CountryCharacterMapImporter, NoInputIsEmpty)
{
   std::stringstream input;
   const auto country_character_map = ImportCountryCharacterMap(input);

   EXPECT_TRUE(country_character_map.empty());
}


TEST(Vic3WorldCharactersVic3CountryCharacterMapImporter, CountryCharacterMapCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "max=535\n";
   input << "1 = { 1 2 3 }\n";
   input << "2 = { 4 5 6 }\n";
   input << "}\n";

   const auto country_character_map = ImportCountryCharacterMap(input);

   EXPECT_THAT(country_character_map,
       testing::UnorderedElementsAre(testing::Pair(1, std::vector{1, 2, 3}), testing::Pair(2, std::vector{4, 5, 6})));
}


TEST(Vic3WorldCharactersVic3CountryCharacterMapImporter, NumberOfMatchesAreLogged)
{
   std::stringstream input;
   input << "={\n";
   input << "max=535\n";
   input << "1 = { 1 2 3 }\n";
   input << "2 = { 4 5 }\n";
   input << "}\n";

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   ImportCountryCharacterMap(input);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr("[INFO] \tMatched 2 countries to 5 characters.\n"));
}
}  // namespace vic3