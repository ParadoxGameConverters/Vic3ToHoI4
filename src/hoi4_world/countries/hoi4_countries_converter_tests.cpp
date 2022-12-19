#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_countries_converter.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountriesConverter, CountriesAreConverted)
{
   constexpr mappers::CountryMapper country_mapper;
   const vic3::Country source_country_one({.tag = "TAG"});
   const vic3::Country source_country_two({.tag = "TWO"});

   CountriesConverter countries_converter;
   const auto countries =
       countries_converter.ConvertCountries({{1, source_country_one}, {2, source_country_two}}, country_mapper);

   EXPECT_THAT(countries,
       testing::ElementsAre(testing::Pair("TAG", Country("TAG")), testing::Pair("TWO", Country("TWO"))));
}

}  // namespace hoi4