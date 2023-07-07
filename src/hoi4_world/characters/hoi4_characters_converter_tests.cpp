#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/characters/hoi4_characters_converter.h"
#include "src/mappers/character/leader_type_mapper_importer.h"



namespace hoi4
{

TEST(Hoi4worldCharactersHoi4charactersconverter, CharactersAreConverted)
{
   const auto country_mapper = mappers::CountryMapper({{3, "TWO"}});

   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");


   // Commander leader
   const auto character_two = vic3::Character({
       .id = 2,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"politician", "general"},
       .rank = 1,
   });
   // General
   const auto character_four = vic3::Character({
       .id = 4,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"general"},
       .rank = 2,
   });
   // Female General
   const auto character_five = vic3::Character({
       .id = 5,
       .first_name = "Test",
       .last_name = "WoMann",
       .is_female = true,
       .roles = {"general", "agitator"},
       .rank = 2,
   });
   // Admiral
   const auto character_six = vic3::Character({
       .id = 6,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"admiral"},
       .rank = 2,
   });
   // Spy
   const auto character_seven = vic3::Character({
       .id = 7,
       .first_name = "Test",
       .last_name = "Mann",
       .roles = {"agitator"},
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
   });
   // Admiral Advisor
   const auto character_ten = vic3::Character({
       .id = 10,
       .first_name = "Test",
       .last_name = "Mann",
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
   ConvertCharacters(characters,
       source_characters,
       "TAG",
       "sub_ideology",
       vic3::Country({.head_of_state_id = 2, .character_ids = {2, 4, 5, 6, 7, 8, 9, 10}}),
       {},
       leader_type_mapper,
       culture_queues,
       country_mapper);

   const auto leader_data = std::optional<Leader>({.sub_ideology = "sub_ideology"});
   const auto general_data = std::optional<General>({.traits = {}});
   const auto admiral_data = std::optional<Admiral>({.traits = {}});
   const auto advisor_data = std::optional<Advisor>({.slot = "political_advisor"});
   const auto spy_data = std::optional<Spy>({.nationalities = {"TAG", "TWO"}});
   const auto expected_character_two = Character({
       .id = 2,
       .first_name = "Test",
       .last_name = "Mann",
       .general_data = general_data,
       .leader_data = leader_data,
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
       testing::UnorderedElementsAre(std::pair{2, expected_character_two},
           std::pair{4, expected_character_four},
           std::pair{5, expected_character_five},
           std::pair{6, expected_character_six},
           std::pair{7, expected_character_seven},
           std::pair{8, expected_character_eight},
           std::pair{9, expected_character_nine},
           std::pair{10, expected_character_ten}));
}


// TEST(Hoi4worldCharactersHoi4charactersconverter, DISABLED_PortraitsAreAssigned)
//{
//    std::map<int, Character> characters;
//    std::map<std::string, mappers::CultureQueue> culture_queues;
//
//
//    // Commander leader
//    const auto character_two = vic3::Character({
//        .id = 2,
//        .first_name = "Test",
//        .last_name = "Mann",
//        .culture = "culture_1",
//        .roles = {"politician", "general"},
//        .rank = 1,
//    });
//    // General
//    const auto character_four = vic3::Character({
//        .id = 4,
//        .first_name = "Test",
//        .last_name = "Mann",
//        .culture = "culture_1",
//        .roles = {"general"},
//        .rank = 2,
//    });
//    // Female General
//    const auto character_five = vic3::Character({
//        .id = 5,
//        .first_name = "Test",
//        .last_name = "WoMann",
//        .culture = "culture_1",
//        .is_female = true,
//        .roles = {"general", "agitator"},
//        .rank = 2,
//    });
//    // Admiral
//    const auto character_six = vic3::Character({
//        .id = 6,
//        .first_name = "Test",
//        .last_name = "Mann",
//        .culture = "culture_1",
//        .roles = {"admiral"},
//        .rank = 2,
//    });
//    // Spy
//    const auto character_seven = vic3::Character({
//        .id = 7,
//        .first_name = "Test",
//        .last_name = "Mann",
//        .culture = "culture_2",
//        .roles = {"agitator"},
//        .origin_country_id = 3,
//    });
//    // Advisor
//    const auto character_eight = vic3::Character({
//        .id = 8,
//        .first_name = "Test",
//        .last_name = "Mann",
//        .culture = "culture_1",
//        .roles = {"politician"},
//    });
//    // General Advisor
//    const auto character_nine = vic3::Character({
//        .id = 9,
//        .first_name = "Test",
//        .last_name = "Mann",
//        .culture = "culture_1",
//        .roles = {"general", "politician"},
//        .rank = 2,
//    });
//    // Admiral Advisor
//    const auto character_ten = vic3::Character({
//        .id = 10,
//        .first_name = "Test",
//        .last_name = "Mann",
//        .culture = "culture_1",
//        .roles = {"admiral", "politician"},
//        .rank = 2,
//    });
//
//
//    const std::map<int, vic3::Character> source_characters = {
//        {2, character_two},
//        {4, character_four},
//        {5, character_five},
//        {6, character_six},
//        {7, character_seven},
//        {8, character_eight},
//        {9, character_nine},
//        {10, character_ten},
//    };
//    ConvertCharacters(characters,
//        source_characters,
//        "",
//        "",
//        vic3::Country({.head_of_state_id = 2, .character_ids = {2, 4, 5, 6, 7, 8, 9, 10}}),
//        {},
//        leader_type_mapper,
//        culture_queues,
//        {});
//
//
//    std::map<std::string, std::vector<int>> culture_2_queue = {
//        {"male_spy", {7}},
//    };
//    EXPECT_THAT(culture_queues.at("culture_1"),
//        testing::UnorderedElementsAre(std::pair<std::string, std::vector<int>>{"army", {2, 4, 9}},
//            std::pair<std::string, std::vector<int>>{"navy", {6, 10}},
//            std::pair<std::string, std::vector<int>>{"female_leader", {5}},
//            std::pair<std::string, std::vector<int>>{"advisor_neutrality", {8}}));
//    EXPECT_THAT(culture_queues.at("culture_2"),
//        testing::UnorderedElementsAre(std::pair<std::string, std::vector<int>>{"male_spy", {7}}));
// }


TEST(Hoi4worldCharactersHoi4charactersconverter, CouncilsAreCreated)
{
   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const std::map<int, vic3::Character> source_characters;
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");


   ConvertCharacters(characters,
       source_characters,
       "",
       "sub_ideology",
       vic3::Country({.active_laws = {"law_council_republic", "law_anarchy"}, .primary_cultures = {"culture_1"}}),
       {},
       leader_type_mapper,
       culture_queues,
       {});

   const auto leader_data = std::optional<Leader>({.sub_ideology = "sub_ideology"});
   const auto expected_council = Character({
       .id = Character::GetGenId() - 1,
       .first_name = "The",
       .last_name = "Council",
       .leader_data = leader_data,
   });

   EXPECT_THAT(characters, testing::UnorderedElementsAre(std::pair{Character::GetGenId() - 1, expected_council}));
}


TEST(Hoi4worldCharactersHoi4charactersconverter, PrimeMinistersAreFoundInCoalitionParties)
{
   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const auto leader_type_mapper = mappers::ImportLeaderTypeMapper("configurables/leader_type_mappings.txt");
   const std::map<int, vic3::InterestGroup> igs = {
       {1, vic3::InterestGroup{"", 1, 4, 25.0F, true}},
       {2, vic3::InterestGroup{"", 1, 2, 35.0F, true}},
       {3, vic3::InterestGroup{"", 1, 3, 40.0F, false}},
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
   ConvertCharacters(characters,
       source_characters,
       "",
       "sub_ideology",
       vic3::Country({.active_laws = {"law_monarchy", "law_census_voting"}, .ig_ids = {1, 2, 3}}),
       igs,
       leader_type_mapper,
       culture_queues,
       {});

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
       {1, vic3::InterestGroup{"", 1, 4, 35.0F, true}},
       {2, vic3::InterestGroup{"", 1, 2, 25.0F, true}},
       {3, vic3::InterestGroup{"", 1, 3, 40.0F, false}},
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
   ConvertCharacters(characters,
       source_characters,
       "",
       "sub_ideology",
       vic3::Country({.active_laws = {"law_monarchy", "law_census_voting"}, .ig_ids = {1, 2, 3}}),
       igs,
       leader_type_mapper,
       culture_queues,
       {});

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