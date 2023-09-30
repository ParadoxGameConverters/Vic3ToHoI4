#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "hoi4_character_converter.h"
#include "src/mappers/character/character_trait_mapper_importer.h"


namespace hoi4
{

TEST(Hoi4worldCharactersHoi4characterconverter, AdmiralsCanBeConverted)
{
   std::map<std::string, mappers::CultureQueue> dummy_queue;
   const auto character_trait_mapper = mappers::ImportCharacterTraitMapper("configurables/character_traits.txt");

   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"admiral"},
       .rank = 2,
       .traits = {"bandit"},
   });
   const auto expected_data = std::optional<Admiral>({
       .traits = {"naval_trait_0", "naval_trait_1"},
       .attack = 2,
       .defense = 2,
       .maneuvering = 2,
       .coordination = 2,
   });
   const auto character = ConvertCharacter(source_character,
       0,
       {.admiral_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       character_trait_mapper,
       dummy_queue);
   EXPECT_EQ(character, Character({.id = 1, .admiral_data = expected_data}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, GeneralsCanBeConverted)
{
   std::map<std::string, mappers::CultureQueue> dummy_queue;
   const auto character_trait_mapper = mappers::ImportCharacterTraitMapper("configurables/character_traits.txt");

   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"general"},
       .rank = 2,
       .traits = {"charismatic"},
   });
   const auto expected_data = std::optional<General>({
       .traits = {"land_trait_0", "land_trait_1"},
       .attack = 2,
       .defense = 2,
       .planning = 2,
       .logistics = 2,
   });
   const auto character = ConvertCharacter(source_character,
       0,
       {.general_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       character_trait_mapper,
       dummy_queue);
   EXPECT_EQ(character, Character({.id = 1, .general_data = expected_data}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, FieldMarshalsCanBeConverted)
{
   std::map<std::string, mappers::CultureQueue> dummy_queue;
   const auto character_trait_mapper = mappers::ImportCharacterTraitMapper("configurables/character_traits.txt");

   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"general"},
       .rank = 2,
       .traits = {"charismatic"},
   });
   const auto expected_data = std::optional<General>({
       .traits = {"land_trait_0", "land_trait_1", "charismatic"},
       .is_field_marshal = true,
       .attack = 2,
       .defense = 2,
       .planning = 2,
       .logistics = 2,
   });
   const auto character = ConvertCharacter(source_character,
       0,
       {.field_marshal_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       character_trait_mapper,
       dummy_queue);
   EXPECT_EQ(character, Character({.id = 1, .general_data = expected_data}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, AdvisorsCanBeConverted)
{
   std::map<std::string, mappers::CultureQueue> dummy_queue;
   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"politician"},
   });
   const auto expected_data = std::optional<Advisor>({.traits = {}, .slot = "political_advisor"});
   const auto character = ConvertCharacter(source_character,
       0,
       {.advisor_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       dummy_queue);
   EXPECT_EQ(character, Character({.id = 1, .advisor_data = expected_data}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, CountryLeadersCanBeConverted)
{
   std::map<std::string, mappers::CultureQueue> dummy_queue;
   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"politician"},
   });
   const auto expected_data = std::optional<Leader>({.sub_ideology = "test_ideology"});
   const auto character = ConvertCharacter(source_character,
       1,
       {},
       "",
       "",
       "",
       "test_ideology",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       dummy_queue);
   EXPECT_EQ(character, Character({.id = 1, .leader_data = expected_data}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, SpiesCanBeConverted)
{
   std::map<std::string, mappers::CultureQueue> dummy_queue;
   const auto character_trait_mapper = mappers::ImportCharacterTraitMapper("configurables/character_traits.txt");

   const auto source_character = vic3::Character({
       .id = 1,
       .roles = {"agitator"},
       .traits = {"bandit"},
       .origin_country_id = 2,
   });
   const auto expected_data = std::optional<Spy>({
       .traits = {"operative_tough"},
       .nationalities = {"TAG", "TWO"},
   });
   const auto character = ConvertCharacter(source_character,
       0,
       {.spy_ids = {1}},
       "",
       "TAG",
       "",
       "",
       {},
       mappers::CountryMapper({{2, "TWO"}}),
       character_trait_mapper,
       dummy_queue);
   EXPECT_EQ(character, Character({.id = 1, .spy_data = expected_data}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, GenericCharacterDataCanBeConverted)
{
   std::map<std::string, mappers::CultureQueue> dummy_queue;
   const auto source_character = vic3::Character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Woman",
       .is_female = true,
   });
   const auto character = ConvertCharacter(source_character,
       0,
       {},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       dummy_queue);
   EXPECT_EQ(character, Character({.id = 1, .first_name = "Test", .last_name = "Woman", .is_female = true}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, AdmiralPortraitIsEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"admiral"},
       .rank = 2,
   });
   ConvertCharacter(source_character,
       0,
       {.admiral_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"navy", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, GeneralPortraitIsEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"general"},
       .rank = 2,
   });
   ConvertCharacter(source_character,
       0,
       {.general_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"army", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, AdvisorIdeologyPortraitsAreEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character_c = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"politician"},
   });
   const auto source_character_d = vic3::Character({
       .id = 2,
       .culture = "culture_2",
       .roles = {"politician"},
   });
   const auto source_character_f = vic3::Character({
       .id = 3,
       .culture = "culture_2",
       .roles = {"politician"},
   });
   const auto source_character_n = vic3::Character({
       .id = 4,
       .culture = "culture_2",
       .roles = {"politician"},
   });

   ConvertCharacter(source_character_c,
       0,
       {.advisor_ids = {1}},
       "",
       "",
       "communism",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   ConvertCharacter(source_character_d,
       0,
       {.advisor_ids = {2}},
       "",
       "",
       "democratic",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   ConvertCharacter(source_character_f,
       0,
       {.advisor_ids = {3}},
       "",
       "",
       "fascism",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   ConvertCharacter(source_character_n,
       0,
       {.advisor_ids = {4}},
       "",
       "",
       "anything_else",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);

   EXPECT_EQ(culture_queues.at("culture_2"),
       mappers::CultureQueue({
           {"advisor_communism", {1}},
           {"advisor_democratic", {2}},
           {"advisor_fascism", {3}},
           {"advisor_neutrality", {4}},
       }));
}


TEST(Hoi4worldCharactersHoi4characterconverter, CountryLeaderIdeologyPortraitsAreEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character_c = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"politician"},
   });
   const auto source_character_d = vic3::Character({
       .id = 2,
       .culture = "culture_2",
       .roles = {"politician"},
   });
   const auto source_character_f = vic3::Character({
       .id = 3,
       .culture = "culture_2",
       .roles = {"politician"},
   });
   const auto source_character_n = vic3::Character({
       .id = 4,
       .culture = "culture_2",
       .roles = {"politician"},
   });

   ConvertCharacter(source_character_c,
       1,
       {},
       "",
       "",
       "communism",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   ConvertCharacter(source_character_d,
       2,
       {},
       "",
       "",
       "democratic",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   ConvertCharacter(source_character_f,
       3,
       {},
       "",
       "",
       "fascism",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   ConvertCharacter(source_character_n,
       4,
       {},
       "",
       "",
       "anything_else",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);

   EXPECT_EQ(culture_queues.at("culture_2"),
       mappers::CultureQueue({
           {"leader_communism", {1}},
           {"leader_democratic", {2}},
           {"leader_fascism", {3}},
           {"leader_neutrality", {4}},
       }));
}


TEST(Hoi4worldCharactersHoi4characterconverter, SpyPortraitIsEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"agitator"},
   });
   ConvertCharacter(source_character,
       0,
       {.spy_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"operative_male", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, FemaleSpyPortraitsAreEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .is_female = true,
       .roles = {"agitator"},
   });
   ConvertCharacter(source_character,
       0,
       {.spy_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"operative_female", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, LeaderMilitaryEnqueuesMilitaryPortraits)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character_g = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"politician", "general"},
   });
   const auto source_character_a = vic3::Character({
       .id = 2,
       .culture = "culture_2",
       .roles = {"politician", "admiral"},
   });
   ConvertCharacter(source_character_g,
       1,
       {.field_marshal_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   ConvertCharacter(source_character_a,
       2,
       {.admiral_ids = {2}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"),
       mappers::CultureQueue({
           {"army", {1}},
           {"navy", {2}},
       }));
}


TEST(Hoi4worldCharactersHoi4characterconverter, AdvisorMilitaryEnqueuesMilitaryPortraits)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character_g = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"politician", "general"},
   });
   const auto source_character_a = vic3::Character({
       .id = 2,
       .culture = "culture_2",
       .roles = {"politician", "admiral"},
   });
   ConvertCharacter(source_character_g,
       0,
       {.general_ids = {1}, .advisor_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   ConvertCharacter(source_character_a,
       0,
       {.admiral_ids = {2}, .advisor_ids = {2}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"),
       mappers::CultureQueue({
           {"army", {1}},
           {"navy", {2}},
       }));
}


TEST(Hoi4worldCharactersHoi4characterconverter, MonarchMilitaryEnqueuesMonarchPortraits)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"politician", "general"},
   });

   ConvertCharacter(source_character,
       1,
       {.field_marshal_ids = {1}},
       "head_of_state",
       "",
       "",
       "",
       {"law_monarchy"},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"monarch_male", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, CouncilPortraitsAreEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character_g = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"politician"},
   });

   ConvertCharacter(source_character_g,
       1,
       {},
       "council",
       "",
       "",
       "",
       {""},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"council", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, MonarchPortraitsAreEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .roles = {"politician"},
   });
   ConvertCharacter(source_character,
       1,
       {},
       "head_of_state",
       "",
       "",
       "",
       {"law_monarchy"},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"monarch_male", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, FemaleMonarchPortraitsAreEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .is_female = true,
       .roles = {"politician"},
   });
   ConvertCharacter(source_character,
       1,
       {},
       "head_of_state",
       "",
       "",
       "",
       {"law_monarchy"},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"monarch_female", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, AllMiscFemalePortraitsAreEnqueued)
{
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto source_character = vic3::Character({
       .id = 1,
       .culture = "culture_2",
       .is_female = true,
       .roles = {"general"},
       .rank = 2,
   });
   ConvertCharacter(source_character,
       0,
       {.general_ids = {1}},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       culture_queues);
   EXPECT_EQ(culture_queues.at("culture_2"), mappers::CultureQueue({{"female_leader", {1}}}));
}


TEST(Hoi4worldCharactersHoi4characterconverter, PoorlyDefinedCharactersAreLogged)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   std::map<std::string, mappers::CultureQueue> dummy_queue;
   const auto source_character = vic3::Character({
       .id = 1,
       .first_name = "A",
       .last_name = "Man",
   });
   const auto character = ConvertCharacter(source_character,
       0,
       {},
       "",
       "",
       "",
       "",
       {},
       {},
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       dummy_queue);

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
