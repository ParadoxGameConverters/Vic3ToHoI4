#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountryConverter, TagIsFromSourceCountry)
{
   const mappers::CountryMapper country_mapper({{1, "T00"}, {2, "T01"}});
   const vic3::Country source_country_one({.number = 1, .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.number = 2, .color = commonItems::Color{std::array{2, 4, 6}}});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});
   const auto country_two =
       ConvertCountry(source_country_two, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

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
   const vic3::Country source_country_one({});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   EXPECT_EQ(country_one, std::nullopt);
}



TEST(Hoi4worldCountriesCountryConverter, NoCountryIfNoTagMapping)
{
   const mappers::CountryMapper country_mapper;
   const vic3::Country source_country_one({.number = 1});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   EXPECT_EQ(country_one, std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, CapitalStatesAreConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});
   const vic3::Country source_country_two({.number = 2, .capital_state = 3});

   const std::map<int, int> vic3_state_ids_to_hoi4_state_ids{{2, 4}, {3, 9}};

   const auto country_one = ConvertCountry(source_country_one,
       {},
       country_mapper,
       vic3_state_ids_to_hoi4_state_ids,
       {},
       mappers::IdeologyMapper({}),
       {},
       {},
       {},
       {},
       {});
   const auto country_two = ConvertCountry(source_country_two,
       {},
       country_mapper,
       vic3_state_ids_to_hoi4_state_ids,
       {},
       mappers::IdeologyMapper({}),
       {},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(4));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetCapitalState(), std::optional(9));
}


TEST(Hoi4worldCountriesCountryConverter, NoCapitalStateIfNoSourceCapitalState)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({
       .number = 1,
   });

   const std::map<int, int> vic3_state_ids_to_hoi4_state_ids{{2, 4}};

   const auto country_one = ConvertCountry(source_country_one,
       {},
       country_mapper,
       vic3_state_ids_to_hoi4_state_ids,
       {},
       mappers::IdeologyMapper({}),
       {},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, NoCapitalStateIfNoStateMappingAndNoStates)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, HighestVpStateBecomesCapitalIfCapitalNotConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});

   const auto country_one = ConvertCountry(source_country_one,
       {},
       country_mapper,
       {},
       {
           State(1, {.owner = "TAG", .victory_points = {{1, 1}, {2, 2}, {3, 3}}}),
           State(2, {.owner = "TAG", .victory_points = {{2, 2}, {4, 4}, {6, 6}}}),
           State(3, {.owner = "TAG", .victory_points = {{1, 1}, {2, 1}, {3, 1}}}),
       },
       mappers::IdeologyMapper({}),
       {},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(2));
}


TEST(Hoi4worldCountriesCountryConverter, HighestIndustryStateBecomesCapitalIfVpsAreSame)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});

   const auto country_one = ConvertCountry(source_country_one,
       {},
       country_mapper,
       {},
       {
           State(1, {.owner = "TAG", .civilian_factories = 1, .military_factories = 2, .dockyards = 3}),
           State(2, {.owner = "TAG", .civilian_factories = 2, .military_factories = 4, .dockyards = 6}),
           State(3, {.owner = "TAG", .civilian_factories = 1, .military_factories = 1, .dockyards = 1}),
       },
       mappers::IdeologyMapper({}),
       {},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(2));
}


TEST(Hoi4worldCountriesCountryConverter, HighestManpowerStateBecomesCapitalIfIndustriesAreSame)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});

   const auto country_one = ConvertCountry(source_country_one,
       {},
       country_mapper,
       {},
       {
           State(1, {.owner = "TAG", .manpower = 1234}),
           State(2, {.owner = "TAG", .manpower = 2468}),
           State(3, {.owner = "TAG", .manpower = 1111}),
       },
       mappers::IdeologyMapper({}),
       {},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(2));
}


TEST(Hoi4worldCountriesCountryConverter, LowestIdStateBecomesCapitalIfManpowersAreSame)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});

   const auto country_one = ConvertCountry(source_country_one,
       {},
       country_mapper,
       {},
       {
           State(3, {.owner = "TAG"}),
           State(1, {.owner = "TAG"}),
           State(2, {.owner = "TAG"}),
       },
       mappers::IdeologyMapper({}),
       {},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(1));
}


TEST(Hoi4worldCountriesCountryConverter, StatesNotOwnedByCountryCannotBecomeCapital)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});

   const auto country_one = ConvertCountry(source_country_one,
       {},
       country_mapper,
       {},
       {State(1, {}), State(2, {.owner = "TWO"})},
       mappers::IdeologyMapper({}),
       {},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, NonDemocraciesPickSentinelElectionYear)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1933.1.1"));
}


TEST(Hoi4worldCountriesCountryConverter, OutdatedElectionsExtrapolateToPresent)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .last_election = date("1894.4.23")});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1934.4.23"));
}


TEST(Hoi4worldCountriesCountryConverter, FutureElectionsFallbackToPresent)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .last_election = date("1937.2.15")});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1933.2.15"));
}


TEST(Hoi4worldCountriesCountryConverter, ContemporaryElectionsRemainUnchanged)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .last_election = date("1935.11.4")});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1935.11.4"));
}


TEST(Hoi4worldCountriesCountryConverter, InYearFutureElectionsAreCurrentCycle)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .last_election = date("1928.10.14")});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1932.10.14"));
}


TEST(Hoi4worldCountriesCountryConverter, InYearPastElectionsAreNextCycle)
{
   // When election is in the same year as the start date, kick it to next cycle when occurring on or before start date
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .last_election = date("1928.1.1")});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value().GetLastElection(), date("1936.1.1"));
}


TEST(Hoi4worldCountriesCountryConverter, RulingIdeologyCanBeConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .active_laws = {}});
   const vic3::Country source_country_two({.number = 1, .active_laws = {"law_landed_voting"}});

   const mappers::IdeologyMapper ideology_mapper({
       {"law_landed_voting", {{"democratic", 100}}},
   });

   const std::optional<Country> country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, ideology_mapper, {}, {}, {}, {}, {});
   const std::optional<Country> country_two =
       ConvertCountry(source_country_two, {}, country_mapper, {}, {}, ideology_mapper, {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetIdeology(), "neutrality");
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetIdeology(), "democratic");
}


TEST(Hoi4worldCountriesCountryConverter, TechnologiesAreConverted)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});

   const auto country_one = ConvertCountry(source_country_one,
       {"source_tech"},
       country_mapper,
       {},
       {},
       mappers::IdeologyMapper({}),
       {{{"source_tech"}, std::nullopt, {"dest_tech_one", "dest_tech_two"}}},
       {},
       {},
       {},
       {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetTechnologies().GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::nullopt, std::set<std::string>{"dest_tech_one", "dest_tech_two"})));
}


TEST(Hoi4worldCountriesCountryConverter, VariantsRequireAllRequiredTechs)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});

   const auto country_one = ConvertCountry(source_country_one,
       {"source_tech"},
       country_mapper,
       {},
       {},
       mappers::IdeologyMapper({}),
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
       });

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
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});

   const auto country_one = ConvertCountry(source_country_one,
       {"source_tech"},
       country_mapper,
       {},
       {},
       mappers::IdeologyMapper({}),
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
       });

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


TEST(Hoi4worldCountriesCountryConverter, IdeasDefaultsToEmpty)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_TRUE(country_one->GetIdeas().empty());
}


TEST(Hoi4worldCountriesCountryConverter, DecentrailzedCountriesGetDecentralizedIdea)
{
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({.number = 1, .capital_state = 2, .country_type = "decentralized"});

   const auto country_one =
       ConvertCountry(source_country_one, {}, country_mapper, {}, {}, mappers::IdeologyMapper({}), {}, {}, {}, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetIdeas(), testing::ElementsAre("decentralized"));
}

}  // namespace hoi4