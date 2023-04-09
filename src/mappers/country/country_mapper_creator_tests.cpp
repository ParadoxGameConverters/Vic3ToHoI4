#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/country/country_mapper_creator.h"



namespace mappers
{

TEST(MappersCountryCountryMapperCreator, GeneratedMappingsCanBeCreated)
{
   const std::map<int, vic3::Country> source_countries{{1, vic3::Country({.number = 1})}};

   const CountryMapper country_mapper = CreateCountryMappings("", source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag(1), "Z00");
}


TEST(MappersCountryCountryMapperCreator, GeneratedMappingsIncrementTheNumericDigits)
{
   const std::map<int, vic3::Country> source_countries{
       {1, vic3::Country({.number = 1})},
       {2, vic3::Country({.number = 2})},
   };

   const CountryMapper country_mapper = CreateCountryMappings("", source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag(1), "Z00");
   EXPECT_EQ(country_mapper.GetHoiTag(2), "Z01");
}


TEST(MappersCountryCountryMapperCreator, GeneratedMappingsDecrementTheAlphaWhenTheNumericsRollOver)
{
   std::map<int, vic3::Country> source_countries;
   for (int i = 0; i < 102; ++i)
   {
      source_countries.emplace(i, vic3::Country({.number = i}));
   }

   const CountryMapper country_mapper = CreateCountryMappings("", source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag(99), "Z99");
   EXPECT_EQ(country_mapper.GetHoiTag(100), "Y00");
   EXPECT_EQ(country_mapper.GetHoiTag(101), "Y01");
}


TEST(MappersCountryCountryMapperCreator, MappingsCanComeFromRulesFile)
{
   const std::map<int, vic3::Country> source_countries{
       {1, vic3::Country({.number = 1, .tag = "TAG"})},
       {2, vic3::Country({.number = 2, .tag = "TWO"})},
       {3, vic3::Country({.number = 3, .tag = "THR"})},
       {4, vic3::Country({.number = 4, .tag = "TWO"})},
   };

   const CountryMapper country_mapper = CreateCountryMappings(
       "test_files/mappers/MappersCountryCountryMapperCreator/MappingsCanComeFromRulesFile/country_mappings.txt",
       source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag(1), "HOI");  // From rule.
   EXPECT_EQ(country_mapper.GetHoiTag(2), "2ND");  // From rule.
   EXPECT_EQ(country_mapper.GetHoiTag(3), "Z00");  // From rule.
   EXPECT_EQ(country_mapper.GetHoiTag(4), "Z01");  // Generated. Previous rule generated Z00, so advanced to Z01.
}

}  // namespace mappers