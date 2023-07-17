#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/mappers/character/character_trait_mapper_importer.h"



namespace mappers
{

TEST(MappersCharacterCharactertraitmapperimporter, DISABLED_TBD)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");

   EXPECT_EQ(character_trait_mapper.GetAdmiralMappedData({"bandit"}),
       hoi4::Admiral({"naval_trait_0", "naval_trait_1"}, 1, 1, 1, 1));
   EXPECT_EQ(character_trait_mapper.GetGeneralMappedData({"charismatic"}, true),
       hoi4::General({"land_trait_0", "land_trait_1"}, {"charismatic"}, 1, 1, 1, 1));
   EXPECT_EQ(character_trait_mapper.GetSpyMappedTraits({"bandit"}), std::set{"operative_tough"});
   EXPECT_EQ(character_trait_mapper.GetSpyMappedTraits({"persistent"}), std::set{"operative_tough"});
}


}  // namespace mappers