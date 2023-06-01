#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/vic3_country_importer.h"



namespace vic3
{


TEST(Vic3WorldCountriesCountryImporter, NumberIsAsSet)
{
   std::stringstream input;
   const auto country = CountryImporter{}.ImportCountry(42, input, {});

   EXPECT_EQ(country.GetNumber(), 42);
}


TEST(Vic3WorldCountriesCountryImporter, DefaultsAreDefaulted)
{
   std::stringstream input;
   const auto country = CountryImporter{}.ImportCountry(0, input, {});

   EXPECT_TRUE(country.GetTag().empty());
   EXPECT_EQ(country.GetColor(), commonItems::Color(std::array{0, 0, 0}));
   EXPECT_EQ(country.GetCapitalState(), std::nullopt);
   EXPECT_FALSE(country.IsDecentralized());
   EXPECT_TRUE(country.GetActiveLaws().empty());
   EXPECT_TRUE(country.GetPrimaryCultureIds().empty());
   EXPECT_EQ(country.GetLastElection(), std::nullopt);
}


TEST(Vic3WorldCountriesCountryImporter, ItemsCanBeInput)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdefinition=\"TAG\"";
   input << "\tcapital=12345\n";
   input << "\tcountry_type=\"decentralized\"\n";
   input << "\tcultures={ 35 7 }\n";
   input << "}";
   const auto country = CountryImporter{}.ImportCountry(42, input, {{"TAG", commonItems::Color(std::array{1, 2, 3})}});

   EXPECT_EQ(country.GetNumber(), 42);
   EXPECT_EQ(country.GetTag(), "TAG");
   EXPECT_EQ(country.GetColor(), commonItems::Color(std::array{1, 2, 3}));
   EXPECT_EQ(country.GetCapitalState(), std::optional<int>(12345));
   EXPECT_TRUE(country.IsDecentralized());
   EXPECT_THAT(country.GetPrimaryCultureIds(), testing::UnorderedElementsAre(35, 7));
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

   EXPECT_EQ(country_one.GetNumber(), 144);
   EXPECT_EQ(country_one.GetTag(), "TAG");
   EXPECT_EQ(country_one.GetColor(), commonItems::Color(std::array{1, 2, 3}));
   EXPECT_EQ(country_one.GetCapitalState(), std::optional<int>(12345));

   EXPECT_EQ(country_two.GetNumber(), 169);
   EXPECT_TRUE(country_two.GetTag().empty());
   EXPECT_EQ(country_two.GetColor(), commonItems::Color(std::array{0, 0, 0}));
   EXPECT_EQ(country_two.GetCapitalState(), std::nullopt);
}


TEST(Vic3WorldCountriesCountryImporter, ActiveLawsCanBeSet)
{
   std::stringstream input;
   Country country = CountryImporter{}.ImportCountry(0, input, {});
   country.SetActiveLaws({"test_law_one", "test_law_two"});

   EXPECT_THAT(country.GetActiveLaws(), testing::UnorderedElementsAre("test_law_one", "test_law_two"));
}


TEST(Vic3WorldCountriesCountryImporter, LastElectionCanBeSet)
{
   std::stringstream input;
   Country country = CountryImporter{}.ImportCountry(0, input, {});
   country.SetLastElection(date{"1929.9.9"});

   ASSERT_TRUE(country.GetLastElection());
   EXPECT_EQ(*country.GetLastElection(), date{"1929.9.9"});
}


}  // namespace vic3