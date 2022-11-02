#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/country.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountry, TagIsFromSourceCountry)
{
   constexpr mappers::CountryMapper country_mapper;
   const vic3::Country src_country("TAG");
   const vic3::Country src_country_two("TWO");

   const hoi4::Country country(src_country, country_mapper);
   const hoi4::Country country_two(src_country_two, country_mapper);

   EXPECT_EQ(country.GetTag(), "TAG");
   EXPECT_EQ(country_two.GetTag(), "TWO");
}

}  // namespace hoi4