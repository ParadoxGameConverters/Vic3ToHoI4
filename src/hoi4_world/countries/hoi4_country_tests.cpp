#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountry, TagIsFromSourceCountry)
{
   constexpr mappers::CountryMapper country_mapper;
   const vic3::Country source_country("TAG");
   const vic3::Country source_country_two("TWO");

   const Country country(source_country, country_mapper);
   const Country country_two(source_country_two, country_mapper);

   EXPECT_EQ(country.GetTag(), "TAG");
   EXPECT_EQ(country_two.GetTag(), "TWO");
}

}  // namespace hoi4