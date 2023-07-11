#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
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


TEST(Hoi4worldCharactersCharacter, StaticIdTrackerCanIncrement)
{
   // Other Hoi4world tests impact this static value
   // EXPECT_EQ(Character::GetGenId(), 1000);
   // Character::IncrementGenId();
   // EXPECT_EQ(Character::GetGenId(), 1001);
   const int current_id = Character::GetGenId();
   Character::IncrementGenId();
   EXPECT_EQ(Character::GetGenId(), current_id + 1);
}

}  // namespace hoi4