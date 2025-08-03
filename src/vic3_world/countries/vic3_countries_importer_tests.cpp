#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

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
   input << "3019898882={\n";
   input << "\tdefinition=\"BIG\"\n";
   input << "\t}\n";
   input << "}\n";
   const auto countries = ImportCountries(
       {
           {"TAG", commonItems::Color(std::array{1, 2, 3})},
           {"TWO", commonItems::Color(std::array{2, 4, 6})},
           {"BIG", commonItems::Color(std::array{3, 6, 9})},
       },
       input);

   EXPECT_THAT(countries,
       testing::UnorderedElementsAre(
           testing::Pair(0, Country({.number = 0, .tag = "TAG", .color = commonItems::Color(std::array{1, 2, 3})})),
           testing::Pair(1, Country({.number = 1, .tag = "TWO", .color = commonItems::Color(std::array{2, 4, 6})})),
           testing::Pair(-1275068414,
               Country({.number = -1275068414, .tag = "BIG", .color = commonItems::Color(std::array{3, 6, 9})}))));
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
           testing::Pair(1, Country({.number = 1, .tag = "TWO", .color = commonItems::Color(std::array{2, 4, 6})}))));
}

}  // namespace vic3