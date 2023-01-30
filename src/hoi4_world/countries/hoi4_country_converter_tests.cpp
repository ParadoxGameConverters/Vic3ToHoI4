#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountryConverter, TagIsFromSourceCountry)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}});

   CountryConverter country_converter;

   const auto country_one = country_converter.ConvertCountry(source_country_one, country_mapper);
   const auto country_two = country_converter.ConvertCountry(source_country_two, country_mapper);

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetTag(), "TAG");
   EXPECT_EQ(country_one->GetColor(), commonItems::Color(std::array{1, 2, 3}));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetTag(), "TWO");
   EXPECT_EQ(country_two->GetColor(), commonItems::Color(std::array{2, 4, 6}));
}

}  // namespace hoi4