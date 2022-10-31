#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/countries_importer.h"



namespace vic3
{

TEST(Vic3WorldStateVic3CountriesImporter, NoCountriesByDefault)
{
   std::stringstream input;
   const auto countries = CountriesImporter{}.ImportCountries(input);

   EXPECT_TRUE(countries.empty());
}


TEST(Vic3WorldStateVic3CountriesImporter, CountriesCanBeImported)
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
       testing::UnorderedElementsAre(testing::Pair(0, Country("TAG")), testing::Pair(1, Country("TWO"))));
}


TEST(Vic3WorldStateVic3CountriesImporter, CountryIndexesCanBeSkipped)
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

   EXPECT_THAT(countries, testing::UnorderedElementsAre(testing::Pair(1, Country("TWO"))));
}

}  // namespace vic3