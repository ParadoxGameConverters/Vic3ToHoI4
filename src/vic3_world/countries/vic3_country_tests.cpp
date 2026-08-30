#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/country_rankings/country_rankings_importer.h"
#include "src/vic3_world/world/vic3_world.h"



namespace vic3
{

TEST(Vic3worldCountriesVic3country, ColorCanBeSet)  // NOLINT(cert-err58-cpp)
{
   Country country({.number = 1});
   country.SetColor(commonItems::Color{std::array<int, 3>{1, 2, 3}});

   const commonItems::Color expected_color{std::array<int, 3>{1, 2, 3}};
   EXPECT_EQ(country.GetColor(), expected_color);
}


TEST(Vic3worldCountriesVic3country, GetAcquiredTechEmpty)  // NOLINT(cert-err58-cpp)
{
   const Country country = Country(CountryOptions{
       .number = 2,
   });
   const auto world = World({.acquired_technologies = {{1, {"tech_a", "tech_b"}}}});
   EXPECT_TRUE(country.GetAcquiredTechnologies(world).empty());
}


TEST(Vic3worldCountriesVic3country, GetAcquiredTech)  // NOLINT(cert-err58-cpp)
{
   const Country country = Country(CountryOptions{
       .number = 1,
   });
   const auto world = World({.acquired_technologies = {{1, {"tech_a", "tech_b"}}}});
   EXPECT_THAT(country.GetAcquiredTechnologies(world), testing::UnorderedElementsAre("tech_a", "tech_b"));
}


TEST(Vic3worldCountriesVic3country, PowerRankWorks)  // NOLINT(cert-err58-cpp)
{
   std::stringstream input;
   input << "={\n";
   input << "\tcountry_rankings={ {\n";
   input << "\t\t\trank=great_power\t\t\ttarget=great_power\t\t\tprestige=2034\n";
   input << "\t\t\tscore=3\n";
   input << "\t\t\tcountry=1\n";
   input << "\t\t}\n";
   input << " {\n";
   input << "\t\t\trank=great_power\t\t\ttarget=great_power\t\t\tprestige=1659\n";
   input << "\t\t\tscore=4\n";
   input << "\t\t\tcountry=18\n";
   input << "\t\t}\n";
   input << " {\n";
   input << "\t\t\trank=major_power\t\t\ttarget=major_power\t\t\tprestige=659\n";
   input << "\t\t\tscore=11\n";
   input << "\t\t\tcountry=2\n";
   input << "\t\t}\n";
   input << " {\n";
   input << "\t\t\trank=minor_power\t\t\ttarget=major_power\t\t\tprestige=559\n";
   input << "\t\t\tscore=16\n";
   input << "\t\t\tcountry=3\n";
   input << "\t\t}\n";
   input << " }\n";
   input << "}";

   const CountryRankings country_rankings = ImportCountryRankings(input);

   const Country country_one({.number = 1});
   const Country country_two({.number = 2});
   const Country country_three({.number = 3});

   const World world({
       .countries =
           {
               {1, country_one},
               {2, country_two},
               {3, country_three},
           },
       .country_rankings = country_rankings,
   });

   EXPECT_EQ(country_one.GetCountryRankCategory(world), RankCategory::kGreatPower);
   EXPECT_EQ(country_two.GetCountryRankCategory(world), RankCategory::kMajorPower);
   EXPECT_EQ(country_three.GetCountryRankCategory(world), RankCategory::kMinorPower);
}


TEST(Vic3worldCountriesVic3country, DecentralizedCountriesAreDecentralized)  // NOLINT(cert-err58-cpp)
{
   const Country country_one({.country_type = "decentralized"});
   EXPECT_TRUE(country_one.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, ColonalCountriesAreNotDecentralized)  // NOLINT(cert-err58-cpp)
{
   const Country country_two({.country_type = "colonial"});
   EXPECT_FALSE(country_two.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, UnrecognizedCountriesAreNotDecentralized)  // NOLINT(cert-err58-cpp)
{
   const Country country_three({.country_type = "unrecognized"});
   EXPECT_FALSE(country_three.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, RecognizedCountriesAreNotDecentralized)  // NOLINT(cert-err58-cpp)
{
   const Country country_four({.country_type = "recognized"});
   EXPECT_FALSE(country_four.IsDecentralized());
}

TEST(Vic3worldCountriesVic3country, DecentralizedCountriesAreNotRecognized)  // NOLINT(cert-err58-cpp)
{
   const Country country_one({.country_type = "decentralized"});
   EXPECT_FALSE(country_one.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, ColonialCountriesAreRecognized)  // NOLINT(cert-err58-cpp)
{
   const Country country_two({.country_type = "colonial"});
   EXPECT_TRUE(country_two.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, UnrecognizedCountriesAreNotRecognized)  // NOLINT(cert-err58-cpp)
{
   const Country country_three({.country_type = "unrecognized"});
   EXPECT_FALSE(country_three.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, RecognizedCountriesAreRecognized)  // NOLINT(cert-err58-cpp)
{
   const Country country_four({.country_type = "recognized"});
   EXPECT_TRUE(country_four.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, DefaultLegitimacyZero)  // NOLINT(cert-err58-cpp)
{
   const Country country_four({});
   EXPECT_EQ(country_four.GetLegitimacy(), 0);
}

}  // namespace vic3