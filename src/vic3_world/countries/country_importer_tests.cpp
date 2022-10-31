#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/country_importer.h"



namespace vic3
{

TEST(Vic3WorldCountriesCountryImporter, DefaultsAreDefaulted)
{
   std::stringstream input;
   const auto country = CountryImporter{}.ImportCountry(input);

   EXPECT_TRUE(country.GetTag().empty());
}


TEST(Vic3WorldCountriesCountryImporter, ItemsCanBeInput)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdefinition=\"TAG\"";
   input << "}";
   const auto country = CountryImporter{}.ImportCountry(input);

   EXPECT_EQ(country.GetTag(), "TAG");
}


TEST(Vic3WorldCountriesCountryImporter, MultipleCountriesCanBeImported)
{
   std::stringstream input_one;
   input_one << "={\n";
   input_one << "\tdefinition=\"TAG\"";
   input_one << "}";
   const auto country_one = CountryImporter{}.ImportCountry(input_one);

   std::stringstream input_two;
   const auto country_two = CountryImporter{}.ImportCountry(input_two);

   EXPECT_EQ(country_one.GetTag(), "TAG");
   EXPECT_TRUE(country_two.GetTag().empty());
}


}  // namespace vic3