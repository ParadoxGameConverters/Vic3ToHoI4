#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/country_rankings/country_rankings_importer.h"
#include "src/vic3_world/world/vic3_world.h"



TEST(Vic3worldCountriesVic3country, ColorCanBeSet)
{
   vic3::Country country({.number = 1});

   country.SetColor(commonItems::Color{std::array<int, 3>{1, 2, 3}});

   const commonItems::Color expected_color{std::array<int, 3>{1, 2, 3}};
   EXPECT_EQ(country.GetColor(), expected_color);
}


TEST(Vic3worldCountriesVic3country, GetAcquiredTechEmpty)
{
   const auto state = vic3::Country(vic3::CountryOptions{
       .number = 2,
   });
   const auto world = vic3::World({.acquired_technologies = {{1, {"tech_a", "tech_b"}}}});
   EXPECT_TRUE(state.GetAcquiredTechnologies(world).empty());
}


TEST(Vic3worldCountriesVic3country, GetAcquiredTech)
{
   const auto state = vic3::Country(vic3::CountryOptions{
       .number = 1,
   });
   const auto world = vic3::World({.acquired_technologies = {{1, {"tech_a", "tech_b"}}}});
   EXPECT_THAT(state.GetAcquiredTechnologies(world), testing::UnorderedElementsAre("tech_a", "tech_b"));
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

   const vic3::CountryRankings country_rankings = vic3::ImportCountryRankings(input);

   const vic3::Country c1({.number = 1});
   const vic3::Country c2({.number = 2});
   const vic3::Country c3({.number = 3});

   const vic3::World world({
       .countries =
           {
               {1, c1},
               {2, c2},
               {3, c3},
           },
       .country_rankings = country_rankings,
   });

   EXPECT_EQ(c1.GetCountryRankCategory(world), vic3::RankCategory::GreatPower);
   EXPECT_EQ(c2.GetCountryRankCategory(world), vic3::RankCategory::MajorPower);
   EXPECT_EQ(c3.GetCountryRankCategory(world), vic3::RankCategory::MinorPower);
}


TEST(Vic3worldCountriesVic3country, IsDecentralized_DecentralizedNation)
{
   vic3::Country c1({.country_type = "decentralized"});
   EXPECT_TRUE(c1.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, IsDecentralized_ColonialNation)
{
   vic3::Country c2({.country_type = "colonial"});
   EXPECT_FALSE(c2.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, IsDecentralized_UnrecognizedNation)
{
   vic3::Country c3({.country_type = "unrecognized"});
   EXPECT_FALSE(c3.IsDecentralized());
}


TEST(Vic3worldCountriesVic3country, IsDecentralized_RecognizedNation)
{
   vic3::Country c4({.country_type = "recognized"});
   EXPECT_FALSE(c4.IsDecentralized());
}

TEST(Vic3worldCountriesVic3country, IsRecognized_DecentralizedNation)
{
   vic3::Country c1({.country_type = "decentralized"});
   EXPECT_FALSE(c1.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, IsRecognized_ColonialNation)
{
   vic3::Country c2({.country_type = "colonial"});
   EXPECT_TRUE(c2.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, IsRecognized_UnrecognizedNation)
{
   vic3::Country c3({.country_type = "unrecognized"});
   EXPECT_FALSE(c3.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, IsRecognized_RecognizedNation)
{
   vic3::Country c4({.country_type = "recognized"});
   EXPECT_TRUE(c4.IsRecognized());
}


TEST(Vic3worldCountriesVic3country, DefaultLegitimacyZero)
{
   vic3::Country c4({});
   EXPECT_EQ(c4.GetLegitimacy(), 0);
}