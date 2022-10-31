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


}  // namespace vic3