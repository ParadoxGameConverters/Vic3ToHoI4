#include <map>
#include <sstream>
#include <string>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_countries_converter.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper_importer.h"
#include "src/mappers/world/world_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

TEST(Hoi4worldCountriesCountriesConverter, CountriesAreConverted)
{
   std::map<int, Character> characters;
   std::map<std::string, mappers::CultureQueue> culture_queues;
   const mappers::WorldMapper world_mapper =
       mappers::WorldMapperBuilder::CreateNullMapper()
           .AddCountries({{1, "TAG"}, {2, "TWO"}})
           .SetCultureGraphicsMapper(mappers::ImportCultureGraphicsMapper("configurables/culture_graphics.txt"))
           .AddTechs({{{"source_technology_one"}, std::nullopt, {"dest_technology_one", "dest_technology_two"}},
               {{"source_technology_two"}, std::nullopt, {"dest_technology_three", "dest_technology_four"}}})
           .Build();


   const vic3::Country source_country_one({
       .number = 1,
       .color = commonItems::Color{std::array{1, 2, 3}},
       .capital_state = 1,
       .primary_cultures = {"culture_0"},
       .head_of_state_id = 1,
       .character_ids = {1, 3, 4},
   });
   const vic3::Country source_country_two({
       .number = 2,
       .color = commonItems::Color{std::array{2, 4, 6}},
       .capital_state = 2,
       .active_laws = {"law_universal_suffrage", "law_presidential_republic"},
       .primary_cultures = {"culture_1"},
       .head_of_state_id = 2,
       .character_ids = {2},
   });

   vic3::WorldOptions options = {
       .countries =
           {
               {1, source_country_one},
               {2, source_country_two},
           },
       .acquired_technologies =
           {
               {1, {"source_technology_one"}},
               {2, {"source_technology_two"}},
           },
       .culture_definitions =
           {
               {"culture_0", vic3::CultureDefinition({"culture_0"}, {}, {}, {})},
               {"culture_1", vic3::CultureDefinition({"culture_1"}, {}, {}, {})},
           },
       .characters =
           {
               {1, vic3::Character({.id = 1, .roles = {"politician"}})},
               {2, vic3::Character({.id = 2, .roles = {"general"}})},
               {3, vic3::Character({.id = 3, .roles = {"general"}})},
               {4, vic3::Character({.id = 4, .roles = {"agitator"}})},
           },
   };

   const vic3::World v3World = vic3::World(options);

   const auto countries = ConvertCountries(v3World,
       world_mapper,
       commonItems::LocalizationDatabase{{}, {}},
       {
           {1, 10},
           {2, 20},
       },
       {},
       characters,
       culture_queues);

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

   const mappers::GraphicsBlock expected_graphics_block_one{{{"army", {"GFX_general_0"}}},
       "western_european_gfx",
       "western_european_2d"};
   const mappers::GraphicsBlock expected_graphics_block_two{{{"army", {"GFX_general_1"}}}, "asian_gfx", "asian_2d"};

   EXPECT_THAT(countries,
       testing::ElementsAre(testing::Pair("TAG",
                                Country(CountryOptions{
                                    .tag = "TAG",
                                    .color = commonItems::Color{std::array{1, 2, 3}},
                                    .capital_state = 10,
                                    .ideology = "neutrality",
                                    .ideology_support = {{"neutrality", 100}},
                                    .technologies = expected_techs_one,
                                    .legacy_ship_variants = expected_legacy_ship_variants_one,
                                    .ship_variants = expected_ship_variants_one,
                                    .plane_variants = expected_plane_variants_one,
                                    .tank_variants = expected_tank_variants_one,
                                    .graphics_block = expected_graphics_block_one,
                                    .character_ids = {1, 3},
                                    .spy_ids = {4},
                                })),
           testing::Pair("TWO",
               Country(CountryOptions{
                   .tag = "TWO",
                   .color = commonItems::Color{std::array{2, 4, 6}},
                   .capital_state = 20,
                   .ideology = "democratic",
                   .ideology_support = {{"neutrality", 100}},
                   .technologies = expected_techs_two,
                   .legacy_ship_variants = expected_legacy_ship_variants_two,
                   .ship_variants = expected_ship_variants_two,
                   .plane_variants = expected_plane_variants_two,
                   .tank_variants = expected_tank_variants_two,
                   .graphics_block = expected_graphics_block_two,
                   .character_ids = {2},
               }))));
}

}  // namespace hoi4