#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/vic3_countries_importer.h"



namespace vic3
{

TEST(Vic3WorldCountriesVic3CountriesImporter, NoCountriesByDefault)
{
   std::stringstream input;
   const auto countries = ImportCountries({}, input);

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
   const auto countries = ImportCountries(
       {
           {"TAG", commonItems::Color(std::array{1, 2, 3})},
           {"TWO", commonItems::Color(std::array{2, 4, 6})},
       },
       input);

   EXPECT_THAT(countries,
       testing::UnorderedElementsAre(
           testing::Pair(0, Country({.tag = "TAG", .color = commonItems::Color(std::array{1, 2, 3})})),
           testing::Pair(1, Country({.tag = "TWO", .color = commonItems::Color(std::array{2, 4, 6})}))));
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
   const auto countries = ImportCountries({{"TWO", commonItems::Color(std::array{2, 4, 6})}}, input);

   EXPECT_THAT(countries,
       testing::UnorderedElementsAre(
           testing::Pair(1, Country({.tag = "TWO", .color = commonItems::Color(std::array{2, 4, 6})}))));
}

}  // namespace vic3