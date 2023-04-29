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
   const mappers::CountryMapper country_mapper({{1, "TAG"}, {2, "TWO"}});
   const vic3::Country source_country_one({
       .number = 1,
       .color = commonItems::Color{std::array{1, 2, 3}},
       .capital_state = 1,
   });
   const vic3::Country source_country_two({
       .number = 2,
       .color = commonItems::Color{std::array{2, 4, 6}},
       .capital_state = 2,
       .active_laws = {"law_landed_voting"},
   });

   const auto countries = ConvertCountries(
       {
           {1, source_country_one},
           {2, source_country_two},
       },
       {
           {1, {"source_technology_one"}},
           {2, {"source_technology_two"}},
       },
       country_mapper,
       {
           {1, 10},
           {2, 20},
       },
       {},
       {
           {{"source_technology_one"}, std::nullopt, {"dest_technology_one", "dest_technology_two"}},
           {{"source_technology_two"}, std::nullopt, {"dest_technology_three", "dest_technology_four"}},
       });

   const Technologies expected_techs_one{
       std::map<std::optional<std::string>, std::set<std::string>>{
           {std::nullopt, std::set<std::string>{"dest_technology_one", "dest_technology_two"}}},
   };
   const Technologies expected_techs_two{
       std::map<std::optional<std::string>, std::set<std::string>>{
           {std::nullopt, std::set<std::string>{"dest_technology_three", "dest_technology_four"}}},
   };
   const std::vector<EquipmentVariant> expected_legacy_ship_variants_one = {
       EquipmentVariant({"dest_technology_one"}, {}, {{"name", "= \"Test Legacy Ship Variant One\""}}),
       EquipmentVariant({}, {"dest_technology_four"}, {{"name", "= \"Test Legacy Ship Variant Four\""}}),
   };
   const std::vector<EquipmentVariant> expected_legacy_ship_variants_two = {
       EquipmentVariant({"dest_technology_three"}, {}, {{"name", "= \"Test Legacy Ship Variant Two\""}}),
       EquipmentVariant({}, {"dest_technology_two"}, {{"name", "= \"Test Legacy Ship Variant Three\""}}),
   };
   const std::vector<EquipmentVariant> expected_ship_variants_one = {
       EquipmentVariant({"dest_technology_one"}, {}, {{"name", "= \"Test Ship Variant One\""}}),
       EquipmentVariant({}, {"dest_technology_four"}, {{"name", "= \"Test Ship Variant Four\""}}),
   };
   const std::vector<EquipmentVariant> expected_ship_variants_two = {
       EquipmentVariant({"dest_technology_three"}, {}, {{"name", "= \"Test Ship Variant Two\""}}),
       EquipmentVariant({}, {"dest_technology_two"}, {{"name", "= \"Test Ship Variant Three\""}}),
   };
   const std::vector<EquipmentVariant> expected_plane_variants_one = {
       EquipmentVariant({"dest_technology_one"}, {}, {{"name", "= \"Test Plane Design One\""}}),
       EquipmentVariant({}, {"dest_technology_four"}, {{"name", "= \"Test Plane Design Four\""}}),
   };
   const std::vector<EquipmentVariant> expected_plane_variants_two = {
       EquipmentVariant({"dest_technology_three"}, {}, {{"name", "= \"Test Plane Design Two\""}}),
       EquipmentVariant({}, {"dest_technology_two"}, {{"name", "= \"Test Plane Design Three\""}}),
   };
   const std::vector<EquipmentVariant> expected_tank_variants_one = {
       EquipmentVariant({"dest_technology_one"}, {}, {{"name", "= \"Test Tank Design One\""}}),
       EquipmentVariant({}, {"dest_technology_four"}, {{"name", "= \"Test Tank Design Four\""}}),
   };
   const std::vector<EquipmentVariant> expected_tank_variants_two = {
       EquipmentVariant({"dest_technology_three"}, {}, {{"name", "= \"Test Tank Design Two\""}}),
       EquipmentVariant({}, {"dest_technology_two"}, {{"name", "= \"Test Tank Design Three\""}}),
   };

   EXPECT_THAT(countries,
       testing::ElementsAre(testing::Pair("TAG",
                                Country(CountryOptions{.tag = "TAG",
                                    .color = commonItems::Color{std::array{1, 2, 3}},
                                    .capital_state = 10,
                                    .ideology = "neutrality",
                                    .technologies = expected_techs_one,
                                    .legacy_ship_variants = expected_legacy_ship_variants_one,
                                    .ship_variants = expected_ship_variants_one,
                                    .plane_variants = expected_plane_variants_one,
                                    .tank_variants = expected_tank_variants_one})),
           testing::Pair("TWO",
               Country(CountryOptions{.tag = "TWO",
                   .color = commonItems::Color{std::array{2, 4, 6}},
                   .capital_state = 20,
                   .ideology = "democratic",
                   .technologies = expected_techs_two,
                   .legacy_ship_variants = expected_legacy_ship_variants_two,
                   .ship_variants = expected_ship_variants_two,
                   .plane_variants = expected_plane_variants_two,
                   .tank_variants = expected_tank_variants_two}))));
}

}  // namespace hoi4