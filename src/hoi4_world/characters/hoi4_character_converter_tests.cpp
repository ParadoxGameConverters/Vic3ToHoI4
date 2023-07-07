#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "hoi4_character_converter.h"



namespace hoi4
{
namespace
{
std::map<std::string, mappers::CultureQueue> dummy_queue;
}

TEST(Hoi4worldCharactersHoi4characterconverter, AdmiralsCanBeConverted)
{
   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"admiral"},
       .rank = 2,
   });
   const auto expected_data = std::optional<Admiral>({.traits = {}});
   const auto character = ConvertCharacter(source_character, 0, "", "", "", {}, dummy_queue, {});
   EXPECT_EQ(character, Character({.id = 1, .admiral_data = expected_data}));
}
TEST(Hoi4worldCharactersHoi4characterconverter, GeneralsCanBeConverted)
{
   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"general"},
       .rank = 2,
   });
   const auto expected_data = std::optional<General>({.traits = {}});
   const auto character = ConvertCharacter(source_character, 0, "", "", "", {}, dummy_queue, {});
   EXPECT_EQ(character, Character({.id = 1, .general_data = expected_data}));
}
TEST(Hoi4worldCharactersHoi4characterconverter, AdvisorsCanBeConverted)
{
   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"politician"},
   });
   const auto expected_data = std::optional<Advisor>({.traits = {}, .slot = "political_advisor"});
   const auto character = ConvertCharacter(source_character, 0, "", "", "", {}, dummy_queue, {});
   EXPECT_EQ(character, Character({.id = 1, .advisor_data = expected_data}));
}
TEST(Hoi4worldCharactersHoi4characterconverter, CountryLeadersCanBeConverted)
{
   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"politician"},
   });
   const auto expected_data = std::optional<Leader>({.sub_ideology = "test_ideology"});
   const auto character = ConvertCharacter(source_character, 1, "", "", "test_ideology", {}, dummy_queue, {});
   EXPECT_EQ(character, Character({.id = 1, .leader_data = expected_data}));
}
TEST(Hoi4worldCharactersHoi4characterconverter, SpiesCanBeConverted)
{
   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"agitator"},
       .origin_country_id = 2,
   });
   const auto expected_data = std::optional<Spy>({.nationalities = {"TAG", "TWO"}});
   const auto character =
       ConvertCharacter(source_character, 0, "", "TAG", "", {}, dummy_queue, mappers::CountryMapper({{2, "TWO"}}));
   EXPECT_EQ(character, Character({.id = 1, .spy_data = expected_data}));
}
TEST(Hoi4worldCharactersHoi4characterconverter, GenericCharacterDataCanBeConverted)
{
   const auto source_character = vic3::Character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Woman",
       .is_female = true,
   });
   const auto character = ConvertCharacter(source_character, 0, "", "", "", {}, dummy_queue, {});
   EXPECT_EQ(character, Character({.id = 1, .first_name = "Test", .last_name = "Woman", .is_female = true}));
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
TEST(Hoi4worldCharactersHoi4characterconverter, PoorlyDefinedCharactersAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto source_character = vic3::Character({
       .id = 1,
       .first_name = "A",
       .last_name = "Man",
   });
   const auto character = ConvertCharacter(source_character, 0, "", "", "", {}, dummy_queue, {});

   std::cout.rdbuf(cout_buffer);

   EXPECT_EQ(character, Character({.id = 1, .first_name = "A", .last_name = "Man"}));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(R"([WARNING] Unable to find a portrait category for character with ID: 1. A Man.)"));
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