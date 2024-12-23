#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/country_rankings/country_rankings_importer.h"
#include "src/vic3_world/world/vic3_world.h"



namespace vic3
{

TEST(Vic3worldCountriesVic3country, ColorCanBeSet)
{
   Country country({.number = 1});
   country.SetColor(commonItems::Color{std::array<int, 3>{1, 2, 3}});

   const commonItems::Color expected_color{std::array<int, 3>{1, 2, 3}};
   EXPECT_EQ(country.GetColor(), expected_color);
}


TEST(Vic3worldCountriesVic3country, GetAcquiredTechEmpty)
{
   const Country country = Country(CountryOptions{
       .number = 2,
   });
   const auto world = World({.acquired_technologies = {{1, {"tech_a", "tech_b"}}}});
   EXPECT_TRUE(country.GetAcquiredTechnologies(world).empty());
}


TEST(Vic3worldCountriesVic3country, GetAcquiredTech)
{
   const Country country = Country(CountryOptions{
       .number = 1,
   });
   const auto world = World({.acquired_technologies = {{1, {"tech_a", "tech_b"}}}});
   EXPECT_THAT(country.GetAcquiredTechnologies(world), testing::UnorderedElementsAre("tech_a", "tech_b"));
}


TEST(Vic3worldCountriesVic3country, PowerRankWorks)
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

   const Country c1({.number = 1});
   const Country c2({.number = 2});
   const Country c3({.number = 3});

   const World world({
       .countries =
           {
               {1, c1},
               {2, c2},
               {3, c3},
           },
       .country_rankings = country_rankings,
   });

   EXPECT_EQ(c1.GetCountryRankCategory(world), RankCategory::GreatPower);
   EXPECT_EQ(c2.GetCountryRankCategory(world), RankCategory::MajorPower);
   EXPECT_EQ(c3.GetCountryRankCategory(world), RankCategory::MinorPower);
}


TEST(Vic3worldCountriesVic3country, DecentralizedCountriesAreDecentralized)
{
   const Country c1({.country_type = "decentralized"});
   EXPECT_TRUE(c1.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, ColonalCountriesAreNotDecentralized)
{
   const Country c2({.country_type = "colonial"});
   EXPECT_FALSE(c2.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, UnrecognizedCountriesAreNotDecentralized)
{
   const Country c3({.country_type = "unrecognized"});
   EXPECT_FALSE(c3.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, RecognizedCountriesAreNotDecentralized)
{
   const Country c4({.country_type = "recognized"});
   EXPECT_FALSE(c4.IsDecentralized());
}

TEST(Vic3worldCountriesVic3country, DecentralizedCountriesAreNotRecognized)
{
   const Country c1({.country_type = "decentralized"});
   EXPECT_FALSE(c1.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, ColonialCountriesAreRecognized)
{
   const Country c2({.country_type = "colonial"});
   EXPECT_TRUE(c2.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, UnrecognizedCountriesAreNotRecognized)
{
   const Country c3({.country_type = "unrecognized"});
   EXPECT_FALSE(c3.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, RecognizedCountriesAreRecognized)
{
   const Country c4({.country_type = "recognized"});
   EXPECT_TRUE(c4.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, DefaultLegitimacyZero)
{
   const Country c4({});
   EXPECT_EQ(c4.GetLegitimacy(), 0);
}

}  // namespace vic3