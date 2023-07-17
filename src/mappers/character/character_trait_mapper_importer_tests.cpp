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
       AdmiralTraitMapping({"naval_trait_0", "naval_trait_1"}, 2, 2, 2, 2));
   EXPECT_EQ(character_trait_mapper.GetGeneralMappedData({"charismatic"}, true),
       GeneralTraitMapping({"land_trait_0", "land_trait_1"}, {"charismatic"}, 2, 2, 2, 2));
   EXPECT_EQ(character_trait_mapper.GetSpyMappedTraits({"bandit"}), testing::UnorderedElementsAre("operative_tough"));
   EXPECT_EQ(character_trait_mapper.GetSpyMappedTraits({"persistent"}),
       testing::UnorderedElementsAre("operative_tough"));
}


}  // namespace mappers