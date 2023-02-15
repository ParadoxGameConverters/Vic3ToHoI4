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
   const mappers::CountryMapper country_mapper({{"TAG", "T00"}, {"TWO", "T01"}});
   const vic3::Country source_country_one({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}});

   CountryConverter country_converter;

   const auto country_one = country_converter.ConvertCountry(source_country_one, {}, country_mapper, {}, {});
   const auto country_two = country_converter.ConvertCountry(source_country_two, {}, country_mapper, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetTag(), "T00");
   EXPECT_EQ(country_one->GetColor(), commonItems::Color(std::array{1, 2, 3}));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetTag(), "T01");
   EXPECT_EQ(country_two->GetColor(), commonItems::Color(std::array{2, 4, 6}));
}


TEST(Hoi4worldCountriesCountryConverter, NoCountryIfNoSourceTag)
{
   const mappers::CountryMapper country_mapper(std::map<std::string, std::string>{
       {"TAG", "T00"},
   });
   const vic3::Country source_country_one({});

   CountryConverter country_converter;

   const auto country_one = country_converter.ConvertCountry(source_country_one, {}, country_mapper, {}, {});

   EXPECT_EQ(country_one, std::nullopt);
}



TEST(Hoi4worldCountriesCountryConverter, NoCountryIfNoTagMapping)
{
   const mappers::CountryMapper country_mapper({});
   const vic3::Country source_country_one({.tag = "TAG"});

   CountryConverter country_converter;

   const auto country_one = country_converter.ConvertCountry(source_country_one, {}, country_mapper, {}, {});

   EXPECT_EQ(country_one, std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, CapitalStatesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .capital_state = 2});
   const vic3::Country source_country_two({.tag = "TWO", .capital_state = 3});

   CountryConverter country_converter;

   std::map<int, int> vic3_state_ids_to_hoi4_state_ids{{2, 4}, {3, 9}};

   const auto country_one =
       country_converter.ConvertCountry(source_country_one, {}, country_mapper, vic3_state_ids_to_hoi4_state_ids, {});
   const auto country_two =
       country_converter.ConvertCountry(source_country_two, {}, country_mapper, vic3_state_ids_to_hoi4_state_ids, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::optional(4));
   ASSERT_TRUE(country_two.has_value());
   EXPECT_EQ(country_two->GetCapitalState(), std::optional(9));
}


TEST(Hoi4worldCountriesCountryConverter, NoCapitalStateIfNoSourceCapitalState)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({
       .tag = "TAG",
   });

   CountryConverter country_converter;

   std::map<int, int> vic3_state_ids_to_hoi4_state_ids{{2, 4}};

   const auto country_one =
       country_converter.ConvertCountry(source_country_one, {}, country_mapper, vic3_state_ids_to_hoi4_state_ids, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, NoCapitalStateIfNoStateMapping)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .capital_state = 2});

   CountryConverter country_converter;

   const auto country_one = country_converter.ConvertCountry(source_country_one, {}, country_mapper, {}, {});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_EQ(country_one->GetCapitalState(), std::nullopt);
}


TEST(Hoi4worldCountriesCountryConverter, TechnologiesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .capital_state = 2});

   CountryConverter country_converter;

   const auto country_one = country_converter.ConvertCountry(source_country_one,
       {"source_tech"},
       country_mapper,
       {},
       {{{"source_tech"}, std::nullopt, {"dest_tech_one", "dest_tech_two"}}});

   ASSERT_TRUE(country_one.has_value());
   EXPECT_THAT(country_one->GetTechnologies().GetTechnologies(),
       testing::UnorderedElementsAre(
           testing::Pair(std::nullopt, std::set<std::string>{"dest_tech_one", "dest_tech_two"})));
}

}  // namespace hoi4