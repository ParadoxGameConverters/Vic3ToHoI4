#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountryConverter, TagIsFromSourceCountry)
{
   constexpr mappers::CountryMapper country_mapper;
   const vic3::Country source_country_one("TAG");
   const vic3::Country source_country_two("TWO");

   CountryConverter country_converter;

   const auto country_one = country_converter.ConvertCountry(source_country_one, country_mapper);
   const auto country_two = country_converter.ConvertCountry(source_country_two, country_mapper);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetTag(), "TAG");
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetTag(), "TWO");
}

}  // namespace hoi4