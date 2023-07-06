#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "hoi4_character_converter.h"



namespace hoi4
{


TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_AdmiralsCanBeConverted)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_GeneralsCanBeConverted)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_AdvisorsCanBeConverted)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_CountryLeadersCanBeConverted)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_SpiesCanBeConverted)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_AdmiralPortraitIsEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_GeneralPortraitIsEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_AdvisorIdeologyPortraitsAreIsEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_CountryLeaderIdeologyPortraitsAreEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_SpyPortraitIsEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_LeaderMilitaryEnqueuesMilitaryPortraits)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_AdvisorMilitaryEnqueuesMilitaryPortraits)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_CouncilPortraitsAreEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_CouncilMonarchPortraitsAreEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_CouncilFemaleMonarchPortraitsAreEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_FemaleSpyPortraitsAreEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_AllMiscFemalePortraitsAreEnqueued)
{
}
TEST(Hoi4worldCharactersHoi4characterconverter, DISABLED_PoorlyDefinedCharactersAreLogged)
{
}


TEST(Hoi4worldCharactersHoi4characterconverter, RoyalsAreMonarchs)
{
   EXPECT_TRUE(HasMonarchs("head_of_state", {"law_monarchy"}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, PrimeMinistersArentMonarchs)
{
   EXPECT_FALSE(HasMonarchs("prime_minister", {"law_monarchy"}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, PresidentsArentMonarchs)
{
   EXPECT_FALSE(HasMonarchs("head_of_state", {}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, CouncilsArentMonarchs)
{
   EXPECT_FALSE(HasMonarchs("council", {"law_monarchy"}));
}

}  // namespace hoi4