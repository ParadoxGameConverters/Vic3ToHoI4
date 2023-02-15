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
   const vic3::Country source_country_one(
       {.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}, .capital_state = 1});
   const vic3::Country source_country_two(
       {.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}, .capital_state = 2});

   CountriesConverter countries_converter;
   const auto countries = countries_converter.ConvertCountries({{1, source_country_one}, {2, source_country_two}},
       {
           {1, {"source_technology_one"}},
           {2, {"source_technology_two"}},
       },
       country_mapper,
       {{1, 10}, {2, 20}},
       {
           {{"source_technology_one"}, std::nullopt, {"dest_technology_one", "dest_technology_two"}},
           {{"source_technology_two"}, std::nullopt, {"dest_technology_three", "dest_technology_four"}},
       });

   const Technologies expected_techs_one{std::map<std::optional<std::string>, std::set<std::string>>{
       {std::nullopt, std::set<std::string>{"dest_technology_one", "dest_technology_two"}}}};
   const Technologies expected_techs_two{std::map<std::optional<std::string>, std::set<std::string>>{
       {std::nullopt, std::set<std::string>{"dest_technology_three", "dest_technology_four"}}}};

   EXPECT_THAT(countries,
       testing::ElementsAre(testing::Pair("TAG",
                                Country(CountryOptions{.tag = "TAG",
                                    .color = commonItems::Color{std::array{1, 2, 3}},
                                    .capital_state = 10,
                                    .technologies = expected_techs_one})),
           testing::Pair("TWO",
               Country(CountryOptions{.tag = "TWO",
                   .color = commonItems::Color{std::array{2, 4, 6}},
                   .capital_state = 20,
                   .technologies = expected_techs_two}))));
}

}  // namespace hoi4