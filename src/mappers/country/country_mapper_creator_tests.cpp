#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/country/country_mapper_creator.h"



namespace mappers
{

TEST(MappersCountryCountryMapperCreator, GeneratedMappingsCanBeCreated)
{
   std::map<int, vic3::Country> source_countries{{1, vic3::Country({.tag = "000"})}};

   const CountryMapper country_mapper = CreateCountryMappings(source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag("000"), "Z00");
}


TEST(MappersCountryCountryMapperCreator, GeneratedMappingsIncrementTheNumericDigits)
{
   std::map<int, vic3::Country> source_countries{{1, vic3::Country({.tag = "000"})},
       {2, vic3::Country({.tag = "001"})}};

   const CountryMapper country_mapper = CreateCountryMappings(source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag("000"), "Z00");
   EXPECT_EQ(country_mapper.GetHoiTag("001"), "Z01");
}

TEST(MappersCountryCountryMapperCreator, GeneratedMappingsDecrementTheAlphaWhenTheNumericsRollOver)
{
   std::map<int, vic3::Country> source_countries;
   for (int i = 0; i < 102; ++i)
   {
      source_countries.emplace(i, vic3::Country({.tag = fmt::format("{:0>3}", i)}));
   }

   const CountryMapper country_mapper = CreateCountryMappings(source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag("099"), "Z99");
   EXPECT_EQ(country_mapper.GetHoiTag("100"), "Y00");
   EXPECT_EQ(country_mapper.GetHoiTag("101"), "Y01");
}

}  // namespace mappers