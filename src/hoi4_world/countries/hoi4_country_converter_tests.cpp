#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{
#ifdef KNOCKOUT
TEST(Hoi4worldCountriesCountryConverter, TagIsFromSourceCountry)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "T00"}, {2, "T01"}});
   const vic3::Country source_country_one({.number = 1, .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.number = 2, .color = commonItems::Color{std::array{2, 4, 6}}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);
   const auto country_two = ConvertCountry(source_world,
       source_country_two,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetTag(), "T00");
   EXPECT_EQ(country_one->GetColor(), commonItems::Color(std::array{1, 2, 3}));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetTag(), "T01");
   EXPECT_EQ(country_two->GetColor(), commonItems::Color(std::array{2, 4, 6}));
}


TEST(Hoi4worldCountriesCountryConverter, NoCountryIfNoSourceTag)
{
   const mappers::CountryMapper country_mapper(std::map<int, std::string>{
       {1, "T00"},
   });
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   EXPECT_EQ(country_one, std::nullopt);
}



TEST(Hoi4worldCountriesCountryConverter, NoCountryIfNoTagMapping)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper;
   const vic3::Country source_country_one({.number = 1});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   EXPECT_EQ(country_one, std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, CapitalStatesAreConverted)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   const vic3::Country source_country_two({.number = 2, .capital_state = 3});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const States states({.vic3_state_ids_to_hoi4_state_ids{{2, 4}, {3, 9}}});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);
   const auto country_two = ConvertCountry(source_world,
       source_country_two,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(4));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetCapitalState(), std::optional(9));
}


TEST(Hoi4worldCountriesCountryConverter, NoCapitalStateIfNoSourceCapitalState)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({
       .number = 1,
   });
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const States states({.vic3_state_ids_to_hoi4_state_ids{{2, 4}}});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, NoCapitalStateIfNoStateMappingAndNoStates)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, HighestVpStateBecomesCapitalIfCapitalNotConverted)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;
   const States states({.states{
       State(1, {.owner = "TAG", .victory_points = {{1, 1}, {2, 2}, {3, 3}}}),
       State(2, {.owner = "TAG", .victory_points = {{2, 2}, {4, 4}, {6, 6}}}),
       State(3, {.owner = "TAG", .victory_points = {{1, 1}, {2, 1}, {3, 1}}}),
   }});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(2));
}


TEST(Hoi4worldCountriesCountryConverter, HighestIndustryStateBecomesCapitalIfVpsAreSame)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;
   const States states({.states{
       State(1, {.owner = "TAG", .civilian_factories = 1, .military_factories = 2, .dockyards = 3}),
       State(2, {.owner = "TAG", .civilian_factories = 2, .military_factories = 4, .dockyards = 6}),
       State(3, {.owner = "TAG", .civilian_factories = 1, .military_factories = 1, .dockyards = 1}),
   }});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(2));
}


TEST(Hoi4worldCountriesCountryConverter, HighestManpowerStateBecomesCapitalIfIndustriesAreSame)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;
   const States states({.states{
       State(1, {.owner = "TAG", .manpower = 1234}),
       State(2, {.owner = "TAG", .manpower = 2468}),
       State(3, {.owner = "TAG", .manpower = 1111}),
   }});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(2));
}


TEST(Hoi4worldCountriesCountryConverter, LowestIdStateBecomesCapitalIfManpowersAreSame)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;
   const States states({.states{
       State(3, {.owner = "TAG"}),
       State(1, {.owner = "TAG"}),
       State(2, {.owner = "TAG"}),
   }});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(1));
}


TEST(Hoi4worldCountriesCountryConverter, StatesNotOwnedByCountryCannotBecomeCapital)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;
   const States states({.states{
       State(1, {}),
       State(2, {.owner = "TWO"}),
   }});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, NonDemocraciesPickSentinelElectionYear)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1933.1.1"));
}


TEST(Hoi4worldCountriesCountryConverter, OutdatedElectionsExtrapolateToPresent)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .last_election = date("1894.4.23")});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1934.4.23"));
}


TEST(Hoi4worldCountriesCountryConverter, FutureElectionsFallbackToPresent)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .last_election = date("1937.2.15")});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1933.2.15"));
}


TEST(Hoi4worldCountriesCountryConverter, ContemporaryElectionsRemainUnchanged)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .last_election = date("1935.11.4")});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1935.11.4"));
}


TEST(Hoi4worldCountriesCountryConverter, InYearFutureElectionsAreCurrentCycle)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .last_election = date("1928.10.14")});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1932.10.14"));
}


TEST(Hoi4worldCountriesCountryConverter, InYearPastElectionsAreNextCycle)
{
   // When election is in the same year as the start date, kick it to next cycle when occurring on or before start date
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .last_election = date("1928.1.1")});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1936.1.1"));
}


TEST(Hoi4worldCountriesCountryConverter, RulingIdeologyCanBeConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .active_laws = {}});
   const vic3::Country source_country_two({.number = 1, .active_laws = {"law_landed_voting"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const mappers::IdeologyMapper ideology_mapper(
       {
           {"law_landed_voting", {{"democratic", 100}}},
       },
       {});

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       ideology_mapper,
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   const std::optional<Country> country_two = ConvertCountry(source_world,
       source_country_two,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       ideology_mapper,
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetIdeology(), "neutrality");
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetIdeology(), "democratic");
}


TEST(Hoi4worldCountriesCountryConverter, SubIdeologyCanBeConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .active_laws = {}});
   const vic3::Country source_country_two({.number = 1, .active_laws = {"law_landed_voting"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const mappers::IdeologyMapper ideology_mapper(
       {
           {"law_landed_voting", {{"democratic", 100}}},
       },
       {
           {
               "democratic",
               {
                   {"law_landed_voting",
                       {
                           {"liberalism", 100},
                       }},
               },
           },
       });

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       ideology_mapper,
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   const std::optional<Country> country_two = ConvertCountry(source_world,
       source_country_two,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       ideology_mapper,
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetSubIdeology(), "despotism");
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetSubIdeology(), "liberalism");
}


TEST(Hoi4worldCountriesCountryConverter, TechnologiesAreConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world =
       vic3::World(vic3::WorldOptions{.acquired_technologies = {{{1}, {"source_tech"}}, {{2}, {"source_tech"}}}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {{{"source_tech"}, std::nullopt, {"dest_tech_one", "dest_tech_two"}}},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetTechnologies().GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::nullopt, std::set<std::string>{"dest_tech_one", "dest_tech_two"})));
}


TEST(Hoi4worldCountriesCountryConverter, VariantsRequireAllRequiredTechs)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions({.acquired_technologies = {{1, {"source_tech"}}}}));
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {{{"source_tech"}, std::nullopt, {"required_tech_one", "required_tech_two"}}},
       {
           EquipmentVariant({}, {}, {{"name", "legacy_ship: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_missing"}, {}, {{"name", "legacy_ship: missing_required_tech_fails"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "legacy_ship: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_missing"},
               {},
               {{"name", "legacy_ship: missing_and_present_required_techs_fails"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "legacy_ship: all_present_required_techs_succeeds"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "ship: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_missing"}, {}, {{"name", "ship: missing_required_tech_fails"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "ship: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_missing"},
               {},
               {{"name", "ship: missing_and_present_required_techs_fails"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "ship: all_present_required_techs_succeeds"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "plane: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_missing"}, {}, {{"name", "plane: missing_required_tech_fails"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "plane: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_missing"},
               {},
               {{"name", "plane: missing_and_present_required_techs_fails"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "plane: all_present_required_techs_succeeds"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "tank: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_missing"}, {}, {{"name", "tank: missing_required_tech_fails"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "tank: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_missing"},
               {},
               {{"name", "tank: missing_and_present_required_techs_fails"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "tank: all_present_required_techs_succeeds"}}),
       },
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLegacyShipVariants(),
       testing::ElementsAre(
           EquipmentVariant({}, {}, {{"name", "legacy_ship: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "legacy_ship: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "legacy_ship: all_present_required_techs_succeeds"}})));
   EXPECT_THAT(country_one->GetShipVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "ship: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "ship: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "ship: all_present_required_techs_succeeds"}})));
   EXPECT_THAT(country_one->GetPlaneVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "plane: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "plane: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "plane: all_present_required_techs_succeeds"}})));
   EXPECT_THAT(country_one->GetTankVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "tank: no_required_techs_automatically_succeeds"}}),
           EquipmentVariant({"required_tech_one"}, {}, {{"name", "tank: present_required_tech_succeeds"}}),
           EquipmentVariant({"required_tech_one", "required_tech_two"},
               {},
               {{"name", "tank: all_present_required_techs_succeeds"}})));
}


TEST(Hoi4worldCountriesCountryConverter, VariantsBlockedByAnyBlockingTechs)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world =
       vic3::World(vic3::WorldOptions{.acquired_technologies = {{{1}, {"source_tech"}}, {{2}, {"source_tech"}}}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {{{"source_tech"}, std::nullopt, {"blocking_tech_one", "blocking_tech_two"}}},
       {
           EquipmentVariant({}, {}, {{"name", "legacy_ship: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_one"}, {{"name", "legacy_ship: having_blocking_tech_fails"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "legacy_ship: missing_blocking_tech_succeeds"}}),
           EquipmentVariant({},
               {"blocking_tech_one", "blocking_tech_missing"},
               {{"name", "legacy_ship: missing_and_present_blocking_techs_fails"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "ship: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_one"}, {{"name", "ship: having_blocking_tech_fails"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "ship: missing_blocking_tech_succeeds"}}),
           EquipmentVariant({},
               {"blocking_tech_one", "blocking_tech_missing"},
               {{"name", "ship: missing_and_present_blocking_techs_fails"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "plane: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_one"}, {{"name", "plane: having_blocking_tech_fails"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "plane: missing_blocking_tech_succeeds"}}),
           EquipmentVariant({},
               {"blocking_tech_one", "blocking_tech_missing"},
               {{"name", "plane: missing_and_present_blocking_techs_fails"}}),
       },
       {
           EquipmentVariant({}, {}, {{"name", "tank: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_one"}, {{"name", "tank: having_blocking_tech_fails"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "tank: missing_blocking_tech_succeeds"}}),
           EquipmentVariant({},
               {"blocking_tech_one", "blocking_tech_missing"},
               {{"name", "tank: missing_and_present_blocking_techs_fails"}}),
       },
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLegacyShipVariants(),
       testing::ElementsAre(
           EquipmentVariant({}, {}, {{"name", "legacy_ship: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "legacy_ship: missing_blocking_tech_succeeds"}})));
   EXPECT_THAT(country_one->GetShipVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "ship: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "ship: missing_blocking_tech_succeeds"}})));
   EXPECT_THAT(country_one->GetPlaneVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "plane: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "plane: missing_blocking_tech_succeeds"}})));
   EXPECT_THAT(country_one->GetTankVariants(),
       testing::ElementsAre(EquipmentVariant({}, {}, {{"name", "tank: no_blocking_techs_automatically_succeeds"}}),
           EquipmentVariant({}, {"blocking_tech_missing"}, {{"name", "tank: missing_blocking_tech_succeeds"}})));
}


TEST(Hoi4worldCountriesCountryConverter, LawsDefaultsToDefaultLaws)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLaws(),
       testing::UnorderedElementsAre("civilian_economy", "export_focus", "volunteer_only"));
}


TEST(Hoi4worldCountriesCountryConverter, FascistCountriesGetDifferentEconomicIdeas)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2, .active_laws = {"test_fascism_law"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       {},
       mappers::IdeologyMapper({{"test_fascism_law", {{"fascism", 100}}}}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLaws(),
       testing::UnorderedElementsAre("partial_economic_mobilisation", "limited_exports", "volunteer_only"));
}


TEST(Hoi4worldCountriesCountryConverter, MassConscriptionLeadsToLimitedConscription)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2, .active_laws = {"law_mass_conscription"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   mappers::IdeologyPointsMap foo{{"fascism", 100}};

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLaws(),
       testing::UnorderedElementsAre("civilian_economy", "export_focus", "limited_conscription"));
}


TEST(Hoi4worldCountriesCountryConverter, IdeasDefaultToEmpty)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);
   ASSERT_TRUE(country_one.has_value());
   EXPECT_TRUE(country_one->GetIdeas().empty());
}


TEST(Hoi4worldCountriesCountryConverter, DecentrailzedCountriesGetDecentralizedIdea)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({.number = 1, .capital_state = 2, .country_type = "decentralized"});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);
   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetIdeas(), testing::UnorderedElementsAre("decentralized"));
}


TEST(Hoi4worldCountriesCountryConverter, OnlyConservativeMonarchiesHaveNobleLeaders)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});



   const vic3::Country source_country_one(
       {.number = 1, .active_laws = {"law_monarchy"}, .primary_cultures = {"culture"}});
   const vic3::Country source_country_two(
       {.number = 1, .active_laws = {"law_monarchy", "law_voting"}, .primary_cultures = {"culture"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const vic3::CultureDefinition culture_def{"culture",
       {
           .male_common_first = {"president"},
           .female_common_first = {"presidentin"},
           .common_last = {"doe"},
           .noble_last = {"von Doe"},
           .male_regal_first = {"king0", "king1", "king2", "king3", "king4"},
           .female_regal_first = {"queen0", "queen1", "queen2", "queen3", "queen4"},
       },
       {},
       {}};
   const std::map<std::string, vic3::CultureDefinition> culture_definitions{{"culture", culture_def}};
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.culture_definitions = culture_definitions});

   commonItems::LocalizationDatabase locs("english", {});
   std::vector keys{"president",
       "presidentin",
       "doe",
       "von Doe",
       "king0",
       "king1",
       "king2",
       "king3",
       "king4",
       "queen0",
       "queen1",
       "queen2",
       "queen3",
       "queen4"};
   for (const auto& key: keys)
   {
      commonItems::LocalizationBlock block(key, "english");
      block.ModifyLocalization("english", key);
      locs.AddOrModifyLocalizationBlock(key, block);
   }

   const mappers::LeaderTypeMapper leader_type_mapper({{"law_monarchy", {{"law_voting", "prime_minister"}}}});

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
       locs,
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   const std::optional<Country> country_two = ConvertCountry(source_world,
       source_country_two,
       locs,
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetSubIdeology(), "despotism");
   EXPECT_THAT(country_one->GetNameList().male_names,
       testing::ElementsAre("king0", "king1", "king2", "king3", "king4"));
   EXPECT_THAT(country_one->GetNameList().female_names,
       testing::ElementsAre("queen0", "queen1", "queen2", "queen3", "queen4"));
   EXPECT_THAT(country_one->GetNameList().surnames, testing::ElementsAre("von Doe"));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_THAT(country_two->GetNameList().male_names, testing::ElementsAre("president"));
   EXPECT_THAT(country_two->GetNameList().female_names, testing::ElementsAre("presidentin"));
   EXPECT_THAT(country_two->GetNameList().surnames, testing::ElementsAre("doe"));
}


TEST(Hoi4worldCountriesCountryConverter, UndefinedNobleFirstsDefaultToCommon)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one(
       {.number = 1, .active_laws = {"law_monarchy"}, .primary_cultures = {"culture"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const vic3::CultureDefinition culture_def{"culture",
       {
           .male_common_first = {"president"},
           .female_common_first = {"presidentin"},
           .noble_last = {"von Doe"},
       },
       {},
       {}};
   const std::map<std::string, vic3::CultureDefinition> culture_definitions{{"culture", culture_def}};
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.culture_definitions = culture_definitions});
   commonItems::LocalizationDatabase locs("english", {});
   std::vector keys{"president", "presidentin", "doe", "von Doe", "king", "queen"};
   for (const auto& key: keys)
   {
      commonItems::LocalizationBlock block(key, "english");
      block.ModifyLocalization("english", key);
      locs.AddOrModifyLocalizationBlock(key, block);
   }

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
       locs,
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetNameList().male_names, testing::ElementsAre("president"));
   EXPECT_THAT(country_one->GetNameList().female_names, testing::ElementsAre("presidentin"));
   EXPECT_THAT(country_one->GetNameList().surnames, testing::ElementsAre("von Doe"));
}


TEST(Hoi4worldCountriesCountryConverter, TooFewNobleFirstsAddsCommonFirsts)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one(
       {.number = 1, .active_laws = {"law_monarchy"}, .primary_cultures = {"culture"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const vic3::CultureDefinition culture_def{"culture",
       {
           .male_common_first = {"president"},
           .female_common_first = {"presidentin"},
           .noble_last = {"von Doe"},
           .male_regal_first = {"king"},
           .female_regal_first = {"queen"},
       },
       {},
       {}};
   const std::map<std::string, vic3::CultureDefinition> culture_definitions{{"culture", culture_def}};
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.culture_definitions = culture_definitions});
   commonItems::LocalizationDatabase locs("english", {});
   std::vector keys{"president", "presidentin", "doe", "von Doe", "king", "queen"};
   for (const auto& key: keys)
   {
      commonItems::LocalizationBlock block(key, "english");
      block.ModifyLocalization("english", key);
      locs.AddOrModifyLocalizationBlock(key, block);
   }

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
       locs,
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetNameList().male_names, testing::UnorderedElementsAre("president", "king"));
   EXPECT_THAT(country_one->GetNameList().female_names, testing::UnorderedElementsAre("presidentin", "queen"));
   EXPECT_THAT(country_one->GetNameList().surnames, testing::ElementsAre("von Doe"));
}


TEST(Hoi4worldCountriesCountryConverter, MissingNameLocsUseSentinielValue)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one({.number = 1, .active_laws = {}, .primary_cultures = {"culture"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const vic3::CultureDefinition culture_def{"culture", {.male_common_first = {"president"}}, {}, {}};
   const std::map<std::string, vic3::CultureDefinition> culture_definitions{{"culture", culture_def}};
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.culture_definitions = culture_definitions});
   commonItems::LocalizationDatabase locs("english", {});

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
       locs,
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetNameList().male_names, testing::ElementsAre("John"));
}



TEST(Hoi4worldCountriesCountryConverter, MissingNameLocsLogWarning)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one({.number = 1, .active_laws = {}, .primary_cultures = {"culture"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const vic3::CultureDefinition culture_def{"culture", {.male_common_first = {"president"}}, {}, {}};
   const std::map<std::string, vic3::CultureDefinition> culture_definitions{{"culture", culture_def}};
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.culture_definitions = culture_definitions});
   commonItems::LocalizationDatabase locs("english", {});

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
       locs,
       country_mapper,
       {},
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] Missing loc for vic_name: president.)"));
}


TEST(Hoi4worldCountriesCountryConverter, GraphicsBlocksAreSet)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one({.number = 1, .active_laws = {}, .primary_cultures = {"culture"}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const vic3::CultureDefinition culture_def{"culture", {}, {}, {}};
   const std::map<std::string, vic3::CultureDefinition> culture_definitions{{"culture", culture_def}};
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.culture_definitions = culture_definitions});
   const mappers::CultureGraphicsMapper culture_graphics_mapper{
       {{{"culture"}, {}, {}, {{{"army", {"GFX_general"}}}, {}, {}}}}};

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
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
       culture_graphics_mapper,
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetGraphicsBlock().portrait_paths.at("army"), testing::ElementsAre("GFX_general"));
}

TEST(Hoi4worldCountriesCountryConverter, PuppetsAreConverted)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const mappers::CountryMapper country_mapper({{1, "T00"}, {2, "T01"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({
       .number = 1,
       .color = commonItems::Color{std::array{1, 2, 3}},
       .puppets = {2, 50},
   });
   const vic3::Country source_country_two({.number = 2, .color = commonItems::Color{std::array{2, 4, 6}}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);
   const auto country_two = ConvertCountry(source_world,
       source_country_two,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetPuppets(), std::set<std::string>({"T01"}));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetPuppets(), std::set<std::string>());
   std::cout.rdbuf(cout_buffer);
   EXPECT_THAT(log.str(),
       testing::HasSubstr(R"([ERROR] Invalid subject relationship between 1 and nonexistent country 50)"));
}


TEST(Hoi4worldCountriesCountryConverter, OverlordIsConverted)
{
   const mappers::CountryMapper country_mapper({{1, "T00"}, {2, "T01"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const vic3::Country source_country_one({
       .number = 1,
       .color = commonItems::Color{std::array{1, 2, 3}},
       .overlord = 2,
   });
   const vic3::Country source_country_two({.number = 2, .color = commonItems::Color{std::array{2, 4, 6}}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);
   const auto country_two = ConvertCountry(source_world,
       source_country_two,
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
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetOverlord(), std::make_optional("T01"));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetOverlord(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, SpiesAndLeadersAreSeparated)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});

   const vic3::Country source_country_one({.number = 1, .head_of_state_id = 1, .character_ids = {1, 2, 3}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const std::map<int, vic3::Character> characters = {
       {1, vic3::Character({.id = 1})},
       {2, vic3::Character({.id = 2, .roles = {"agitator"}})},
       {3, vic3::Character({.id = 3, .roles = {"agitator", "general"}})},
   };

   const vic3::World source_world(vic3::WorldOptions({.characters = characters}));

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper({}),
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLeaderIds(), testing::ElementsAre(1, 3));
   EXPECT_THAT(country_one->GetSpyIds(), testing::ElementsAre(2));
}


TEST(Hoi4worldCountriesCountryConverter, CharactersConvert)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});

   const vic3::Country source_country_one({.number = 1, .head_of_state_id = 1, .character_ids = {1, 2, 3}});
   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const std::map<int, vic3::Character> vic3_characters = {
       {1, vic3::Character({.id = 1, .first_name = "Test", .last_name = "Mann"})},
       {2, vic3::Character({.id = 2, .is_female = true, .roles = {"agitator"}, .origin_country_id = 2})},
       {3, vic3::Character({.id = 3, .roles = {"agitator", "general"}})},
   };

   const vic3::World source_world(vic3::WorldOptions({.characters = vic3_characters}));

   const std::optional<Country> country_one = ConvertCountry(source_world,
       source_country_one,
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
       mappers::CultureGraphicsMapper({}),
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       characters,
       dummy_culture_queues);


   const auto expected_data_one = std::optional<Leader>({.sub_ideology = "despotism"});
   const auto expected_data_two = std::optional<Spy>({.nationalities = {"TAG", "TWO"}});
   const auto expected_data_three = std::optional<General>({.traits = {}});
   EXPECT_THAT(characters,
       testing::UnorderedElementsAre(testing::Pair(1,
                                         Character({
                                             .id = 1,
                                             .first_name = "Test",
                                             .last_name = "Mann",
                                             .leader_data = expected_data_one,
                                         })),
           testing::Pair(2,
               Character({
                   .id = 2,
                   .is_female = true,
                   .spy_data = expected_data_two,
               })),
           testing::Pair(3,
               Character({
                   .id = 3,
                   .general_data = expected_data_three,
               }))));
}

TEST(Hoi4worldCountriesCountryConverter, IdeologySupportIsConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2, .ig_ids = {1, 2}});
   std::map<int, hoi4::Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   const States states({.vic3_state_ids_to_hoi4_state_ids{{2, 4}}});

   std::map<int, vic3::InterestGroup> igs;
   igs.insert(
       {1, vic3::InterestGroup("test_group_one", 1, 0, 50.F, false, {"test_ideology_one", "test_ideology_two"})});
   igs.insert(
       {2, vic3::InterestGroup("test_group_two", 1, 0, 150.F, false, {"test_ideology_one", "test_ideology_three"})});

   vic3::Ideologies ideologies = vic3::Ideologies({
       {"test_ideology_one", vic3::Ideology({{"test_law_one", 1}})},
       {"test_ideology_two", vic3::Ideology({{"test_law_two", 2}})},
       {"test_ideology_three", vic3::Ideology({{"test_law_three", -2}})},
   });
   vic3::WorldOptions worldOptions = vic3::WorldOptions{
       .igs = igs,
       .ideologies = ideologies,
   };

   vic3::World source_world = vic3::World(worldOptions);

   mappers::ItemToPointsMap pointsMap;
   pointsMap.insert({"test_law_one",
       {
           {"democratic", 2},
           {"fascist", 3},
       }});
   pointsMap.insert({"test_law_two",
       {
           {"communist", 5},
           {"fascist", 3},
       }});
   pointsMap.insert({"test_law_three",
       {
           {"democratic", 5},
           {"communist", 7},
       }});
   const mappers::IdeologyMapper ideologyMapper = mappers::IdeologyMapper(pointsMap, {});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       ideologyMapper,
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetIdeologySupport(),
       testing::UnorderedElementsAre(testing::Pair("communist", 57),
           testing::Pair("democratic", 42),
           testing::Pair("fascist", 0),
           testing::Pair("neutrality", 1)));
}


TEST(Hoi4worldCountriesCountryConverter, IdeologySupportDefaultsToAllNeutrality)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   std::map<int, hoi4::Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;
   vic3::World source_world = vic3::World({});
   const States states({.vic3_state_ids_to_hoi4_state_ids{{2, 4}}});

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetIdeologySupport(), testing::UnorderedElementsAre(testing::Pair("neutrality", 100)));
}

TEST(Hoi4worldCountriesCountryConverter, StabilityDefaultsToZero)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   std::map<int, hoi4::Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   vic3::World source_world = vic3::World({});
   const std::map<int, int> vic3_state_ids_to_hoi4_state_ids{{2, 4}};

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       vic3_state_ids_to_hoi4_state_ids,
       {},
       mappers::IdeologyMapper({}, {}),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetStability(), 0.0F);
}

TEST(Hoi4worldCountriesCountryConverter, StabilityConvertsFromLegitimacy)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2, .legitimacy = 100});
   const vic3::Country source_country_two({.number = 1, .capital_state = 2, .legitimacy = 0});
   std::map<int, hoi4::Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   vic3::World source_world = vic3::World({});
   const std::map<int, int> vic3_state_ids_to_hoi4_state_ids{{2, 4}};

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       vic3_state_ids_to_hoi4_state_ids,
       {},
       mappers::IdeologyMapper({}, {}),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   const auto country_two = ConvertCountry(source_world,
       source_country_two,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       vic3_state_ids_to_hoi4_state_ids,
       {},
       mappers::IdeologyMapper({}, {}),
       {},
       {},
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}),
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetStability(), 0.60F);
   EXPECT_FLOAT_EQ(country_two->GetStability(), 0.00F);
}

}  // namespace hoi4