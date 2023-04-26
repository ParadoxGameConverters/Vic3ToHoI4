#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/country_rankings/country_rankings_importer.h"



namespace vic3
{

TEST(Vic3WorldCountryRankingsCountryRankingsImporter, PowersDefaultToEmpty)
{
   std::stringstream input;

   const CountryRankings country_rankings = ImportCountryRankings(input);

   EXPECT_TRUE(country_rankings.GetGreatPowers().empty());
   EXPECT_TRUE(country_rankings.GetMajorPowers().empty());
}


TEST(Vic3WorldCountryRankingsCountryRankingsImporter, PowersCanBeImported)
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
   input << "\t\t\tcountry=183\n";
   input << "\t\t}\n";
   input << " {\n";
   input << "\t\t\trank=major_power\t\t\ttarget=major_power\t\t\tprestige=559\n";
   input << "\t\t\tscore=16\n";
   input << "\t\t\tcountry=283\n";
   input << "\t\t}\n";
   input << " }\n";
   input << "}";

   const CountryRankings country_rankings = ImportCountryRankings(input);

   EXPECT_THAT(country_rankings.GetGreatPowers(), testing::UnorderedElementsAre(1, 18));
   EXPECT_THAT(country_rankings.GetMajorPowers(), testing::ElementsAre(183, 283));
}


TEST(Vic3WorldCountryRankingsCountryRankingsImporter, MissingCountryNumberIsSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tcountry_rankings={ {\n";
   input << "\t\t\trank=great_power\t\t\ttarget=great_power\t\t\tprestige=2034\n";
   input << "\t\t\tscore=3\n";
   input << "\t\t}\n";
   input << " {\n";
   input << "\t\t\trank=major_power\t\t\ttarget=major_power\t\t\tprestige=659\n";
   input << "\t\t\tscore=11\n";
   input << "\t\t}\n";
   input << "}";

   const CountryRankings country_rankings = ImportCountryRankings(input);

   EXPECT_TRUE(country_rankings.GetGreatPowers().empty());
   EXPECT_TRUE(country_rankings.GetMajorPowers().empty());
}

}  // namespace vic3