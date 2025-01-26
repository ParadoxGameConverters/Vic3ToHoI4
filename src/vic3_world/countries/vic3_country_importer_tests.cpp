#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

#include "src/vic3_world/countries/vic3_country_importer.h"



namespace vic3
{


TEST(Vic3WorldCountriesCountryImporter, NumberIsAsSet)
{
   std::stringstream input;
   const auto country = CountryImporter{}.ImportCountry(42, input, {});

   EXPECT_TRUE(country.has_value());
   EXPECT_EQ(country.value_or(Country({})).GetNumber(), 42);
}


TEST(Vic3WorldCountriesCountryImporter, DefaultsAreDefaulted)
{
   std::stringstream input;
   const auto country = CountryImporter{}.ImportCountry(0, input, {});

   EXPECT_TRUE(country.has_value());
   EXPECT_TRUE(country.value_or(Country({})).GetTag().empty());
   EXPECT_FALSE(country.value_or(Country({})).IsDead());
   EXPECT_FALSE(country.value_or(Country({})).GetDynamicName());
   EXPECT_FALSE(country.value_or(Country({})).GetDynamicAdjective());
   EXPECT_FALSE(country.value_or(Country({})).GetUseOverlordPrefix());
   EXPECT_EQ(country.value_or(Country({})).GetColor(), commonItems::Color(std::array{0, 0, 0}));
   EXPECT_EQ(country.value_or(Country({})).GetCapitalState(), std::nullopt);
   EXPECT_FALSE(country.value_or(Country({})).IsDecentralized());
   EXPECT_TRUE(country.value_or(Country({})).GetActiveLaws().empty());
   EXPECT_TRUE(country.value_or(Country({})).GetPrimaryCultureIds().empty());
   EXPECT_EQ(country.value_or(Country({})).GetLastElection(), std::nullopt);
   EXPECT_EQ(country.value_or(Country({})).GetHeadOfStateId(), 0);
   EXPECT_FALSE(country.value_or(Country({})).IsCivilWarCountry());
   EXPECT_EQ(country.value_or(Country({})).GetLegitimacy(), 0);
}


TEST(Vic3WorldCountriesCountryImporter, ItemsCanBeInput)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdefinition=\"TAG\"";
   input << "\tdynamic_name={\n";
   input << "\t\tdynamic_country_name=\"dynamic_name\"";
   input << "\t\tdynamic_country_adjective=\"dynamic_adjective\"";
   input << "\t\tuse_overlord_prefix=yes";
   input << "\t}\n";
   input << "\tmap_color=rgb {\n";
   input << "\t\t8 89 54\n";
   input << "\t}\n";
   input << "\tcapital=12345\n";
   input << "\tcountry_type=\"decentralized\"\n";
   input << "\tcultures={ 35 7 }\n";
   input << "\truler=10\n";
   input << "}";
   const auto country = CountryImporter{}.ImportCountry(42, input, {});

   EXPECT_TRUE(country.has_value());
   EXPECT_EQ(country.value_or(Country({})).GetNumber(), 42);
   EXPECT_EQ(country.value_or(Country({})).GetTag(), "TAG");
   EXPECT_EQ(country.value_or(Country({})).GetDynamicName().value_or(""), "dynamic_name");
   EXPECT_EQ(country.value_or(Country({})).GetDynamicAdjective().value_or(""), "dynamic_adjective");
   EXPECT_TRUE(country.value_or(Country({})).GetUseOverlordPrefix());
   EXPECT_EQ(country.value_or(Country({})).GetColor(), commonItems::Color(std::array{8, 89, 54}));
   EXPECT_EQ(country.value_or(Country({})).GetCapitalState(), std::optional<int>(12345));
   EXPECT_TRUE(country.value_or(Country({})).IsDecentralized());
   EXPECT_FALSE(country.value_or(Country({})).IsRecognized());
   EXPECT_THAT(country.value_or(Country({})).GetPrimaryCultureIds(), testing::UnorderedElementsAre(35, 7));
   EXPECT_EQ(country.value_or(Country({})).GetHeadOfStateId(), 10);
}


TEST(Vic3WorldCountriesCountryImporter, LongSentinelValueMapsToNegativeOne)
{
   std::stringstream input;
   input << "={\n";
   input << "\tcapital=4294967295\n";
   input << "\truler=4294967295\n";
   input << "}";
   const auto country = CountryImporter{}.ImportCountry(42, input, {});

   EXPECT_TRUE(country.has_value());
   EXPECT_EQ(country.value_or(Country({})).GetCapitalState(), std::optional<int>(-1));
   EXPECT_EQ(country.value_or(Country({})).GetHeadOfStateId(), -1);
}


TEST(Vic3WorldCountriesCountryImporter, Pre1_5DynamicsCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdynamic_country_name=\"dynamic_name\"";
   input << "\tdynamic_country_adjective=\"dynamic_adjective\"";
   input << "}";
   const auto country = CountryImporter{}.ImportCountry(42, input, {});

   EXPECT_TRUE(country.has_value());
   EXPECT_EQ(country.value_or(Country({})).GetDynamicName().value_or(""), "dynamic_name");
   EXPECT_EQ(country.value_or(Country({})).GetDynamicAdjective().value_or(""), "dynamic_adjective");
}


TEST(Vic3WorldCountriesCountryImporter, ColorCanBeFromColorDefinitions)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdefinition=\"TAG\"";
   input << "}";
   const auto country = CountryImporter{}.ImportCountry(42, input, {{"TAG", commonItems::Color(std::array{1, 2, 3})}});

   EXPECT_TRUE(country.has_value());
   EXPECT_EQ(country.value_or(Country({})).GetColor(), commonItems::Color(std::array{1, 2, 3}));
}


TEST(Vic3WorldCountriesCountryImporter, MultipleCountriesCanBeImported)
{
   CountryImporter country_importer;

   std::stringstream input_one;
   input_one << "={\n";
   input_one << "\tdefinition=\"TAG\"";
   input_one << "\tcapital=12345\n";
   input_one << "}";
   const auto country_one =
       country_importer.ImportCountry(144, input_one, {{"TAG", commonItems::Color(std::array{1, 2, 3})}});

   std::stringstream input_two;
   const auto country_two =
       country_importer.ImportCountry(169, input_two, {{"TAG", commonItems::Color(std::array{1, 2, 3})}});

   EXPECT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one.value_or(Country({})).GetNumber(), 144);
   EXPECT_EQ(country_one.value_or(Country({})).GetTag(), "TAG");
   EXPECT_EQ(country_one.value_or(Country({})).GetColor(), commonItems::Color(std::array{1, 2, 3}));
   EXPECT_EQ(country_one.value_or(Country({})).GetCapitalState(), std::optional<int>(12345));

   EXPECT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two.value_or(Country({})).GetNumber(), 169);
   EXPECT_TRUE(country_two.value_or(Country({})).GetTag().empty());
   EXPECT_EQ(country_two.value_or(Country({})).GetColor(), commonItems::Color(std::array{0, 0, 0}));
   EXPECT_EQ(country_two.value_or(Country({})).GetCapitalState(), std::nullopt);
}

TEST(Vic3WorldCountriesCountryImporter, CivilWarDetected)
{
   CountryImporter country_importer;

   std::stringstream input_one;
   input_one << "={\n";
   input_one << "\tdefinition=\"TAG\"";
   input_one << "\tcivil_war=\"yes\"";
   input_one << "\tcapital=12345\n";
   input_one << "}";
   const auto country_one =
       country_importer.ImportCountry(144, input_one, {{"TAG", commonItems::Color(std::array{1, 2, 3})}});

   EXPECT_TRUE(country_one.has_value());
   EXPECT_TRUE(country_one.value_or(Country({})).IsCivilWarCountry());
}

TEST(Vic3WorldCountriesCountryImporter, ActiveLawsCanBeSet)
{
   Country country({});
   country.SetActiveLaws({"test_law_one", "test_law_two"});

   EXPECT_THAT(country.GetActiveLaws(), testing::UnorderedElementsAre("test_law_one", "test_law_two"));
}


TEST(Vic3WorldCountriesCountryImporter, LastElectionCanBeSet)
{
   Country country({});
   country.SetLastElection(date{"1929.9.9"});

   EXPECT_TRUE(country.GetLastElection());
   EXPECT_EQ(country.GetLastElection().value_or(date{}), date{"1929.9.9"});
}


TEST(Vic3WorldCountriesCountryImporter, CharacterIdsCanBeSet)
{
   Country country({});
   country.SetCharacterIds({1, 2, 3});

   EXPECT_EQ(country.GetCharacterIds(), std::vector({1, 2, 3}));
}


TEST(Vic3WorldCountriesCountryImporter, IgIdsCanBeAdded)
{
   Country country({});
   country.AddInterestGroupId(1);
   country.AddInterestGroupId(2);
   country.AddInterestGroupId(3);

   EXPECT_EQ(country.GetInterestGroupIds(), std::vector({1, 2, 3}));
}


TEST(Vic3WorldCountriesCountryImporter, DeadCountriesAreImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdefinition=\"TAG\"";
   input << "\tcapital=12345\n";
   input << "\tdead=yes\n";
   input << "}";
   const auto country = CountryImporter{}.ImportCountry(0, input, {});

   EXPECT_TRUE(country.has_value());
   EXPECT_TRUE(country.value_or(Country({})).IsDead());
}

TEST(Vic3WorldCountriesCountryImporter, CountersSectionParsed)
{
   CountryImporter country_importer;

   std::stringstream input_one;
   input_one << "={\n";
   input_one << "\tdefinition=\"TAG\"";
   input_one << "\tcapital=12345\n";
   input_one << "\tcounters={\n";
   input_one << "\tbuildings=279\n";
   input_one << "\tlegitimacy=48\n";
   input_one << "\t}\n";
   input_one << "}";
   const auto country_one =
       country_importer.ImportCountry(144, input_one, {{"TAG", commonItems::Color(std::array{1, 2, 3})}});

   EXPECT_EQ(country_one.value_or(Country({})).GetLegitimacy(), 48);
}


}  // namespace vic3