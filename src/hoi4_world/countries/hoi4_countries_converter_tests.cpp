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
#include "src/mappers/world/world_mapper_builder.h"
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
       .states =
           {
               {1, vic3::State({.id = 1})},
               {2, vic3::State({.id = 2})},
               {3, vic3::State({.id = 3})},
               {4, vic3::State({.id = 4})},
           },
       .acquired_technologies =
           {
               {1, {"source_technology_one"}},
               {2, {"source_technology_two"}},
           },
       .buildings = vic3::Buildings({
           {1,
               {
                   vic3::Building(vic3::BuildingType::Port, 1, 0, 1, std::vector<std::string>{"pm_port_3"}),
                   vic3::Building(vic3::BuildingType::NavalBase, 1, 0, 1, std::vector<std::string>{"pm_navy"}),
               }},
           {2, {vic3::Building(vic3::BuildingType::Port, 2, 0, 1, std::vector<std::string>{"pm_port_1"})}},
           {3, {vic3::Building(vic3::BuildingType::Port, 3, 0, 1, std::vector<std::string>{"pm_port_2"})}},
       }),
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

   const States states{
       .states{
           State(10,
               {
                   .owner = "TAG",
                   .provinces = {10},
                   .naval_base_location = 10,
               }),
           State(20,
               {
                   .owner = "TWO",
                   .provinces = {20},
               }),
           State(30,
               {
                   .owner = "TWO",
                   .provinces = {30},
               }),
           State(40,
               {
                   .owner = "TWO",
                   .provinces = {40},
               }),
       },
       .vic3_state_ids_to_hoi4_state_ids{{1, 10}, {2, 20}, {3, 30}, {4, 40}},
       .hoi4_state_ids_to_owner{{10, "TAG"}, {20, "TWO"}, {30, "TWO"}, {40, "TWO"}},
   };
   const vic3::World v3World = vic3::World(options);

   const auto countries = ConvertCountries(v3World,
       world_mapper,
       commonItems::LocalizationDatabase{{}, {}},
       states,
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
       EquipmentVariant("Test Legacy Ship Variant One", "legacy_test_ship", {"dest_technology_one"}, {}, {}),
       EquipmentVariant("Test Legacy Ship Variant Four", "", {}, {"dest_technology_four"}, {}),
   };
   const std::vector<EquipmentVariant> expected_legacy_ship_variants_two = {
       EquipmentVariant("Test Legacy Ship Variant Two", "", {"dest_technology_three"}, {}, {}),
       EquipmentVariant("Test Legacy Ship Variant Three", "", {}, {"dest_technology_two"}, {}),
   };
   const std::vector<EquipmentVariant> expected_ship_variants_one = {
       EquipmentVariant("Test Ship Variant One", "mtg_test_ship", {"dest_technology_one"}, {}, {}),
       EquipmentVariant("Test Ship Variant Four", "", {}, {"dest_technology_four"}, {}),
   };
   const std::vector<EquipmentVariant> expected_ship_variants_two = {
       EquipmentVariant("Test Ship Variant Two", "", {"dest_technology_three"}, {}, {}),
       EquipmentVariant("Test Ship Variant Three", "", {}, {"dest_technology_two"}, {}),
   };
   const std::vector<EquipmentVariant> expected_plane_variants_one = {
       EquipmentVariant("Test Plane Design One", "", {"dest_technology_one"}, {}, {}),
       EquipmentVariant("Test Plane Design Four", "", {}, {"dest_technology_four"}, {}),
   };
   const std::vector<EquipmentVariant> expected_plane_variants_two = {
       EquipmentVariant("Test Plane Design Two", "", {"dest_technology_three"}, {}, {}),
       EquipmentVariant("Test Plane Design Three", "", {}, {"dest_technology_two"}, {}),
   };
   const std::vector<EquipmentVariant> expected_tank_variants_one = {
       EquipmentVariant("Test Tank Design One", "", {"dest_technology_one"}, {}, {}),
       EquipmentVariant("Test Tank Design Four", "", {}, {"dest_technology_four"}, {}),
   };
   const std::vector<EquipmentVariant> expected_tank_variants_two = {
       EquipmentVariant("Test Tank Design Two", "", {"dest_technology_three"}, {}, {}),
       EquipmentVariant("Test Tank Design Three", "", {}, {"dest_technology_two"}, {}),
   };
   const std::vector<TaskForce> expected_task_forces = {
       TaskForce{
           .name = "1. Fleet",
           .ships = {Ship("Test Ship 1", "test_ship", "mtg_test_ship", "legacy_test_ship", "Test Ship Variant One")},
           .location = 10,
       },
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
                                    .starting_research_slots = 3,
                                    .units = {},
                                    .convoys = 100,
                                    .task_forces = expected_task_forces,
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
                   .starting_research_slots = 3,
                   .units = {},
                   .convoys = 11,
                   .task_forces = {},
               }))));
}

}  // namespace hoi4
