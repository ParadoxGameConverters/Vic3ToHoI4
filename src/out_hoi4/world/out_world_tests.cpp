#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/world/out_world.h"



namespace
{

void CreateTestFolders(std::string_view test_name)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder(fmt::format("output/{}", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/countries", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/country_tags", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/countries", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/states", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/map", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/map/strategicregions", test_name));
}

}  // namespace



namespace out
{

TEST(Outhoi4WorldOutworld, CountriesFilesAreCreated)
{
   CreateTestFolders("WorldCountriesFilesAreCreated");

   OutputWorld("WorldCountriesFilesAreCreated",
       hoi4::World(hoi4::WorldOptions{
           .countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}));

   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCountriesFilesAreCreated/common/countries/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCountriesFilesAreCreated/common/countries/TWO.txt"));
}


TEST(Outhoi4WorldOutworld, TagsFileIsCreated)
{
   CreateTestFolders("TagsFileIsCreated");

   OutputWorld("TagsFileIsCreated",
       hoi4::World({.countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}));

   std::ifstream country_file("output/TagsFileIsCreated/common/country_tags/00_countries.txt");
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();
   EXPECT_EQ(country_file_stream.str(),
       "TAG = \"countries/TAG.txt\"\n"
       "TWO = \"countries/TWO.txt\"\n");
}


TEST(Outhoi4WorldOutworld, CountryHistoryFilesAreCreated)
{
   CreateTestFolders("CountryHistoryFilesAreCreated");

   OutputWorld("CountryHistoryFilesAreCreated",
       hoi4::World({.countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}));

   EXPECT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFilesAreCreated/history/countries/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFilesAreCreated/history/countries/TWO.txt"));
}


TEST(Outhoi4WorldOutworld, StatesHistoryFilesAreCreatedAndOutput)
{
   CreateTestFolders("StatesHistoryFilesAreCreatedAreOutput");

   OutputWorld("StatesHistoryFilesAreCreatedAreOutput",
       hoi4::World({.states = {.states = {hoi4::State(1, {}), hoi4::State(2, {})}, .province_to_state_id_map = {}}}));

   EXPECT_TRUE(commonItems::DoesFileExist("output/StatesHistoryFilesAreCreatedAreOutput/history/states/1.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/StatesHistoryFilesAreCreatedAreOutput/history/states/2.txt"));
}


TEST(Outhoi4WorldOutworld, StrategicRegionsFilesAreCreatedAndOutput)
{
   CreateTestFolders("StrategicRegionsFilesAreCreatedAndOutput");

   const std::map<int, hoi4::StrategicRegion> strategic_regions = {
       {1, hoi4::StrategicRegion({.filename = "strategic_region_1.txt", .id = 1})},
       {2, hoi4::StrategicRegion({.filename = "strategic_region_2.txt", .id = 2})}};

   OutputWorld("StrategicRegionsFilesAreCreatedAndOutput",
       hoi4::World({.strategic_regions = hoi4::StrategicRegions({.strategic_regions = strategic_regions})}));

   EXPECT_TRUE(commonItems::DoesFileExist(
       "output/StrategicRegionsFilesAreCreatedAndOutput/map/strategicregions/strategic_region_1.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist(
       "output/StrategicRegionsFilesAreCreatedAndOutput/map/strategicregions/strategic_region_2.txt"));
}


TEST(Outhoi4WorldOutworld, BuildingsFileIsCreatedAndOutput)
{
   CreateTestFolders("BuildingsFileIsCreatedAndOutput");

   const std::vector<hoi4::Building> buildings_list(
       {hoi4::Building({.state_id = 42,
            .type = "test_type",
            .position = {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25}}),
           hoi4::Building({.state_id = 144,
               .type = "second_type",
               .position = {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25},
               .connecting_sea_province = 145}),
           hoi4::Building({.state_id = 169,
               .type = "third_type",
               .position = {.x_coordinate = 4.25, .y_coordinate = 9.25, .z_coordinate = 16.25, .rotation = 25.25},
               .connecting_sea_province = 170})});

   const hoi4::BuildingsOptions options{.buildings = buildings_list, .airport_locations = {{2, 4}, {3, 9}, {4, 16}}};
   const hoi4::Buildings buildings(options);

   OutputWorld("BuildingsFileIsCreatedAndOutput", hoi4::World({.buildings = buildings}));
}

}  // namespace out