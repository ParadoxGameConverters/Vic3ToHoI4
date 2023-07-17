#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/character/character_trait_mapper.h"
#include "src/mappers/character/character_trait_mapper_importer.h"



namespace mappers
{

TEST(MappersCharacterCharactertraitmapper, DISABLED_TBD)
{
   const CharacterTraitMapper character_trait_mapper =
       ImportCharacterTraitMapper("test_files/configurables/character_traits.txt");

   //    EXPECT_EQ(character_trait_mapper.GetGeneralTraitRules() , TBD);
   //    EXPECT_EQ(character_trait_mapper.GetAdmiralTraitRules() , TBD);
   //    EXPECT_EQ(character_trait_mapper.GetSpyTraitRules() , TBD);
}


}  // namespace mappers