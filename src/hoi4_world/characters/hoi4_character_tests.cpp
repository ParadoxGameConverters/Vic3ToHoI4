#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/hoi4_world/characters/hoi4_character.h"


namespace hoi4
{

TEST(Hoi4worldCharactersCharacter, PortraitAliasCanBeSet)
{
   Character character({});
   EXPECT_TRUE(character.GetPortraitAlias().empty());
   character.SetPortraitAlias("GFX_test");
   EXPECT_EQ(character.GetPortraitAlias(), "GFX_test");
}

}  // namespace hoi4