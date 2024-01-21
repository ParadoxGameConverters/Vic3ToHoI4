#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
           EquipmentVariant("legacy_ship: no_required_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("legacy_ship: missing_required_tech_fails", "", {"required_tech_missing"}, {}, {}),
           EquipmentVariant("legacy_ship: present_required_tech_succeeds", "", {"required_tech_one"}, {}, {}),
           EquipmentVariant("legacy_ship: missing_and_present_required_techs_fails",
               "",
               {"required_tech_one", "required_tech_missing"},
               {},
               {}),
           EquipmentVariant("legacy_ship: all_present_required_techs_succeeds",
               "",
               {"required_tech_one", "required_tech_two"},
               {},
               {}),
       },
       {
           EquipmentVariant("ship: no_required_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("ship: missing_required_tech_fails", "", {"required_tech_missing"}, {}, {}),
           EquipmentVariant("ship: present_required_tech_succeeds", "", {"required_tech_one"}, {}, {}),
           EquipmentVariant("ship: missing_and_present_required_techs_fails",
               "",
               {"required_tech_one", "required_tech_missing"},
               {},
               {}),
           EquipmentVariant("ship: all_present_required_techs_succeeds",
               "",
               {"required_tech_one", "required_tech_two"},
               {},
               {}),
       },
       {
           EquipmentVariant("plane: no_required_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("plane: missing_required_tech_fails", "", {"required_tech_missing"}, {}, {}),
           EquipmentVariant("plane: present_required_tech_succeeds", "", {"required_tech_one"}, {}, {}),
           EquipmentVariant("plane: missing_and_present_required_techs_fails",
               "",
               {"required_tech_one", "required_tech_missing"},
               {},
               {}),
           EquipmentVariant("plane: all_present_required_techs_succeeds",
               "",
               {"required_tech_one", "required_tech_two"},
               {},
               {}),
       },
       {
           EquipmentVariant("tank: no_required_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("tank: missing_required_tech_fails", "", {"required_tech_missing"}, {}, {}),
           EquipmentVariant("tank: present_required_tech_succeeds", "", {"required_tech_one"}, {}, {}),
           EquipmentVariant("tank: missing_and_present_required_techs_fails",
               "",
               {"required_tech_one", "required_tech_missing"},
               {},
               {}),
           EquipmentVariant("tank: all_present_required_techs_succeeds",
               "",
               {"required_tech_one", "required_tech_two"},
               {},
               {}),
       },
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLegacyShipVariants(),
       testing::ElementsAre(EquipmentVariant("legacy_ship: no_required_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("legacy_ship: present_required_tech_succeeds", "", {"required_tech_one"}, {}, {}),
           EquipmentVariant("legacy_ship: all_present_required_techs_succeeds",
               "",
               {"required_tech_one", "required_tech_two"},
               {},
               {})));
   EXPECT_THAT(country_one->GetShipVariants(),
       testing::ElementsAre(EquipmentVariant("ship: no_required_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("ship: present_required_tech_succeeds", "", {"required_tech_one"}, {}, {}),
           EquipmentVariant("ship: all_present_required_techs_succeeds",
               "",
               {"required_tech_one", "required_tech_two"},
               {},
               {})));
   EXPECT_THAT(country_one->GetPlaneVariants(),
       testing::ElementsAre(EquipmentVariant("plane: no_required_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("plane: present_required_tech_succeeds", "", {"required_tech_one"}, {}, {}),
           EquipmentVariant("plane: all_present_required_techs_succeeds",
               "",
               {"required_tech_one", "required_tech_two"},
               {},
               {})));
   EXPECT_THAT(country_one->GetTankVariants(),
       testing::ElementsAre(EquipmentVariant("tank: no_required_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("tank: present_required_tech_succeeds", "", {"required_tech_one"}, {}, {}),
           EquipmentVariant("tank: all_present_required_techs_succeeds",
               "",
               {"required_tech_one", "required_tech_two"},
               {},
               {})));
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
           EquipmentVariant("legacy_ship: no_blocking_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("legacy_ship: having_blocking_tech_fails", "", {}, {"blocking_tech_one"}, {}),
           EquipmentVariant("legacy_ship: missing_blocking_tech_succeeds", "", {}, {"blocking_tech_missing"}, {}),
           EquipmentVariant("legacy_ship: missing_and_present_blocking_techs_fails",
               "",
               {},
               {"blocking_tech_one", "blocking_tech_missing"},
               {}),
       },
       {
           EquipmentVariant("ship: no_blocking_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("ship: having_blocking_tech_fails", "", {}, {"blocking_tech_one"}, {}),
           EquipmentVariant("ship: missing_blocking_tech_succeeds", "", {}, {"blocking_tech_missing"}, {}),
           EquipmentVariant("ship: missing_and_present_blocking_techs_fails",
               "",
               {},
               {"blocking_tech_one", "blocking_tech_missing"},
               {}),
       },
       {
           EquipmentVariant("plane: no_blocking_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("plane: having_blocking_tech_fails", "", {}, {"blocking_tech_one"}, {}),
           EquipmentVariant("plane: missing_blocking_tech_succeeds", "", {}, {"blocking_tech_missing"}, {}),
           EquipmentVariant("plane: missing_and_present_blocking_techs_fails",
               "",
               {},
               {"blocking_tech_one", "blocking_tech_missing"},
               {}),
       },
       {
           EquipmentVariant("tank: no_blocking_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("tank: having_blocking_tech_fails", "", {}, {"blocking_tech_one"}, {}),
           EquipmentVariant("tank: missing_blocking_tech_succeeds", "", {}, {"blocking_tech_missing"}, {}),
           EquipmentVariant("tank: missing_and_present_blocking_techs_fails",
               "",
               {},
               {"blocking_tech_one", "blocking_tech_missing"},
               {}),
       },
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetLegacyShipVariants(),
       testing::ElementsAre(EquipmentVariant("legacy_ship: no_blocking_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("legacy_ship: missing_blocking_tech_succeeds", "", {}, {"blocking_tech_missing"}, {})));
   EXPECT_THAT(country_one->GetShipVariants(),
       testing::ElementsAre(EquipmentVariant("ship: no_blocking_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("ship: missing_blocking_tech_succeeds", "", {}, {"blocking_tech_missing"}, {})));
   EXPECT_THAT(country_one->GetPlaneVariants(),
       testing::ElementsAre(EquipmentVariant("plane: no_blocking_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("plane: missing_blocking_tech_succeeds", "", {}, {"blocking_tech_missing"}, {})));
   EXPECT_THAT(country_one->GetTankVariants(),
       testing::ElementsAre(EquipmentVariant("tank: no_blocking_techs_automatically_succeeds", "", {}, {}, {}),
           EquipmentVariant("tank: missing_blocking_tech_succeeds", "", {}, {"blocking_tech_missing"}, {})));
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);
   ASSERT_TRUE(country_one.has_value());
   EXPECT_TRUE(country_one->GetIdeas().empty());
}


TEST(Hoi4worldCountriesCountryConverter, DecentralizedCountriesGetDecentralizedIdeas)
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       leader_type_mapper,
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       culture_graphics_mapper,
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper({}),
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
       {},
       mappers::CultureGraphicsMapper({}),
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
   pointsMap.insert({"test_law_one", {{"democratic", 2}, {"fascist", 3}, {"neutrality", 0}}});
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

   // TG1 has testlaw1: support, testlaw2: strong support
   // TG2 has testlaw1: support, testlaw3: strongly oppose
   // this maps to:
   // TG1: democratic2, fasc3 + com10,fasc6 = (2,10,9,0) (total 21) * 50clout
   // TG1 has a 0, so normalization has no offset
   // TG2: democratic2,fasc3+ democratic-10, com-14 = (-8,-14,3,0) (total 25) *150clout
   // total is then (-1100,-1600,900,0)
   // get all positive: (500,0,2500,1600).
   // divide by total of 4600: (.10,0,.54,.36)

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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetIdeologySupport(),
       testing::UnorderedElementsAre(testing::Pair("democratic", 10),
           testing::Pair("communist", 0),
           testing::Pair("fascist", 54),
           testing::Pair("neutrality", 36)));
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
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
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       States{.vic3_state_ids_to_hoi4_state_ids = vic3_state_ids_to_hoi4_state_ids},
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
   mappers::TemplateMap templates;

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       States{.vic3_state_ids_to_hoi4_state_ids = vic3_state_ids_to_hoi4_state_ids},
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

   const auto country_two = ConvertCountry(source_world,
       source_country_two,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       States{.vic3_state_ids_to_hoi4_state_ids = vic3_state_ids_to_hoi4_state_ids},
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

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetStability(), 0.60F);
   EXPECT_FLOAT_EQ(country_two->GetStability(), 0.00F);
}


TEST(Hoi4worldCountriesCountryConverter, UnitsAreConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   vic3::Buildings buildings(std::map<int, std::vector<vic3::Building>>{
       {1,
           {vic3::Building(vic3::BuildingType::Barracks,
               1,
               0,
               1,
               std::vector<std::string>{"trench_infantry", "field_hospitals"})}},
       {2,
           {vic3::Building(vic3::BuildingType::Barracks,
               2,
               0,
               1,
               std::vector<std::string>{"trench_infantry", "wound_dressing", "militia"})}},
   });
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.buildings = buildings});
   const vic3::Country source_country_one({.number = 1});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   const States states({.states{
                            State(1,
                                {
                                    .owner = "TAG",
                                    .provinces = {1},
                                }),
                            State(2,
                                {
                                    .owner = "TAG",
                                    .provinces = {2},
                                }),
                            State(3,
                                {
                                    .owner = "TAG",
                                    .provinces = {3},
                                }),
                        },
       .vic3_state_ids_to_hoi4_state_ids{{1, 1}, {2, 2}, {3, 3}},
       .hoi4_state_ids_to_owner{{1, "TAG"}, {2, "TAG"}, {3, "TAG"}}});
   mappers::TemplateMap templates{
       {"trench_infantry", mappers::BattalionTemplate(30, {{"infantry", 2.0F}})},
       {"militia", mappers::BattalionTemplate(40, {{"militia", 1.0F}})},
       {"field_hospitals", mappers::BattalionTemplate(10, {})},
       {"wound_dressing", mappers::BattalionTemplate(0, {})},
   };
   std::vector<std::string> template_names{"Heavy Infantry", "Cavalry", "Light Infantry", "Ghosts"};
   hoi4::UnitCount heavy_infantry{{"infantry", 2}};
   hoi4::UnitCount light_infantry{{"infantry", 1}};
   hoi4::UnitCount cavalry{{"cavalry", 1}};
   hoi4::UnitCount artillery{{"artillery", 1}};
   hoi4::UnitCount empty;
   std::vector<hoi4::DivisionTemplate> division_templates{
       hoi4::DivisionTemplate(template_names[0], heavy_infantry, artillery),
       hoi4::DivisionTemplate(template_names[1], cavalry, empty),
       hoi4::DivisionTemplate(template_names[2], light_infantry, empty),
       hoi4::DivisionTemplate(template_names[3], empty, empty),
   };

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
       division_templates,
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetUnits(),
       testing::UnorderedElementsAre(hoi4::Unit{"Light Infantry", 40, 1},
           hoi4::Unit{"Light Infantry", 40, 1},
           hoi4::Unit{"Light Infantry", 70, 2},
           hoi4::Unit{"Light Infantry", 70, 2}));
}


TEST(Hoi4worldCountriesCountryConverter, WarsDefaultToEmpty)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "T00"}});
   const vic3::Country source_country_one({.number = 1, .color = commonItems::Color{std::array{1, 2, 3}}});
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_TRUE(country_one->GetWars().empty());
}


TEST(Hoi4worldCountriesCountryConverter, WarsCanBeAdded)
{
   const vic3::World source_world = vic3::World(vic3::WorldOptions());
   const mappers::CountryMapper country_mapper({{1, "T00"}});
   const vic3::Country source_country_one({.number = 1, .color = commonItems::Color{std::array{1, 2, 3}}});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   mappers::TemplateMap templates;

   auto country_one = ConvertCountry(source_world,
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       dummy_characters,
       dummy_culture_queues);

   const War war_one({.original_attacker = "ONE"});
   country_one->AddWar(war_one);
   const War war_two({.original_attacker = "TWO"});
   country_one->AddWar(war_two);


   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetWars(), testing::ElementsAre(war_one, war_two));
}


TEST(Hoi4worldCountriesCountryConverter, MonarchIdeaCanBeAdded)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}});
   const vic3::World source_world = vic3::World(vic3::WorldOptions{});
   const vic3::Country source_country_one(
       {.number = 1, .active_laws = {"law_monarchy"}, .last_election = date("1932.1.1"), .head_of_state_id = 42});
   std::map<int, Character> characters = {{42, Character({.first_name = "FirstName", .last_name = "LastName"})}};
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   const States states({.states{
                            State(1,
                                {
                                    .owner = "TAG",
                                    .provinces = {1},
                                }),
                        },
       .vic3_state_ids_to_hoi4_state_ids{{1, 1}},
       .hoi4_state_ids_to_owner{{1, "TAG"}}});
   mappers::TemplateMap templates;

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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       {},
       characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetIdeas(), testing::ElementsAre("TAG_FirstName_LastName"));
   EXPECT_THAT(country_one->GetMonarchIdeaIds(), 42);
}

TEST(Hoi4worldCountriesCountryConverter, ConvoysConvert)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "ABC"}});
   const vic3::Country source_country_one({.number = 1});
   const vic3::Country source_country_two({.number = 2});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   const std::map<int, vic3::State> vic3_states{
       {1, vic3::State({.id = 1})},
       {2, vic3::State({.id = 2})},
       {3, vic3::State({.id = 3})},
   };
   vic3::Buildings buildings(std::map<int, std::vector<vic3::Building>>{
       {1, {vic3::Building(vic3::BuildingType::Port, 1, 0, 1, std::vector<std::string>{"dummy", "pm_port_1"})}},
       {2, {vic3::Building(vic3::BuildingType::Port, 2, 0, 1, std::vector<std::string>{"pm_port_2"})}},
       {3, {vic3::Building(vic3::BuildingType::Port, 3, 0, 1, std::vector<std::string>{"pm_port_3"})}},
   });
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.states = vic3_states, .buildings = buildings});
   const States states({.states{
                            State(1,
                                {
                                    .owner = "TAG",
                                    .provinces = {1},
                                }),
                            State(2,
                                {
                                    .owner = "TAG",
                                    .provinces = {2},
                                }),
                            State(3,
                                {
                                    .owner = "ABC",
                                    .provinces = {3},
                                }),
                        },
       .vic3_state_ids_to_hoi4_state_ids{{1, 1}, {2, 2}, {3, 3}},
       .hoi4_state_ids_to_owner{{1, "TAG"}, {2, "TAG"}, {3, "ABC"}}});
   mappers::TemplateMap templates;
   ConvoyDistributor convoy_distributor(111, {{"pm_port_1", 1}, {"pm_port_2", 10}, {"pm_port_3", 100}});
   convoy_distributor.CalculateStateWeights(source_world);

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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       convoy_distributor,
       {},
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
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       convoy_distributor,
       {},
       dummy_characters,
       dummy_culture_queues);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetConvoys(), 11);
   ASSERT_TRUE(country_two.has_value());
   EXPECT_THAT(country_two->GetConvoys(), 100);
}

TEST(Hoi4worldCountriesCountryConverter, NaviesConvert)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "ABC"}});
   const vic3::Country source_country_one({.number = 1});
   const vic3::Country source_country_two({.number = 2});
   std::map<int, Character> dummy_characters;
   std::map<std::string, mappers::CultureQueue> dummy_culture_queues;
   const std::map<int, vic3::State> vic3_states{
       {1, vic3::State({.id = 1})},
       {2, vic3::State({.id = 2})},
       {3, vic3::State({.id = 3})},
   };
   vic3::Buildings buildings(std::map<int, std::vector<vic3::Building>>{
       {1, {vic3::Building(vic3::BuildingType::NavalBase, 1, 0, 10, std::vector<std::string>{"pm_victorian"})}},
       {2, {vic3::Building(vic3::BuildingType::NavalBase, 2, 0, 10, std::vector<std::string>{"pm_ancient"})}},
       {3, {vic3::Building(vic3::BuildingType::NavalBase, 3, 0, 10, std::vector<std::string>{"pm_modern"})}},
   });
   const vic3::World source_world = vic3::World(vic3::WorldOptions{.states = vic3_states, .buildings = buildings});
   const States states({.states{
                            State(1,
                                {
                                    .owner = "TAG",
                                    .provinces = {1},
                                    .naval_base_location = 1,
                                }),
                            State(2,
                                {
                                    .owner = "TAG",
                                    .provinces = {2},
                                    .naval_base_location = 2,
                                }),
                            State(3,
                                {
                                    .owner = "ABC",
                                    .provinces = {3},
                                    .naval_base_location = 3,
                                }),
                        },
       .vic3_state_ids_to_hoi4_state_ids{{1, 1}, {2, 2}, {3, 3}},
       .hoi4_state_ids_to_owner{{1, "TAG"}, {2, "TAG"}, {3, "ABC"}}});
   mappers::TemplateMap templates;

   std::vector<EquipmentVariant> legacy_ships{
       EquipmentVariant("Basic Ship", "legacy_basic_ship", {}, {}, {}),
       EquipmentVariant("1936 Ship", "legacy_1936_ship", {}, {}, {}),
   };
   std::vector<EquipmentVariant> mtg_ships{
       EquipmentVariant("Basic Ship", "mtg_basic_ship", {}, {}, {}),
       EquipmentVariant("1936 Ship", "mtg_1936_ship", {}, {}, {}),
   };
   std::vector<TaskForceTemplate> task_force_templates{
       {{{"pm_victorian", 5}}, {Ship("Cruiser", "basic_ship", "mtg_basic_ship", "legacy_basic_ship", "Basic Ship")}},
       {{{"pm_modern", 10}}, {Ship("Battleship", "1936_ship", "mtg_1936_ship", "legacy_1936_ship", "1936 Ship")}},
   };

   const auto country_one = ConvertCountry(source_world,
       source_country_one,
       commonItems::LocalizationDatabase{{}, {}},
       country_mapper,
       states,
       mappers::IdeologyMapper({}, {}),
       mappers::UnitMapper(templates),
       {},
       legacy_ships,
       mtg_ships,
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       task_force_templates,
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
       legacy_ships,
       mtg_ships,
       {},
       {},
       {},
       mappers::CultureGraphicsMapper{{}},
       mappers::LeaderTypeMapper({}),
       mappers::CharacterTraitMapper({}, {}, {}, {}),
       {0, {}},
       task_force_templates,
       dummy_characters,
       dummy_culture_queues);

   EXPECT_THAT(country_one.value_or(Country({})).GetTaskForces(),
       testing::UnorderedElementsAre(TaskForce{
           .name = "1. Fleet",
           .ships =
               {
                   Ship("Cruiser 1", "basic_ship", "mtg_basic_ship", "legacy_basic_ship", "Basic Ship"),
                   Ship("Cruiser 2", "basic_ship", "mtg_basic_ship", "legacy_basic_ship", "Basic Ship"),
               },
           .location = 1,
       }));
   EXPECT_THAT(country_two.value_or(Country({})).GetTaskForces(),
       testing::UnorderedElementsAre(TaskForce{
           .name = "1. Fleet",
           .ships =
               {
                   Ship("Battleship 1", "1936_ship", "mtg_1936_ship", "legacy_1936_ship", "1936 Ship"),
               },
           .location = 3,
       }));
}


}  // namespace hoi4
