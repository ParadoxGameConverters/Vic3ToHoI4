#include <ranges>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/characters/hoi4_characters_converter.h"
#include "src/mappers/character/character_trait_mapper_importer.h"
#include "src/mappers/character/leader_type_mapper_importer.h"
#include "src/mappers/culture/culture_graphics_mapper_importer.h"
namespace hoi4
{

TEST(Hoi4worldCharactersHoi4charactersconverter, CharactersAreConverted)
{
   const auto country_mapper = mappers::CountryMapper({{3, "TWO"}});

   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");
   const auto character_trait_mapper = mappers::ImportCharacterTraitMapper("configurables/character_traits.txt");


   // General
   const auto character_one = vic3::Character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"general"},
       .rank = 1,
       .traits = {"charismatic"},
   });
   // Commander leader
   const auto character_two = vic3::Character({
       .id = 2,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"politician", "general"},
       .rank = 1,
       .traits = {"charismatic"},
   });
   // Field Marshal
   const auto character_three = vic3::Character({
       .id = 3,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"general"},
       .rank = 2,
       .traits = {"charismatic"},
   });
   // Filler General
   const auto character_four = vic3::Character({
       .id = 4,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"general"},
       .rank = 2,
       .traits = {"charismatic"},
   });
   // Female General
   const auto character_five = vic3::Character({
       .id = 5,
       .first_name = "Test",
       .last_name = "WoMann",
       .is_female = true,
       .roles = {"general", "agitator"},
       .rank = 2,
       .traits = {"charismatic"},
   });
   // Admiral
   const auto character_six = vic3::Character({
       .id = 6,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"admiral"},
       .rank = 2,
       .traits = {"bandit"},
   });
   // Spy
   const auto character_seven = vic3::Character({
       .id = 7,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"agitator"},
       .traits = {"bandit"},
       .origin_country_id = 3,
   });
   // Advisor
   const auto character_eight = vic3::Character({
       .id = 8,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"politician"},
   });
   // General Advisor
   const auto character_nine = vic3::Character({
       .id = 9,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"general", "politician"},
       .rank = 2,
       .traits = {"charismatic"},
   });
   // Admiral Advisor
   const auto character_ten = vic3::Character({
       .id = 10,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"admiral", "politician"},
       .rank = 2,
       .traits = {"bandit"},
   });


   const std::map<int, vic3::Character> source_characters = {
       {1, character_one},
       {2, character_two},
       {3, character_three},
       {4, character_four},
       {5, character_five},
       {6, character_six},
       {7, character_seven},
       {8, character_eight},
       {9, character_nine},
       {10, character_ten},
   };
   ConvertCharacters(source_characters,
       "TAG",
       "",
       "sub_ideology",
       vic3::Country({.head_of_state_id = 2, .character_ids = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}}),
       {},
       leader_type_mapper,
       character_trait_mapper,
       country_mapper,
       characters,
       culture_queues);

   const auto leader_data = std::optional<Leader>({
       .sub_ideology = "sub_ideology",
   });
   const auto field_marshal_data = std::optional<General>({
       .traits = {"land_trait_0", "land_trait_1", "charismatic"},
       .is_field_marshal = true,
       .attack = 2,
       .defense = 2,
       .planning = 2,
       .logistics = 2,
   });
   const auto general_data = std::optional<General>({
       .traits = {"land_trait_0", "land_trait_1"},
       .attack = 2,
       .defense = 2,
       .planning = 2,
       .logistics = 2,
   });
   const auto admiral_data = std::optional<Admiral>({
       .traits = {"naval_trait_0", "naval_trait_1"},
       .attack = 2,
       .defense = 2,
       .maneuvering = 2,
       .coordination = 2,
   });
   const auto advisor_data = std::optional<Advisor>({
       .slot = "political_advisor",
   });
   const auto spy_data = std::optional<Spy>({
       .traits = {"operative_tough"},
       .nationalities = {"TAG", "TWO"},
   });
   const auto expected_character_one = Character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .general_data = general_data,
   });
   const auto expected_character_two = Character({
       .id = 2,
       .first_name = "Test",
       .last_name = "Mann",
       .general_data = field_marshal_data,
       .leader_data = leader_data,
   });
   const auto expected_character_three = Character({
       .id = 3,
       .first_name = "Test",
       .last_name = "Mann",
       .general_data = field_marshal_data,
   });
   const auto expected_character_four = Character({
       .id = 4,
       .first_name = "Test",
       .last_name = "Mann",
       .general_data = general_data,
   });
   const auto expected_character_five = Character({
       .id = 5,
       .first_name = "Test",
       .last_name = "WoMann",
       .is_female = true,
       .general_data = general_data,
   });
   const auto expected_character_six = Character({
       .id = 6,
       .first_name = "Test",
       .last_name = "Mann",
       .admiral_data = admiral_data,
   });
   const auto expected_character_seven = Character({
       .id = 7,
       .first_name = "Test",
       .last_name = "Mann",
       .spy_data = spy_data,
   });
   const auto expected_character_eight = Character({
       .id = 8,
       .first_name = "Test",
       .last_name = "Mann",
       .advisor_data = advisor_data,
   });
   const auto expected_character_nine = Character({
       .id = 9,
       .first_name = "Test",
       .last_name = "Mann",
       .general_data = general_data,
       .advisor_data = advisor_data,
   });
   const auto expected_character_ten = Character({
       .id = 10,
       .first_name = "Test",
       .last_name = "Mann",
       .admiral_data = admiral_data,
       .advisor_data = advisor_data,
   });

   EXPECT_THAT(characters,
       testing::UnorderedElementsAre(testing::Pair(1, expected_character_one),
           testing::Pair(2, expected_character_two),
           testing::Pair(3, expected_character_three),
           testing::Pair(4, expected_character_four),
           testing::Pair(5, expected_character_five),
           testing::Pair(6, expected_character_six),
           testing::Pair(7, expected_character_seven),
           testing::Pair(8, expected_character_eight),
           testing::Pair(9, expected_character_nine),
           testing::Pair(10, expected_character_ten)));
}


TEST(Hoi4worldCharactersHoi4charactersconverter, PortraitsAreAssigned)
{
   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");


   // Commander leader
   const auto character_two = vic3::Character({
       .id = 2,
       .first_name = "Test",
       .last_name = "Mann",
       .culture = "culture_1",
       .roles = {"politician", "general"},
       .rank = 1,
   });
   // General
   const auto character_four = vic3::Character({
       .id = 4,
       .first_name = "Test",
       .last_name = "Mann",
       .culture = "culture_1",
       .roles = {"general"},
       .rank = 2,
   });
   // Female General
   const auto character_five = vic3::Character({
       .id = 5,
       .first_name = "Test",
       .last_name = "WoMann",
       .culture = "culture_1",
       .is_female = true,
       .roles = {"general", "agitator"},
       .rank = 2,
   });
   // Admiral
   const auto character_six = vic3::Character({
       .id = 6,
       .first_name = "Test",
       .last_name = "Mann",
       .culture = "culture_1",
       .roles = {"admiral"},
       .rank = 2,
   });
   // Spy
   const auto character_seven = vic3::Character({
       .id = 7,
       .first_name = "Test",
       .last_name = "Mann",
       .culture = "culture_2",
       .roles = {"agitator"},
       .origin_country_id = 3,
   });
   // Advisor
   const auto character_eight = vic3::Character({
       .id = 8,
       .first_name = "Test",
       .last_name = "Mann",
       .culture = "culture_1",
       .roles = {"politician"},
   });
   // General Advisor
   const auto character_nine = vic3::Character({
       .id = 9,
       .first_name = "Test",
       .last_name = "Mann",
       .culture = "culture_1",
       .roles = {"general", "politician"},
       .rank = 2,
   });
   // Admiral Advisor
   const auto character_ten = vic3::Character({
       .id = 10,
       .first_name = "Test",
       .last_name = "Mann",
       .culture = "culture_1",
       .roles = {"admiral", "politician"},
       .rank = 2,
   });


   const std::map<int, vic3::Character> source_characters = {
       {2, character_two},
       {4, character_four},
       {5, character_five},
       {6, character_six},
       {7, character_seven},
       {8, character_eight},
       {9, character_nine},
       {10, character_ten},
   };
   ConvertCharacters(source_characters,
       "",
       "",
       "",
       vic3::Country({.head_of_state_id = 2, .character_ids = {2, 4, 5, 6, 7, 8, 9, 10}}),
       {},
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}),
       {},
       characters,
       culture_queues);


   std::map<std::string, std::vector<int>> culture_2_queue = {
       {"operative_male", {7}},
   };
   EXPECT_THAT(culture_queues.at("culture_1"),
       testing::UnorderedElementsAre(testing::Pair("army", std::vector{2, 4, 9}),
           testing::Pair("navy", std::vector{6, 10}),
           testing::Pair("female_leader", std::vector{5}),
           testing::Pair("advisor_neutrality", std::vector{8})));
   EXPECT_THAT(culture_queues.at("culture_2"),
       testing::UnorderedElementsAre(testing::Pair("operative_male", std::vector{7})));
}


TEST(Hoi4worldCharactersHoi4charactersconverter, CouncilsAreCreated)
{
   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const std::map<int, vic3::Character> source_characters;
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");

   const auto leader_data = std::optional<Leader>({.sub_ideology = "sub_ideology"});
   const auto expected_council = Character({
       .id = Character::GetGenId(),
       .first_name = "The",
       .last_name = "Council",
       .leader_data = leader_data,
   });

   ConvertCharacters(source_characters,
       "",
       "",
       "sub_ideology",
       vic3::Country({.active_laws = {"law_council_republic", "law_anarchy"}, .primary_cultures = {"culture_1"}}),
       {},
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}),
       {},
       characters,
       culture_queues);

   EXPECT_THAT(characters, testing::UnorderedElementsAre(testing::Pair(Character::GetGenId() - 1, expected_council)));
}


TEST(Hoi4worldCharactersHoi4charactersconverter, NewlyGeneratedCharactersDontCollideWithExisting)
{
   // Have a source character with base genID:1000.
   // Input council laws, see that newly generated character resolves collision
   // SrcCharacter intact, old council with ID 1001, new council with 1002
   std::map<int, Character> characters{
       {Character::GetGenId() + 1, hoi4::Character({.id = Character::GetGenId() + 1})},
   };
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const std::map<int, vic3::Character> source_characters{
       {Character::GetGenId(), vic3::Character({.id = Character::GetGenId()})},
   };
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");

   const auto leader_data = std::optional<Leader>({.sub_ideology = "sub_ideology"});
   const auto expected_council = Character({
       .id = Character::GetGenId() + 2,
       .first_name = "The",
       .last_name = "Council",
       .leader_data = leader_data,
   });

   ConvertCharacters(source_characters,
       "",
       "",
       "sub_ideology",
       vic3::Country({.active_laws = {"law_council_republic", "law_anarchy"}, .primary_cultures = {"culture_1"}}),
       {},
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}),
       {},
       characters,
       culture_queues);

   EXPECT_THAT(characters,
       testing::UnorderedElementsAre(
           testing::Pair(Character::GetGenId() - 2, Character({.id = Character::GetGenId() - 2})),
           testing::Pair(Character::GetGenId() - 1, expected_council)));
}


TEST(Hoi4worldCharactersHoi4charactersconverter, PrimeMinistersAreFoundInCoalitionParties)
{
   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");
   const std::map<int, vic3::InterestGroup> igs = {
       {1, vic3::InterestGroup{"", 1, 4, 25.0F, true, {}}},
       {2, vic3::InterestGroup{"", 1, 2, 35.0F, true, {}}},
       {3, vic3::InterestGroup{"", 1, 3, 40.0F, false, {}}},
   };
   const auto head_of_state = vic3::Character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .ig_id = 1,
       .roles = {"politician"},
   });
   const auto head_of_state_party_leader = vic3::Character({
       .id = 4,
       .first_name = "Test",
       .last_name = "Mann",
       .ig_id = 1,
       .roles = {"politician"},
   });
   const auto prime_minister = vic3::Character({
       .id = 2,
       .first_name = "Test",
       .last_name = "Mann",
       .ig_id = 2,
       .roles = {"politician"},
   });
   const auto opposition_leader = vic3::Character({
       .id = 3,
       .first_name = "Test",
       .last_name = "Mann",
       .ig_id = 3,
       .roles = {"politician"},
   });

   const std::map<int, vic3::Character> source_characters = {
       {1, head_of_state},
       {2, prime_minister},
       {3, opposition_leader},
       {4, head_of_state_party_leader},
   };
   ConvertCharacters(source_characters,
       "",
       "",
       "sub_ideology",
       vic3::Country({.active_laws = {"law_monarchy", "law_census_voting"}, .ig_ids = {1, 2, 3}}),
       igs,
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}),
       {},
       characters,
       culture_queues);

   // The prime minister is the government IG leader with the most clout.
   const auto leader_data = std::optional<Leader>({.sub_ideology = "sub_ideology"});
   const auto expected_leader = Character({
       .id = 2,
       .first_name = "Test",
       .last_name = "Mann",
       .leader_data = leader_data,
   });

   EXPECT_EQ(characters.at(2), expected_leader);
}


TEST(Hoi4worldCharactersHoi4charactersconverter, PrimeMinistersAreFoundInLeaderParty)
{
   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");
   const std::map<int, vic3::InterestGroup> igs = {
       {1, vic3::InterestGroup{"", 1, 4, 35.0F, true, {}}},
       {2, vic3::InterestGroup{"", 1, 2, 25.0F, true, {}}},
       {3, vic3::InterestGroup{"", 1, 3, 40.0F, false, {}}},
   };
   const auto head_of_state = vic3::Character({
       .id = 1,
       .first_name = "Test",
       .last_name = "Mann",
       .ig_id = 1,
       .roles = {"politician"},
   });
   const auto head_of_state_party_leader = vic3::Character({
       .id = 4,
       .first_name = "Test",
       .last_name = "Mann",
       .ig_id = 1,
       .roles = {"politician"},
   });
   const auto prime_minister = vic3::Character({
       .id = 2,
       .first_name = "Test",
       .last_name = "Mann",
       .ig_id = 2,
       .roles = {"politician"},
   });
   const auto opposition_leader = vic3::Character({
       .id = 3,
       .first_name = "Test",
       .last_name = "Mann",
       .ig_id = 3,
       .roles = {"politician"},
   });

   const std::map<int, vic3::Character> source_characters = {
       {1, head_of_state},
       {2, prime_minister},
       {3, opposition_leader},
       {4, head_of_state_party_leader},
   };
   ConvertCharacters(source_characters,
       "",
       "",
       "sub_ideology",
       vic3::Country({.active_laws = {"law_monarchy", "law_census_voting"}, .ig_ids = {1, 2, 3}}),
       igs,
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}),
       {},
       characters,
       culture_queues);

   // The prime minister is the government IG leader with the most clout.
   const auto leader_data = std::optional<Leader>({.sub_ideology = "sub_ideology"});
   const auto expected_leader = Character({
       .id = 4,
       .first_name = "Test",
       .last_name = "Mann",
       .leader_data = leader_data,
   });

   EXPECT_EQ(characters.at(4), expected_leader);
}


TEST(Hoi4worldCharactersHoi4charactersconverter, OrderIsPreservedOnSamePlaythrough)
{
   const auto culture_graphics_mapper = mappers::ImportCultureGraphicsMapper("configurables/culture_graphics.txt");
   std::map<int, Character> characters_one{
       {1, Character({.id = 1})},
       {2, Character({.id = 2})},
       {3, Character({.id = 3})},
       {4, Character({.id = 4})},
       {5, Character({.id = 5})},
       {6, Character({.id = 6})},
       {7, Character({.id = 7})},
   };
   std::map<int, Character> characters_two{
       {1, Character({.id = 1})},
       {2, Character({.id = 2})},
       {3, Character({.id = 3})},
       {4, Character({.id = 4})},
       {5, Character({.id = 5})},
       {6, Character({.id = 6})},
       {7, Character({.id = 7})},
   };
   std::map<std::string, mappers::CultureQueue> culture_queues{
       {"culture_2", {{"operative_female", {1, 2, 3, 4, 5, 6, 7}}}},
   };
   std::map<std::string, vic3::CultureDefinition> source_cultures{
       {"culture_2", vic3::CultureDefinition({"culture_2"}, {}, {}, {})},
   };

   AssignPortraits(culture_queues, culture_graphics_mapper, source_cultures, 10, characters_one);
   AssignPortraits(culture_queues, culture_graphics_mapper, source_cultures, 10, characters_two);
   EXPECT_EQ(characters_one, characters_two);
}


TEST(Hoi4worldCharactersHoi4charactersconverter, OrderIsChangedOnDifferentPlaythrough)
{
   const auto culture_graphics_mapper = mappers::ImportCultureGraphicsMapper("configurables/culture_graphics.txt");
   std::map<int, Character> characters_one{
       {1, Character({.id = 1})},
       {2, Character({.id = 2})},
       {3, Character({.id = 3})},
       {4, Character({.id = 4})},
       {5, Character({.id = 5})},
       {6, Character({.id = 6})},
       {7, Character({.id = 7})},
   };
   std::map<int, Character> characters_two{
       {1, Character({.id = 1})},
       {2, Character({.id = 2})},
       {3, Character({.id = 3})},
       {4, Character({.id = 4})},
       {5, Character({.id = 5})},
       {6, Character({.id = 6})},
       {7, Character({.id = 7})},
   };
   std::map<std::string, mappers::CultureQueue> culture_queues{
       {"culture_2", {{"operative_female", {1, 2, 3, 4, 5, 6, 7}}}},
   };
   std::map<std::string, vic3::CultureDefinition> source_cultures{
       {"culture_2", vic3::CultureDefinition({"culture_2"}, {}, {}, {})},
   };

   AssignPortraits(culture_queues, culture_graphics_mapper, source_cultures, 10, characters_one);
   AssignPortraits(culture_queues, culture_graphics_mapper, source_cultures, 11, characters_two);
   EXPECT_NE(characters_one, characters_two);
}


TEST(Hoi4worldCharactersHoi4charactersconverter, PreferUnusedPortraitsBetweenCultures)
{
   // Have ~5 cultures using the same ~5 portraits in the same input order for 1 character each.
   // Portrait usage will be [2 2 1] instead of [5 0 0]
   const auto culture_graphics_mapper = mappers::ImportCultureGraphicsMapper("configurables/culture_graphics.txt");
   std::map<int, Character> characters{
       {1, Character({.id = 1})},
       {2, Character({.id = 2})},
       {3, Character({.id = 3})},
       {4, Character({.id = 4})},
       {5, Character({.id = 5})},
   };
   std::map<std::string, mappers::CultureQueue> culture_queues{
       {"culture_3", {{"army", {1}}}},
       {"culture_4", {{"army", {2}}}},
       {"culture_5", {{"army", {3}}}},
       {"culture_6", {{"army", {4}}}},
       {"culture_7", {{"army", {5}}}},
   };
   // Each culture has an army portrait list of the same length, so they will all be shuffled the same way
   std::map<std::string, vic3::CultureDefinition> source_cultures{
       {"culture_3", vic3::CultureDefinition({"culture_3"}, {}, {}, {})},
       {"culture_4", vic3::CultureDefinition({"culture_4"}, {}, {}, {})},
       {"culture_5", vic3::CultureDefinition({"culture_5"}, {}, {}, {})},
       {"culture_6", vic3::CultureDefinition({"culture_6"}, {}, {}, {})},
       {"culture_7", vic3::CultureDefinition({"culture_7"}, {}, {}, {})},
   };

   AssignPortraits(culture_queues, culture_graphics_mapper, source_cultures, 10, characters);

   std::map<std::string, int> portrait_counts;
   for (const auto& character: characters | std::views::values)
   {
      portrait_counts.emplace(character.GetPortraitAlias(), 0);
      portrait_counts.find(character.GetPortraitAlias())->second++;
   }

   for (const auto& count: portrait_counts | std::views::values)
   {
      EXPECT_LE(count, 2);
   }
}

}  // namespace hoi4