#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/characters/country_character_map_importer.h"

namespace vic3
{


TEST(Vic3WorldCharactersCountryCharacterMapImporter, NoInputIsEmpty)
{
   std::stringstream input;
   const auto country_character_map = ImportCountryCharacterMap(input);

   EXPECT_TRUE(country_character_map.empty());
}


TEST(Vic3WorldCharactersCountryCharacterMapImporter, CountryCharacterMapCanBeImported)
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
}  // namespace vic3