#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <sstream>

#include "hoi4_world_framework_builder.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/hoi4_world/world/hoi4_world_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/mappers/world/world_mapper_builder.h"
#include "src/maps/province_definitions.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

TEST(Hoi4worldWorldHoi4worldconverter, EmptyWorldIsEmpty)
{
   const vic3::World source_world(vic3::WorldOptions{});
   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper().Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateNullWorldFramework().Build();
       }));

   EXPECT_TRUE(world.GetCountries().empty());
   EXPECT_TRUE(world.GetStates().states.empty());
   EXPECT_TRUE(world.GetStates().province_to_state_id_map.empty());
   EXPECT_TRUE(world.GetCharacters().empty());
}


TEST(Hoi4worldWorldHoi4worldconverter, CountriesAreConverted)
{
   const vic3::Country source_country_one({
       .number = 1,
       .tag = "TAG",
       .color = commonItems::Color{std::array{1, 2, 3}},
       .head_of_state_id = 1,
       .character_ids = {1},
   });
   const vic3::Country source_country_two({
       .number = 3,
       .tag = "TWO",
       .color = commonItems::Color{std::array{2, 4, 6}},
       .head_of_state_id = 3,
       .character_ids = {2, 3},
   });

   vic3::Character character_one({.id = 1});  // Compiler was being weird about it directly in map
   vic3::Character character_two({.id = 2, .roles = {"agitator"}});
   vic3::Character character_three({.id = 3});
   const vic3::World source_world({
       .countries = {{1, source_country_one}, {3, source_country_two}},
       .states = {},
       .acquired_technologies =
           {
               {1, {"source_tech"}},
               {3, {"source_tech_two", "source_tech_three"}},
           },
       .characters =
           {
               {1, character_one},
               {2, character_two},
               {3, character_three},
           },
   });

   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({{1, "TAG"}, {3, "TWO"}})
                                                 .DefaultTechMapper()
                                                 .Build();
   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateNullWorldFramework().Build();
       }));

   const Technologies expected_techs_one{std::map<std::optional<std::string>, std::set<std::string>>{
       {std::nullopt, std::set<std::string>{"dest_tech_one", "dest_tech_two"}}}};
   const Technologies expected_techs_two{std::map<std::optional<std::string>, std::set<std::string>>{
       {R"(not = { has_dlc = "Test DLC" })", std::set<std::string>{"dest_tech_three"}}}};
   const std::vector<EquipmentVariant> expected_legacy_ship_variants_one = {
       EquipmentVariant(EquipmentVariantName{"Test Legacy Ship Variant Three"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_two"},
           {}),
       EquipmentVariant(EquipmentVariantName{"Test Legacy Ship Variant Four"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_four"},
           {}),
   };
   const std::vector<EquipmentVariant> expected_legacy_ship_variants_two = {
       EquipmentVariant(EquipmentVariantName{"Test Legacy Ship Variant Three"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_two"},
           {}),
       EquipmentVariant(EquipmentVariantName{"Test Legacy Ship Variant Four"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_four"},
           {}),
   };
   const std::vector<EquipmentVariant> expected_ship_variants_one = {
       EquipmentVariant(EquipmentVariantName{"Test Ship Variant Three"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_two"},
           {}),
       EquipmentVariant(EquipmentVariantName{"Test Ship Variant Four"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_four"},
           {}),
   };
   const std::vector<EquipmentVariant> expected_ship_variants_two = {
       EquipmentVariant(EquipmentVariantName{"Test Ship Variant Three"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_two"},
           {}),
       EquipmentVariant(EquipmentVariantName{"Test Ship Variant Four"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_four"},
           {}),
   };
   const std::vector<EquipmentVariant> expected_plane_variants_one = {
       EquipmentVariant(EquipmentVariantName{"Test Plane Design Three"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_two"},
           {}),
       EquipmentVariant(EquipmentVariantName{"Test Plane Design Four"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_four"},
           {}),
   };
   const std::vector<EquipmentVariant> expected_plane_variants_two = {
       EquipmentVariant(EquipmentVariantName{"Test Plane Design Three"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_two"},
           {}),
       EquipmentVariant(EquipmentVariantName{"Test Plane Design Four"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_four"},
           {}),
   };
   const std::vector<EquipmentVariant> expected_tank_variants_one = {
       EquipmentVariant(EquipmentVariantName{"Test Tank Design Three"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_two"},
           {}),
       EquipmentVariant(EquipmentVariantName{"Test Tank Design Four"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_four"},
           {}),
   };
   const std::vector<EquipmentVariant> expected_tank_variants_two = {
       EquipmentVariant(EquipmentVariantName{"Test Tank Design Three"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_two"},
           {}),
       EquipmentVariant(EquipmentVariantName{"Test Tank Design Four"},
           EquipmentVariantType{""},
           {},
           {"dest_technology_four"},
           {}),
   };

   EXPECT_THAT(world.GetCountries(),
       testing::ElementsAre(testing::Pair("TAG",
                                Country(CountryOptions{
                                    .source_country_number = 1,
                                    .tag = "TAG",
                                    .color = commonItems::Color{std::array{1, 2, 3}},
                                    .ideology_support = {{"neutrality", 100}},
                                    .technologies = expected_techs_one,
                                    .legacy_ship_variants = expected_legacy_ship_variants_one,
                                    .ship_variants = expected_ship_variants_one,
                                    .plane_variants = expected_plane_variants_one,
                                    .tank_variants = expected_tank_variants_one,
                                    .character_ids = {1},
                                    .starting_research_slots = 3,
                                })),
           testing::Pair("TWO",
               Country(CountryOptions{
                   .source_country_number = 3,
                   .tag = "TWO",
                   .color = commonItems::Color{std::array{2, 4, 6}},
                   .ideology_support = {{"neutrality", 100}},
                   .technologies = expected_techs_two,
                   .legacy_ship_variants = expected_legacy_ship_variants_two,
                   .ship_variants = expected_ship_variants_two,
                   .plane_variants = expected_plane_variants_two,
                   .tank_variants = expected_tank_variants_two,
                   .character_ids = {3},
                   .spy_ids = {2},
                   .starting_research_slots = 3,
               }))));
}


TEST(Hoi4worldWorldHoi4worldconverter, StatesAreConverted)
{
   const std::map<std::string, vic3::StateRegion> state_regions({{"STATE_ONE",
       vic3::StateRegion(
           {
               {"x000005", "city"},
               {"x000004", "port"},
               {"x000003", "farm"},
               {"x000002", "mine"},
               {"x000001", "wood"},
           },
           {})}});
   const auto province_definitions = vic3::ProvinceDefinitions({
       "x000001",
       "x000002",
       "x000003",
       "x000004",
       "x000005",
       "x000006",
   });
   const vic3::Buildings vic3_buildings({
       {1, std::vector{vic3::Building("", 1, 875'000)}},
       {2, std::vector{vic3::Building("", 2, 700'000)}},
   });

   const vic3::World source_world(vic3::WorldOptions{
       .states =
           {
               {1, vic3::State({.owner_number = 1, .owner_tag = "TAG", .provinces = {1, 2, 3}})},
               {2, vic3::State({.owner_number = 2, .owner_tag = "TWO", .provinces = {4, 5, 6}})},
           },
       .state_regions = state_regions,
       .province_definitions = province_definitions,
       .buildings = vic3_buildings,
   });

   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({{1, "TAG"}, {2, "TWO"}})
                                                 .AddProvinces({
                                                     {"x000001", {10}},
                                                     {"x000002", {20}},
                                                     {"x000003", {30}},
                                                     {"x000004", {40}},
                                                     {"x000005", {50}},
                                                     {"x000006", {60}},
                                                 })
                                                 .Build();
   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
              commonItems::ModFilesystem("test_files/hoi4_world", {}))
              .Build();
       }));

   EXPECT_THAT(world.GetStates().states,
       testing::ElementsAre(State(1,
                                {.owner = "TAG",
                                    .provinces = {10, 20, 30},
                                    .category = "large_city",
                                    .continent = "europe",
                                    .victory_points = {{30, 3}},
                                    .civilian_factories = 3,
                                    .military_factories = 2,
                                    .air_base_level = 1}),
           State(2,
               {.owner = "TWO",
                   .provinces = {40, 50, 60},
                   .category = "city",
                   .continent = "australia",
                   .victory_points = {{50, 2}},
                   .civilian_factories = 2,
                   .military_factories = 2,
                   .air_base_level = 1})));
   EXPECT_THAT(world.GetStates().province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2),
           testing::Pair(60, 2)));
}


TEST(Hoi4worldWorldHoi4worldconverter, CapitalsGetExtraVictoryPointValue)
{
   std::map<int, vic3::Country> countries;
   std::map<std::string, vic3::StateRegion> state_regions;
   std::vector<std::string> province_definitions_initializer;
   std::map<int, std::vector<vic3::Building>> buildings_initializer;
   std::map<int, int> scored_countries;
   std::map<int, vic3::State> vic3_states;

   mappers::WorldMapperBuilder mapper_builder = mappers::WorldMapperBuilder::CreateNullMapper();
   for (int i = 1; i <= 80; ++i)
   {
      mapper_builder.AddCountries({{i, fmt::format("X{:0>2}", i)}});
      countries.emplace(i,
          vic3::Country({
              .number = i,
              .tag = fmt::format("X{:0>2}", i),
              .capital_state = i,
          }));
      state_regions.emplace(fmt::format("STATE_{:0>2}", i),
          vic3::StateRegion(
              {
                  {fmt::format("x0000{:0>2}", i), "city"},
              },
              {}));
      province_definitions_initializer.emplace_back(fmt::format("x0000{:0>2}", i));
      buildings_initializer.emplace(i, std::vector{vic3::Building("", i, 1000.0f - static_cast<float>(i))});
      scored_countries.emplace(i, i);
      vic3_states.emplace(i,
          vic3::State({.owner_number = i, .owner_tag = fmt::format("x0000{:0>2}", i), .provinces = {i}}));
      mapper_builder.AddProvinces({{fmt::format("x0000{:0>2}", i), i}});
   }
   const vic3::ProvinceDefinitions province_definitions(province_definitions_initializer);
   const vic3::Buildings buildings;
   const vic3::CountryRankings country_rankings({1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, scored_countries);

   const vic3::World source_world(vic3::WorldOptions{
       .countries = countries,
       .states = vic3_states,
       .state_regions = state_regions,
       .province_definitions = province_definitions,
       .buildings = buildings,
       .country_rankings = country_rankings,
   });

   const World world =
       ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/CapitalsGetExtraVictoryPointValue", {}),
           source_world,
           mapper_builder.Build(),
           std::async<>(std::launch::async, []() {
              return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
                  commonItems::ModFilesystem("test_files/hoi4_world/CapitalsGetExtraVictoryPointValue", {}))
                  .Build();
           }));

   // HoI4 states are in an arbitrary order compared to Vic3 states, so store by province number for the actual
   // checks
   std::map<int, State> states;
   for (const auto& state: world.GetStates().states)
   {
      ASSERT_FALSE(state.GetProvinces().empty());
      const int province = *state.GetProvinces().begin();
      states.emplace(province, state);
   }

   // five great powers, first two have a VP of 50
   EXPECT_THAT(states.at(1).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(1, 50)));
   EXPECT_THAT(states.at(2).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(2, 50)));

   // remaining GPs have VPs worth 40
   EXPECT_THAT(states.at(3).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(3, 40)));
   EXPECT_THAT(states.at(4).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(4, 40)));
   EXPECT_THAT(states.at(5).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(5, 40)));

   // five major powers, first two have a VP of 30
   EXPECT_THAT(states.at(6).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(6, 30)));
   EXPECT_THAT(states.at(7).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(7, 30)));

   // remaining majors have VPs worth 25
   EXPECT_THAT(states.at(8).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(8, 25)));
   EXPECT_THAT(states.at(9).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(9, 25)));
   EXPECT_THAT(states.at(10).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(10, 25)));

   // the first 30 of the remaining countries have VPs worth 20
   for (int i = 11; i < 41; ++i)
   {
      EXPECT_THAT(states.at(i).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(i, 20)));
   }

   // the next 30 of the remaining countries have VPs worth 15
   for (int i = 41; i < 71; ++i)
   {
      EXPECT_THAT(states.at(i).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(i, 15)));
   }

   // the remaining countries have VPs worth 10
   for (int i = 71; i < 81; ++i)
   {
      EXPECT_THAT(states.at(i).GetVictoryPoints(), testing::UnorderedElementsAre(testing::Pair(i, 10)));
   }
}


TEST(Hoi4worldWorldHoi4worldconverter, CapitalsGetExtraAirBaseLevel)
{
   const vic3::Country source_country_one({
       .number = 1,
       .tag = "TAG",
       .color = commonItems::Color{std::array{1, 2, 3}},
       .head_of_state_id = 1,
       .character_ids = {1},
   });

   const std::map<std::string, vic3::StateRegion> state_regions({{"STATE_ONE",
       vic3::StateRegion(
           {
               {"x000005", "city"},
               {"x000004", "port"},
               {"x000003", "farm"},
               {"x000002", "mine"},
               {"x000001", "wood"},
           },
           {})}});
   const auto province_definitions = vic3::ProvinceDefinitions({
       "x000001",
       "x000002",
       "x000003",
       "x000004",
       "x000005",
       "x000006",
   });
   const vic3::Buildings vic3_buildings({
       {1, std::vector{vic3::Building("", 1, 0)}},
       {2, std::vector{vic3::Building("", 2, 0)}},
   });

   const vic3::World source_world(vic3::WorldOptions{
       .countries = {{1, source_country_one}},
       .states =
           {
               {1, vic3::State({.owner_number = 1, .owner_tag = "TAG", .provinces = {1, 2, 3}})},
               {2, vic3::State({.owner_number = 2, .owner_tag = "TWO", .provinces = {4, 5, 6}})},
           },
       .state_regions = state_regions,
       .province_definitions = province_definitions,
       .buildings = vic3_buildings,
   });

   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({{1, "TAG"}})
                                                 .AddProvinces({
                                                     {"x000001", {10}},
                                                     {"x000002", {20}},
                                                     {"x000003", {30}},
                                                     {"x000004", {40}},
                                                     {"x000005", {50}},
                                                     {"x000006", {60}},
                                                 })
                                                 .Build();
   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
              commonItems::ModFilesystem("test_files/hoi4_world", {}))
              .Build();
       }));

   EXPECT_THAT(world.GetStates().states,
       testing::ElementsAre(State(1,
                                {.owner = "TAG",
                                    .provinces = {10, 20, 30},
                                    .category = "rural",
                                    .is_capital = true,
                                    .continent = "europe",
                                    .victory_points = {{30, 1}},
                                    .air_base_level = 5}),
           State(2,
               {.provinces = {40, 50, 60},
                   .category = "rural",
                   .continent = "australia",
                   .victory_points = {{50, 1}},
                   .air_base_level = 0})));
}


TEST(Hoi4worldWorldHoi4worldconverter, CapitalsAreMarkedAsCapitals)
{
   const vic3::Country source_country_one(
       {.number = 1, .tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}, .capital_state = 1});

   const std::map<std::string, vic3::StateRegion> state_regions({{"STATE_ONE",
       vic3::StateRegion(
           {
               {"x000005", "city"},
               {"x000004", "port"},
               {"x000003", "farm"},
               {"x000002", "mine"},
               {"x000001", "wood"},
           },
           {})}});
   const auto province_definitions = vic3::ProvinceDefinitions({
       "x000001",
       "x000002",
       "x000003",
       "x000004",
       "x000005",
       "x000006",
   });
   const vic3::Buildings vic3_buildings({
       {1, std::vector{vic3::Building("", 1, 0)}},
       {2, std::vector{vic3::Building("", 2, 0)}},
   });

   const vic3::World source_world(vic3::WorldOptions{
       .countries = {{1, source_country_one}},
       .states =
           {
               {1, vic3::State({.owner_number = 1, .owner_tag = "TAG", .provinces = {1, 2, 3}})},
               {2, vic3::State({.owner_number = 2, .owner_tag = "TWO", .provinces = {4, 5, 6}})},
           },
       .state_regions = state_regions,
       .province_definitions = province_definitions,
       .buildings = vic3_buildings,
   });

   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({{1, "TAG"}})
                                                 .AddProvinces({
                                                     {"x000001", {10}},
                                                     {"x000002", {20}},
                                                     {"x000003", {30}},
                                                     {"x000004", {40}},
                                                     {"x000005", {50}},
                                                     {"x000006", {60}},
                                                 })
                                                 .Build();
   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
              commonItems::ModFilesystem("test_files/hoi4_world", {}))
              .Build();
       }));

   EXPECT_TRUE(world.GetStates().states[0].IsCapital());
   EXPECT_FALSE(world.GetStates().states[1].IsCapital());
}


TEST(Hoi4worldWorldHoi4worldconverter, StrategicRegionsAreCreated)
{
   const auto province_definitions = vic3::ProvinceDefinitions({
       "x000001",
       "x000002",
       "x000003",
       "x000004",
       "x000005",
       "x000006",
   });

   const vic3::World source_world({
       .states =
           {
               {1, vic3::State({.provinces = {1, 2, 3}})},
               {2, vic3::State({.provinces = {4, 5, 6}})},
           },
       .province_definitions = province_definitions,
   });
   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddProvinces({
                                                     {"x000001", {10}},
                                                     {"x000002", {20}},
                                                     {"x000003", {30}},
                                                     {"x000004", {40}},
                                                     {"x000005", {50}},
                                                     {"x000006", {60}},
                                                 })
                                                 .Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/StrategicRegionsAreCreated", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
              commonItems::ModFilesystem("test_files/hoi4_world/StrategicRegionsAreCreated", {}))
              .Build();
       }));

   const auto strategic_regions = world.GetStrategicRegions().GetStrategicRegions();
   ASSERT_TRUE(strategic_regions.contains(10));
   ASSERT_TRUE(strategic_regions.contains(50));

   const auto region_10 = strategic_regions.find(10)->second;
   EXPECT_EQ(region_10.GetFilename(), "strategic_region_10.txt");
   EXPECT_EQ(region_10.GetID(), 10);
   EXPECT_EQ(region_10.GetName(), "STRATEGICREGION_10");
   EXPECT_THAT(region_10.GetOldProvinces(), testing::UnorderedElementsAre(10, 20, 30, 40));
   EXPECT_THAT(region_10.GetNewProvinces(), testing::UnorderedElementsAre(10, 20, 30));
   EXPECT_TRUE(region_10.HasStaticModifiers());
   EXPECT_THAT(region_10.GetStaticModifiers(),
       testing::UnorderedElementsAre(testing::Pair("test_modifier", "always"),
           testing::Pair("test_modifier_two", "always")));
   ASSERT_TRUE(region_10.GetNavalTerrain().has_value());
   EXPECT_EQ(region_10.GetNavalTerrain().value_or(""), "test_naval_terrain");
   EXPECT_EQ(region_10.GetWeather(),
       "= {\n"
       "\t\tperiod={\n"
       "\t\t\tbetween={ 0.0 30.0 }\n"
       "\t\t\ttemperature={ -6.0 12.0 }\n"
       "\t\t\tno_phenomenon=0.500\n"
       "\t\t\train_light=1.000\n"
       "\t\t\train_heavy=0.150\n"
       "\t\t\tsnow=0.200\n"
       "\t\t\tblizzard=0.000\n"
       "\t\t\tarctic_water=0.000\n"
       "\t\t\tmud=0.300\n"
       "\t\t\tsandstorm=0.000\n"
       "\t\t\tmin_snow_level=0.000\n"
       "\t\t}\n"
       "\t\tperiod={\n"
       "\t\t\tbetween={ 0.1 27.1 }\n"
       "\t\t\ttemperature={ -7.0 12.0 }\n"
       "\t\t\tno_phenomenon=0.500\n"
       "\t\t\train_light=1.000\n"
       "\t\t\train_heavy=0.150\n"
       "\t\t\tsnow=0.200\n"
       "\t\t\tblizzard=0.050\n"
       "\t\t\tarctic_water=0.000\n"
       "\t\t\tmud=0.300\n"
       "\t\t\tsandstorm=0.000\n"
       "\t\t\tmin_snow_level=0.000\n"
       "\t\t}\n"
       "\t}");

   const auto region_50 = strategic_regions.find(50)->second;
   EXPECT_EQ(region_50.GetFilename(), "strategic_region_50.txt");
   EXPECT_EQ(region_50.GetID(), 50);
   EXPECT_EQ(region_50.GetName(), "STRATEGICREGION_50");
   EXPECT_THAT(region_50.GetOldProvinces(), testing::UnorderedElementsAre(50, 60));
   EXPECT_THAT(region_50.GetNewProvinces(), testing::UnorderedElementsAre(40, 50, 60));
   EXPECT_FALSE(region_50.HasStaticModifiers());
   EXPECT_TRUE(region_50.GetStaticModifiers().empty());
   ASSERT_FALSE(region_50.GetNavalTerrain().has_value());
   EXPECT_TRUE(region_50.GetWeather().empty());

   EXPECT_THAT(world.GetStrategicRegions().GetProvinceToStrategicRegionMap(),
       testing::UnorderedElementsAre(testing::Pair(10, 10),
           testing::Pair(20, 10),
           testing::Pair(30, 10),
           testing::Pair(40, 50),
           testing::Pair(50, 50),
           testing::Pair(60, 50)));
   EXPECT_THAT(world.GetStrategicRegions().GetStrategicRegions().at(10).GetNewProvinces(),
       testing::UnorderedElementsAre(10, 20, 30));
   EXPECT_THAT(world.GetStrategicRegions().GetStrategicRegions().at(50).GetNewProvinces(),
       testing::UnorderedElementsAre(40, 50, 60));
}


TEST(Hoi4worldWorldHoi4worldconverter, BuildingsAreCreated)
{
   const auto province_definitions = vic3::ProvinceDefinitions({
       "x000001",
       "x000002",
       "x000003",
       "x000004",
       "x000005",
       "x000006",
       "x000007",
       "x000008",
   });

   const vic3::World source_world({
       .states =
           {
               {1, vic3::State({.provinces = {1, 2, 3, 4, 5}})},
               {2, vic3::State({.provinces = {6, 7}})},
               {3, vic3::State({.provinces = {8}})},
           },
       .province_definitions = province_definitions,
   });

   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddProvinces({
                                                     {{"x000001"}, 1},
                                                     {{"x000002"}, 2},
                                                     {{"x000003"}, 3},
                                                     {{"x000004"}, 4},
                                                     {{"x000005"}, 5},
                                                     {{"x000006"}, 6},
                                                     {{"x000007"}, 7},
                                                     {{"x000008"}, 8},
                                                 })
                                                 .Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/BuildingsAreCreated", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
              commonItems::ModFilesystem("test_files/hoi4_world/BuildingsAreCreated", {}))
              .Build();
       }));

   EXPECT_FALSE(world.GetBuildings().GetBuildings().empty());
}


TEST(Hoi4worldWorldHoi4worldconverter, RailwaysAreCreated)
{
   const vic3::StateRegion vic3_state_region({{"x000001", "city"}, {"x000002", "port"}, {"x000003", "mine"}}, {});

   const auto province_definitions = vic3::ProvinceDefinitions({
       "x000001",
       "x000002",
       "x000003",
       "x000004",
       "x000005",
       "x000006",
       "x000007",
       "x000008",
       "x000009",
       "x000010",
       "x000011",
       "x000012",
   });

   const std::map<int, std::vector<vic3::Building>> buildings = {
       {1,
           {vic3::Building(vic3::kBuildingTypeNavalBase,
               1,
               vic3::GoodsSalesValue{0.0F},
               vic3::StaffingLevel{5.0F},
               {})}},
   };

   const vic3::World source_world({.states =
                                       {
                                           {1,
                                               vic3::State({
                                                   .id = 1,
                                                   .owner_number = 1,
                                                   .owner_tag = "TAG",
                                                   .provinces = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
                                               })},
                                       },
       .state_regions =
           {
               {"state", vic3_state_region},
           },
       .province_definitions = province_definitions,
       .buildings = vic3::Buildings(buildings)});

   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddProvinces({
                                                     {"x000001", {1}},
                                                     {"x000002", {2}},
                                                     {"x000003", {3}},
                                                     {"x000004", {4}},
                                                     {"x000005", {5}},
                                                     {"x000006", {6}},
                                                     {"x000007", {7}},
                                                     {"x000008", {8}},
                                                     {"x000009", {9}},
                                                     {"x000010", {10}},
                                                     {"x000011", {11}},
                                                     {"x000012", {12}},
                                                 })
                                                 .Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/RailwaysAreCreated", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
              commonItems::ModFilesystem("test_files/hoi4_world/RailwaysAreCreated", {}))
              .Build();
       }));

   EXPECT_FALSE(world.GetRailways().railways.empty());
}


TEST(Hoi4worldWorldHoi4worldconverter, GreatPowersAreConverted)
{
   const vic3::World source_world({
       .countries =
           {
               {1, vic3::Country({})},
               {3, vic3::Country({})},
               {5, vic3::Country({})},
           },
       .country_rankings = {{1, 3, 5}, {}, {}},
   });
   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({
                                                     {1, "ONE"},
                                                     {3, "THR"},
                                                     {5, "FIV"},
                                                 })
                                                 .Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/RailwaysAreCreated", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateNullWorldFramework().Build();
       }));

   EXPECT_THAT(world.GetGreatPowers(), testing::UnorderedElementsAre("ONE", "THR", "FIV"));
}


TEST(Hoi4worldWorldHoi4worldconverter, MajorPowersAreConverted)
{
   const vic3::World source_world({
       .countries =
           {
               {1, vic3::Country({})},
               {3, vic3::Country({})},
               {5, vic3::Country({})},
           },
       .country_rankings = {{}, {1, 3, 5}, {}},
   });

   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({
                                                     {1, "ONE"},
                                                     {3, "THR"},
                                                     {5, "FIV"},
                                                 })
                                                 .Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/RailwaysAreCreated", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
              commonItems::ModFilesystem("test_files/hoi4_world/RailwaysAreCreated", {}))
              .Build();
       }));

   EXPECT_THAT(world.GetMajorPowers(), testing::UnorderedElementsAre("ONE", "THR", "FIV"));
}


TEST(Hoi4worldWorldHoi4worldconverter, LocalizationsAreConverted)
{
   const vic3::Country source_country_one({
       .number = 1,
       .tag = "TAG",
       .color = commonItems::Color{std::array{1, 2, 3}},
   });
   const vic3::Country source_country_two({
       .number = 3,
       .tag = "THR",
       .color = commonItems::Color{std::array{2, 4, 6}},
   });
   const std::map<int, vic3::Country> source_countries = {{1, source_country_one}, {3, source_country_two}};

   const std::map<int, vic3::State> source_states = {
       {1, vic3::State({.owner_number = 1, .owner_tag = "TAG", .provinces = {1, 2, 3}})},
       {2, vic3::State({.owner_number = 2, .owner_tag = "TAG", .provinces = {4, 5, 6}})},
   };

   const std::map<std::string, vic3::StateRegion> source_state_regions = {
       {"REGION_ONE", vic3::StateRegion({{"x000001", "city"}}, {"x000001", "x000002", "x000003"})},
       {"REGION_TWO", vic3::StateRegion({{"x000004", "mine"}}, {"x000004", "x000005", "x000006"})},
   };

   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock country_block_one("TAG", "english");
   country_block_one.ModifyLocalization("english", "test");
   country_block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("TAG", country_block_one);
   commonItems::LocalizationBlock country_adj_block_one("TAG_ADJ", "english");
   country_adj_block_one.ModifyLocalization("english", "testy");
   country_adj_block_one.ModifyLocalization("spanish", "pruebamente");
   vic3_localizations.AddOrModifyLocalizationBlock("TAG_ADJ", country_adj_block_one);
   commonItems::LocalizationBlock country_block_two("THR", "english");
   country_block_two.ModifyLocalization("english", "test three");
   country_block_two.ModifyLocalization("spanish", "prueba tres");
   vic3_localizations.AddOrModifyLocalizationBlock("THR", country_block_two);
   commonItems::LocalizationBlock country_adj_block_two("THR_ADJ", "english");
   country_adj_block_two.ModifyLocalization("english", "testy three");
   country_adj_block_two.ModifyLocalization("spanish", "pruebamente tres");
   vic3_localizations.AddOrModifyLocalizationBlock("THR_ADJ", country_adj_block_two);
   commonItems::LocalizationBlock state_block_one("REGION_ONE", "english");
   state_block_one.ModifyLocalization("english", "test");
   state_block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("REGION_ONE", state_block_one);
   commonItems::LocalizationBlock state_block_two("REGION_TWO", "english");
   state_block_two.ModifyLocalization("english", "test two");
   state_block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("REGION_TWO", state_block_two);
   commonItems::LocalizationBlock victory_point_block_one("HUB_NAME_REGION_ONE_city", "english");
   victory_point_block_one.ModifyLocalization("english", "test");
   victory_point_block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_ONE_city", victory_point_block_one);
   commonItems::LocalizationBlock victory_point_block_two("HUB_NAME_REGION_TWO_mine", "english");
   victory_point_block_two.ModifyLocalization("english", "test two");
   victory_point_block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("HUB_NAME_REGION_TWO_mine", victory_point_block_two);

   vic3::ProvinceDefinitions province_definitions({"x000001", "x000002", "x000003", "x000004", "x000005", "x000006"});

   const vic3::World source_world({
       .countries = source_countries,
       .states = source_states,
       .state_regions = source_state_regions,
       .province_definitions = province_definitions,
       .localizations = vic3_localizations,
   });

   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({{1, "TAG"}, {3, "THR"}})
                                                 .AddProvinces({
                                                     {"x000001", {10}},
                                                     {"x000002", {20}},
                                                     {"x000003", {30}},
                                                     {"x000004", {40}},
                                                     {"x000005", {50}},
                                                     {"x000006", {60}},
                                                 })
                                                 .DefaultCultureGraphicsMapper()
                                                 .Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateDefaultWorldFramework(
              commonItems::ModFilesystem("test_files/hoi4_world", {}))
              .Build();
       }));

   const std::optional<commonItems::LocalizationBlock> hoi_country_localization_block =
       world.GetLocalizations().GetCountryLocalizations().GetLocalizationBlock("TAG");
   EXPECT_THAT(hoi_country_localization_block.value_or(commonItems::LocalizationBlock("", "")).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_country_localization_block_two =
       world.GetLocalizations().GetCountryLocalizations().GetLocalizationBlock("THR");
   EXPECT_THAT(hoi_country_localization_block_two.value_or(commonItems::LocalizationBlock("", "")).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test three"), testing::Pair("spanish", "prueba tres")));

   const std::optional<commonItems::LocalizationBlock> hoi_state_localization_block =
       world.GetLocalizations().GetStateLocalizations().GetLocalizationBlock("STATE_1");
   EXPECT_THAT(hoi_state_localization_block.value_or(commonItems::LocalizationBlock("", "")).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_state_localization_block_two =
       world.GetLocalizations().GetStateLocalizations().GetLocalizationBlock("STATE_2");
   EXPECT_THAT(hoi_state_localization_block_two.value_or(commonItems::LocalizationBlock("", "")).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));

   const std::optional<commonItems::LocalizationBlock> hoi_victory_point_localization_block =
       world.GetLocalizations().GetVictoryPointLocalizations().GetLocalizationBlock("VICTORY_POINTS_10");
   EXPECT_THAT(hoi_victory_point_localization_block.value_or(commonItems::LocalizationBlock("", "")).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_victory_point_localization_block_two =
       world.GetLocalizations().GetVictoryPointLocalizations().GetLocalizationBlock("VICTORY_POINTS_40");
   EXPECT_THAT(
       hoi_victory_point_localization_block_two.value_or(commonItems::LocalizationBlock("", "")).GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));
}


TEST(Hoi4worldWorldHoi4worldconverter, CharactersAreConverted)
{
   const vic3::Country source_country_one({
       .number = 1,
       .tag = "TAG",
       .color = commonItems::Color{std::array{1, 2, 3}},
       .head_of_state_id = 1,
       .character_ids = {1, 4, 5, 6},
   });
   const vic3::Country source_country_two({
       .number = 3,
       .tag = "TWO",
       .color = commonItems::Color{std::array{2, 4, 6}},
       .head_of_state_id = 3,
       .character_ids = {2, 3},
   });

   // Compiler was being weird about it directly in map
   vic3::Character character_one({.id = 1, .culture = "culture_2"});
   vic3::Character character_two({.id = 2, .culture = "culture_2", .roles = {"agitator"}, .origin_country_id = 1});
   vic3::Character character_three({.id = 3, .culture = "culture_2"});
   vic3::Character character_four({.id = 4, .culture = "culture_1", .roles = {"general"}});
   vic3::Character character_five({.id = 5, .culture = "culture_2", .roles = {"admiral"}});
   vic3::Character character_six({.id = 6, .culture = "culture_2", .roles = {"politician"}});
   const vic3::World source_world({
       .countries = {{1, source_country_one}, {3, source_country_two}},
       .states = {},
       .acquired_technologies =
           {
               {1, {"source_tech"}},
               {3, {"source_tech_two", "source_tech_three"}},
           },
       .culture_definitions =
           {
               {"culture_1", vic3::CultureDefinition({.name = "culture_1"})},
               {"culture_2", vic3::CultureDefinition({.name = "culture_2"})},
           },
       .characters =
           {
               {1, character_one},
               {2, character_two},
               {3, character_three},
               {4, character_four},
               {5, character_five},
               {6, character_six},
           },
   });
   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({{1, "TAG"}, {3, "TWO"}})
                                                 .DefaultCultureGraphicsMapper()
                                                 .Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateNullWorldFramework().Build();
       }));

   const auto expected_data_leader = std::optional<Leader>({.sub_ideology = "despotism"});
   const auto expected_data_spy = std::optional<Spy>({.nationalities = {"TAG", "TWO"}});
   const auto expected_data_general = std::optional<General>({.traits = {}});
   const auto expected_data_admiral = std::optional<Admiral>({.traits = {}});
   const auto expected_data_advisor = std::optional<Advisor>({.slot = "political_advisor"});


   EXPECT_THAT(world.GetCharacters(),
       testing::UnorderedElementsAre(testing::Pair(1,
                                         Character({
                                             .id = 1,
                                             .portrait_alias = "GFX_leader_2",
                                             .leader_data = expected_data_leader,
                                         })),
           testing::Pair(2,
               Character({
                   .id = 2,
                   .portrait_alias = "GFX_m_op_1",
                   .spy_data = expected_data_spy,
               })),
           testing::Pair(3,
               Character({
                   .id = 3,
                   .portrait_alias = "GFX_leader_1",
                   .leader_data = expected_data_leader,
               })),
           testing::Pair(4,
               Character({
                   .id = 4,
                   .portrait_alias = "GFX_general_1",
                   .general_data = expected_data_general,
               })),
           testing::Pair(5,
               Character({
                   .id = 5,
                   .portrait_alias = "GFX_admiral_1",
                   .admiral_data = expected_data_admiral,
               })),
           testing::Pair(6,
               Character({
                   .id = 6,
                   .portrait_alias = "GFX_minister_1",
                   .advisor_data = expected_data_advisor,
               }))));
}


TEST(Hoi4worldWorldHoi4worldconverter, WarsAreConverted)
{
   const vic3::Country source_country_one({
       .number = 1,
       .tag = "TAG",
       .color = commonItems::Color{std::array{1, 2, 3}},
       .head_of_state_id = 1,
       .character_ids = {1, 4, 5, 6},
   });
   const vic3::Country source_country_two({
       .number = 3,
       .tag = "TWO",
       .color = commonItems::Color{std::array{2, 4, 6}},
       .head_of_state_id = 3,
       .character_ids = {2, 3},
   });

   const vic3::World source_world({
       .countries = {{1, source_country_one}, {3, source_country_two}},
       .states = {},
       .wars = {vic3::War({.original_attacker = 1, .original_defender = 3})},
   });
   const mappers::WorldMapper world_mapper = mappers::WorldMapperBuilder::CreateNullMapper()
                                                 .AddCountries({{1, "TAG"}, {3, "TWO"}})
                                                 .DefaultCultureGraphicsMapper()
                                                 .Build();

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       world_mapper,
       std::async<>(std::launch::async, []() {
          return hoi4::WorldFrameworkBuilder::CreateNullWorldFramework().Build();
       }));

   EXPECT_THAT(world.GetCountries().find("TAG")->second.GetWars(),
       testing::ElementsAre(War({.original_defender = "TWO", .original_attacker = "TAG"})));
}

}  // namespace hoi4
