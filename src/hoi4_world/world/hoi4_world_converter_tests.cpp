#include <sstream>

#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/hoi4_world/world/hoi4_world_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/maps/province_definitions.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

TEST(Hoi4worldWorldHoi4worldconverter, EmptyWorldIsEmpty)
{
   const mappers::CountryMapper country_mapper;
   const vic3::World source_world(vic3::WorldOptions{});

   mappers::ProvinceMapper province_mapper{{}, {}};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       country_mapper,
       province_mapper);

   EXPECT_TRUE(world.GetCountries().empty());
   EXPECT_TRUE(world.GetStates().states.empty());
   EXPECT_TRUE(world.GetStates().province_to_state_id_map.empty());
}


TEST(Hoi4worldWorldHoi4worldconverter, CountriesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}});

   const vic3::World source_world({.countries = {{1, source_country_one}, {3, source_country_two}},
       .states = {},
       .acquired_technologies = {
           {1, {"source_tech"}},
           {3, {"source_tech_two", "source_tech_three"}},
       }});

   mappers::ProvinceMapper province_mapper{{}, {}};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       country_mapper,
       province_mapper);

   const Technologies expected_techs_one{std::map<std::optional<std::string>, std::set<std::string>>{
       {std::nullopt, std::set<std::string>{"dest_tech_one", "dest_tech_two"}}}};
   const Technologies expected_techs_two{std::map<std::optional<std::string>, std::set<std::string>>{
       {R"(not = { has_dlc = "Test DLC" })", std::set<std::string>{"dest_tech_three"}}}};

   EXPECT_THAT(world.GetCountries(),
       testing::ElementsAre(testing::Pair("TAG",
                                Country(CountryOptions{.tag = "TAG",
                                    .color = commonItems::Color{std::array{1, 2, 3}},
                                    .technologies = expected_techs_one})),
           testing::Pair("TWO",
               Country(CountryOptions{.tag = "TWO",
                   .color = commonItems::Color{std::array{2, 4, 6}},
                   .technologies = expected_techs_two}))));
}


TEST(Hoi4worldWorldHoi4worldconverter, StatesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});

   const auto province_definitions =
       vic3::ProvinceDefinitions({"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});

   const vic3::World source_world(
       vic3::WorldOptions{.states = {{1, vic3::State({.owner_tag = "TAG", .provinces = {1, 2, 3}})},
                              {2, vic3::State({.owner_number = 42, .owner_tag = "TWO", .provinces = {4, 5, 6}})}},

           .province_definitions = province_definitions});

   const mappers::ProvinceMapper province_mapper{{},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       }};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       country_mapper,
       province_mapper);

   EXPECT_THAT(world.GetStates().states,
       testing::ElementsAre(State(1, {.owner = "TAG", .provinces = {10, 20, 30}, .category = "enclave"}),
           State(2, {.owner = "TWO", .provinces = {40, 50, 60}, .category = "enclave"})));
   EXPECT_THAT(world.GetStates().province_to_state_id_map,
       testing::UnorderedElementsAre(testing::Pair(10, 1),
           testing::Pair(20, 1),
           testing::Pair(30, 1),
           testing::Pair(40, 2),
           testing::Pair(50, 2),
           testing::Pair(60, 2)));
}


TEST(Hoi4worldWorldHoi4worldconverter, StrategicRegionsAreCreated)
{
   const mappers::CountryMapper country_mapper;

   const auto province_definitions =
       vic3::ProvinceDefinitions({"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"});

   const vic3::World source_world(
       {.states = {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
           .province_definitions = province_definitions});

   const mappers::ProvinceMapper province_mapper{{},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       }};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/StrategicRegionsAreCreated", {}),
       source_world,
       country_mapper,
       province_mapper);

   const auto strategic_regions = world.GetStrategicRegions().GetStrategicRegions();
   ASSERT_TRUE(strategic_regions.contains(10));
   ASSERT_TRUE(strategic_regions.contains(50));

   const auto region_10 = strategic_regions.find(10)->second;
   EXPECT_EQ(region_10.GetFilename(), "strategic_region_10.txt");
   EXPECT_EQ(region_10.GetID(), 10);
   EXPECT_EQ(region_10.GetName(), "STRATEGICREGION_10");
   EXPECT_THAT(region_10.GetOldProvinces(), testing::UnorderedElementsAre(10, 20, 30, 40));
   EXPECT_THAT(region_10.GetNewProvinces(), testing::UnorderedElementsAre(10, 20, 30));
   EXPECT_TRUE(region_10.hasStaticModifiers());
   EXPECT_THAT(region_10.GetStaticModifiers(),
       testing::UnorderedElementsAre(testing::Pair("test_modifier", "always"),
           testing::Pair("test_modifier_two", "always")));
   ASSERT_TRUE(region_10.GetNavalTerrain().has_value());
   EXPECT_EQ(region_10.GetNavalTerrain().value(), "test_naval_terrain");
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
   EXPECT_FALSE(region_50.hasStaticModifiers());
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
   const mappers::CountryMapper country_mapper;

   const auto province_definitions = vic3::ProvinceDefinitions(
       {"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006", "0x000007", "0x000008"});

   const vic3::World source_world({.states = {{1, vic3::State({.provinces = {1, 2, 3, 4, 5}})},
                                       {2, vic3::State({.provinces = {6, 7}})},
                                       {3, vic3::State({.provinces = {8}})}},
       .province_definitions = province_definitions});

   const mappers::ProvinceMapper province_mapper{{},
       {
           {1, {"0x000001"}},
           {2, {"0x000002"}},
           {3, {"0x000003"}},
           {4, {"0x000004"}},
           {5, {"0x000005"}},
           {6, {"0x000006"}},
           {7, {"0x000007"}},
           {8, {"0x000008"}},
       }};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/BuildingsAreCreated", {}),
       source_world,
       country_mapper,
       province_mapper);

   EXPECT_FALSE(world.GetBuildings().GetBuildings().empty());
   EXPECT_FALSE(world.GetBuildings().GetAirportLocations().empty());
}


TEST(Hoi4worldWorldHoi4worldconverter, LocalizationsAreConverted)
{
   const vic3::Country source_country_one({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}});
   const std::map<int, vic3::Country> source_countries = {{1, source_country_one}, {3, source_country_two}};

   const std::map<int, vic3::State> source_states = {{1, vic3::State({.owner_tag = "TAG", .provinces = {1, 2, 3}})},
       {2, vic3::State({.owner_tag = "TAG", .provinces = {4, 5, 6}})}};

   const std::map<std::string, vic3::StateRegion> source_state_regions = {
       {"REGION_ONE", vic3::StateRegion({}, {"x000001", "x000002", "x000003"})},
       {"REGION_TWO", vic3::StateRegion({}, {"x000004", "x000005", "x000006"})}};

   commonItems::LocalizationDatabase vic3_localizations("english", {"spanish"});
   commonItems::LocalizationBlock country_block_one("TAG", "english");
   country_block_one.ModifyLocalization("english", "test");
   country_block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("TAG", country_block_one);
   commonItems::LocalizationBlock country_block_two("TWO", "english");
   country_block_two.ModifyLocalization("english", "test two");
   country_block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("TWO", country_block_two);
   commonItems::LocalizationBlock state_block_one("REGION_ONE", "english");
   state_block_one.ModifyLocalization("english", "test");
   state_block_one.ModifyLocalization("spanish", "prueba");
   vic3_localizations.AddOrModifyLocalizationBlock("REGION_ONE", state_block_one);
   commonItems::LocalizationBlock state_block_two("REGION_TWO", "english");
   state_block_two.ModifyLocalization("english", "test two");
   state_block_two.ModifyLocalization("spanish", "prueba dos");
   vic3_localizations.AddOrModifyLocalizationBlock("REGION_TWO", state_block_two);

   vic3::ProvinceDefinitions province_definitions({"x000001", "x000002", "x000003", "x000004", "x000005", "x000006"});

   const vic3::World source_world({.countries = source_countries,
       .states = source_states,
       .state_regions = source_state_regions,
       .province_definitions = province_definitions,
       .localizations = vic3_localizations});

   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});

   const mappers::Vic3ToHoi4ProvinceMapping vic3_to_hoi4_province_map = {{"x000001", {10}},
       {"x000002", {20}},
       {"x000003", {30}},
       {"x000004", {40}},
       {"x000005", {50}},
       {"x000006", {60}}};
   const mappers::Hoi4ToVic3ProvinceMapping hoi4_to_vic3_province_map = {{10, {"x000001"}},
       {20, {"x000002"}},
       {30, {"x000003"}},
       {40, {"x000004"}},
       {50, {"x000005"}},
       {60, {"x000006"}}};
   const mappers::ProvinceMapper province_mapper{vic3_to_hoi4_province_map, hoi4_to_vic3_province_map};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       country_mapper,
       province_mapper);

   const std::optional<commonItems::LocalizationBlock> hoi_country_localization_block =
       world.GetLocalizations().GetCountryLocalizations().GetLocalizationBlock("TAG");
   ASSERT_TRUE(hoi_country_localization_block.has_value());
   EXPECT_THAT(hoi_country_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_country_localization_block_two =
       world.GetLocalizations().GetCountryLocalizations().GetLocalizationBlock("TWO");
   ASSERT_TRUE(hoi_country_localization_block_two.has_value());
   EXPECT_THAT(hoi_country_localization_block_two->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));

   const std::optional<commonItems::LocalizationBlock> hoi_state_localization_block =
       world.GetLocalizations().GetStateLocalizations().GetLocalizationBlock("STATE_1");
   ASSERT_TRUE(hoi_state_localization_block.has_value());
   EXPECT_THAT(hoi_state_localization_block->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test"), testing::Pair("spanish", "prueba")));

   const std::optional<commonItems::LocalizationBlock> hoi_state_localization_block_two =
       world.GetLocalizations().GetStateLocalizations().GetLocalizationBlock("STATE_2");
   ASSERT_TRUE(hoi_state_localization_block_two.has_value());
   EXPECT_THAT(hoi_state_localization_block_two->GetLocalizations(),
       testing::UnorderedElementsAre(testing::Pair("english", "test two"), testing::Pair("spanish", "prueba dos")));
}

}  // namespace hoi4