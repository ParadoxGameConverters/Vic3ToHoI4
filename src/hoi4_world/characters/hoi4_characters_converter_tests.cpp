#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"



namespace hoi4
{

TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_CharactersAreConverted)
{
}
TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_PortraitsAreAssigned)
{
}
TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_CouncilsAreCreated)
{
}
TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_PrimeMinistersAreFound)
{
}
TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_OrderIsPreservedOnSamePlaythrough)
{
   // Setup Expected

   constexpr int playthrough_id = 10;
   // Setup Input
   // AssignPortraits(playthrough_id = 10)
   // Tests against expected
   // AssignPortraits(playthrough_id = 10) // Again
   // Test against the same expected, nothing is different
}
TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_OrderIsChangedOnDifferentPlaythrough)
{
   // Setup Expected

   constexpr int playthrough_id_1 = 10;
   constexpr int playthrough_id_2 = 11;
   // Setup Input
   // AssignPortraits(playthrough_id = 10)
   // Tests against expected
   // AssignPortraits(playthrough_id = 11) // Again
   // Test against the different expected, order should be different
}
TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_PreferUnusedPortraitsBetweenCultures)
{
   // Have ~12 cultures using the same ~3 portraits in the same input order for 1 character each.
   // Portrait usage will be [4 4 4] instead of [12 0 0]
}
TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_NewlyGeneratedCharactersDontCollideWithExisting)
{
   // Have a source character with base genID:1000.
   // Input council laws, see that newly generated character resolves collision
   // SrcCharacter intact, new council with ID 1001, Next genID listed as 1002
}

}  // namespace hoi4