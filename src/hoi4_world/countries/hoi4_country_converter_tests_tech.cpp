// Contains tech and research tests for Country
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "hoi4_country_converter.h"
#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/mappers/character/culture_queue.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountryConverter, DecentralizedCountriesHaveTwoResearchSlots)
{
   const vic3::World source_world(
       {.country_rankings = vic3::CountryRankings(std::set{4}, std::set{3, 2}, {{2, 10}, {3, 10}, {4, 10}})});
   const vic3::Country c1({.number = 1, .country_type = "decentralized"});

   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       c1,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   EXPECT_EQ(country_one.value().GetStartingResearchSlots(), 2);
}


TEST(Hoi4worldCountriesCountryConverter, CountriesHaveThreeResearchSlotsByDefault)
{
   const vic3::World source_world(
       {.country_rankings = vic3::CountryRankings(std::set{4}, std::set{3, 2}, {{2, 10}, {3, 10}, {4, 10}})});
   const vic3::Country c2({.number = 2, .country_type = "colonial"});

   const mappers::CountryMapper country_mapper({{2, "Z02"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_two = ConvertCountry(source_world,
       c2,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   EXPECT_EQ(country_two.value().GetStartingResearchSlots(), 3);
}


TEST(Hoi4worldCountriesCountryConverter, UnrecognizedCountriesHaveTwoResearchSlots)
{
   const vic3::World source_world(
       {.country_rankings = vic3::CountryRankings(std::set{4}, std::set{3, 2}, {{2, 10}, {3, 10}, {4, 10}})});
   const vic3::Country c3({.number = 3, .country_type = "unrecognized"});

   const mappers::CountryMapper country_mapper({{3, "Z03"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_three = ConvertCountry(source_world,
       c3,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   EXPECT_EQ(country_three.value().GetStartingResearchSlots(), 2);
}


TEST(Hoi4worldCountriesCountryConverter, GreatPowersHaveFourResearchSlots)
{
   const vic3::World source_world(
       {.country_rankings = vic3::CountryRankings(std::set{4}, std::set{3, 2}, {{2, 10}, {3, 10}, {4, 10}})});
   const vic3::Country c4({.number = 4, .country_type = "recognized"});

   const mappers::CountryMapper country_mapper({{4, "Z04"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_four = ConvertCountry(source_world,
       c4,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   EXPECT_EQ(country_four.value().GetStartingResearchSlots(), 4);
}

}  // namespace hoi4