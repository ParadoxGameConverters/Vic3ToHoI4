#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <sstream>

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

TEST(MappersCountryCountryMapperCreator, SourceTagAsPrimaryFallback)
{
   const std::map<int, vic3::Country> source_countries{
       {1, vic3::Country({.number = 1, .tag = "TAG"})},
       {2, vic3::Country({.number = 2, .tag = "TWO"})},
       {3, vic3::Country({.number = 3, .tag = "TWO"})},
       {4, vic3::Country({.number = 4, .tag = "D01"})},
   };

   const CountryMapper country_mapper = CreateCountryMappings("", source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag(1), "TAG");
   EXPECT_EQ(country_mapper.GetHoiTag(2), "TWO");
   EXPECT_EQ(country_mapper.GetHoiTag(3), "Z00");  // "TWO" was already taken, so fall back to the Znn system
   EXPECT_EQ(country_mapper.GetHoiTag(4), "Z01");  // "D01" is a dynamic tag, fall back to the Znn system
}


TEST(MappersCountryCountryMapperCreator, DisallowedSourceTagsAreNotPrimaryFallback)
{
   const std::map<int, vic3::Country> source_countries{
       {1, vic3::Country({.number = 1, .tag = "CON"})},
       {2, vic3::Country({.number = 2, .tag = "PRN"})},
       {3, vic3::Country({.number = 3, .tag = "AUX"})},
       {4, vic3::Country({.number = 4, .tag = "NUL"})},
   };

   const CountryMapper country_mapper = CreateCountryMappings("", source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag(1), "Z00");
   EXPECT_EQ(country_mapper.GetHoiTag(2), "Z01");
   EXPECT_EQ(country_mapper.GetHoiTag(3), "Z02");
   EXPECT_EQ(country_mapper.GetHoiTag(4), "Z03");
}


TEST(MappersCountryCountryMapperCreator, CivilWarCountryIsSecondary)
{
   const std::map<int, vic3::Country> source_countries{
       {1, vic3::Country({.number = 1, .tag = "TAG", .is_civil_war = true})},
       {2, vic3::Country({.number = 2, .tag = "TAG"})},
   };

   const CountryMapper country_mapper = CreateCountryMappings("", source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag(1), "Z00");
   EXPECT_EQ(country_mapper.GetHoiTag(2), "TAG");
}

TEST(MappersCountryCountryMapperCreator, MappingsCanComeFromRulesFile)
{
   const std::map<int, vic3::Country> source_countries{
       {1, vic3::Country({.number = 1, .tag = "TAG"})},
       {2, vic3::Country({.number = 2, .tag = "TWO"})},
       {3, vic3::Country({.number = 3, .tag = "THR"})},
       {4, vic3::Country({.number = 4, .tag = "Z00"})},
       {5, vic3::Country({.number = 5, .tag = "NO0"})},
       {6, vic3::Country({.number = 6, .tag = "NO1"})},
       {7, vic3::Country({.number = 7, .tag = "NO2"})},
       {8, vic3::Country({.number = 8, .tag = "NO3"})},
   };

   const CountryMapper country_mapper = CreateCountryMappings(
       "test_files/mappers/MappersCountryCountryMapperCreator/MappingsCanComeFromRulesFile/country_mappings.txt",
       source_countries);
   EXPECT_EQ(country_mapper.GetHoiTag(1), "HOI");  // From rule.
   EXPECT_EQ(country_mapper.GetHoiTag(2), "2ND");  // From rule.
   EXPECT_EQ(country_mapper.GetHoiTag(3), "Z00");  // From rule.
   EXPECT_EQ(country_mapper.GetHoiTag(4), "Z01");  // Generated. Previous rule generated Z00, so advanced to Z01.
   EXPECT_EQ(country_mapper.GetHoiTag(5), "NO0");  // Vic3 tag. Rule had a disallowed tag.
   EXPECT_EQ(country_mapper.GetHoiTag(6), "NO1");  // Vic3 tag. Rule had a disallowed tag.
   EXPECT_EQ(country_mapper.GetHoiTag(7), "NO2");  // Vic3 tag. Rule had a disallowed tag.
   EXPECT_EQ(country_mapper.GetHoiTag(8), "NO3");  // Vic3 tag. Rule had a disallowed tag.
}

}  // namespace mappers