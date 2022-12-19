#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/vic3_countries_importer.h"



namespace vic3
{

TEST(Vic3WorldCountriesVic3CountriesImporter, NoCountriesByDefault)
{
   std::stringstream input;
   const auto countries = CountriesImporter{}.ImportCountries(input);

   EXPECT_TRUE(countries.empty());
}


TEST(Vic3WorldCountriesVic3CountriesImporter, CountriesCanBeImported)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase = {\n";
   input << "0={\n";
   input << "\tdefinition=\"TAG\"\n";
   input << "}\n";
   input << "1={\n";
   input << "\tdefinition=\"TWO\"\n";
   input << "\t}\n";
   input << "}\n";
   const auto countries = CountriesImporter{}.ImportCountries(input);

   EXPECT_THAT(countries,
       testing::UnorderedElementsAre(testing::Pair(0, Country({.tag = "TAG"})),
           testing::Pair(1, Country({.tag = "TWO"}))));
}


TEST(Vic3WorldCountriesVic3CountriesImporter, CountryIndexesCanBeSkipped)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase = {\n";
   input << "0=none\n";
   input << "1={\n";
   input << "\tdefinition=\"TWO\"\n";
   input << "\t}\n";
   input << "}";
   const auto countries = CountriesImporter{}.ImportCountries(input);

   EXPECT_THAT(countries, testing::UnorderedElementsAre(testing::Pair(1, Country({.tag = "TWO"}))));
}

}  // namespace vic3