#include <map>
#include <sstream>
#include <string>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_countries_converter.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountriesConverter, CountriesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}});

   CountriesConverter countries_converter;
   const auto countries =
       countries_converter.ConvertCountries({{1, source_country_one}, {2, source_country_two}}, country_mapper);

   EXPECT_THAT(countries,
       testing::ElementsAre(
           testing::Pair("TAG", Country({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}})),
           testing::Pair("TWO", Country({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}}))));
}

}  // namespace hoi4