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
   commonItems::TryCreateFolder(fmt::format("output/{}/common/bookmarks", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/characters", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/countries", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/country_tags", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/decisions", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/decisions/categories", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/ideas", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/names", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/national_focus", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/countries", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/states", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/units", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/localisation", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/map", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/map/strategicregions", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/portraits", test_name));
}

}  // namespace



namespace out
{

TEST(Outhoi4WorldOutworld, CountriesFilesAreCreated)
{
   CreateTestFolders("WorldCountriesFilesAreCreated");

   OutputWorld("WorldCountriesFilesAreCreated",
       hoi4::World(hoi4::WorldOptions{
           .countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCountriesFilesAreCreated/common/countries/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCountriesFilesAreCreated/common/countries/TWO.txt"));
}


TEST(Outhoi4WorldOutworld, TagsFileIsCreated)
{
   CreateTestFolders("WorldTagsFileIsCreated");

   OutputWorld("WorldTagsFileIsCreated",
       hoi4::World({.countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}),
       configuration::UseStories::kNo);

   std::ifstream country_file("output/WorldTagsFileIsCreated/common/country_tags/00_countries.txt");
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


TEST(Outhoi4WorldOutworld, CharactersFilesAreCreated)
{
   CreateTestFolders("WorldCharactersFilesAreCreated");

   OutputWorld("WorldCharactersFilesAreCreated",
       hoi4::World(hoi4::WorldOptions{
           .countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCharactersFilesAreCreated/common/characters/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCharactersFilesAreCreated/common/characters/TWO.txt"));
}


TEST(Outhoi4WorldOutworld, NamesFileIsCreated)
{
   CreateTestFolders("WorldNamesFileIsCreated");

   OutputWorld("WorldNamesFileIsCreated",
       hoi4::World({.countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldNamesFileIsCreated/common/names/converter_names.txt"));
}


TEST(Outhoi4WorldOutworld, CountryHistoryFilesAreCreated)
{
   CreateTestFolders("WorldCountryHistoryFilesAreCreated");

   OutputWorld("WorldCountryHistoryFilesAreCreated",
       hoi4::World({.countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCountryHistoryFilesAreCreated/history/countries/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCountryHistoryFilesAreCreated/history/countries/TWO.txt"));
}


TEST(Outhoi4WorldOutworld, DivisionTemplatesAreCopied)
{
   CreateTestFolders("WorldDivisionTemplatesAreCopied");

   OutputWorld("WorldDivisionTemplatesAreCopied",
       hoi4::World({.countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldDivisionTemplatesAreCopied/history/units/TAG_1936.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldDivisionTemplatesAreCopied/history/units/TWO_1936.txt"));
}


TEST(Outhoi4WorldOutworld, StatesHistoryFilesAreCreatedAndOutput)
{
   CreateTestFolders("StatesHistoryFilesAreCreatedAreOutput");

   OutputWorld("StatesHistoryFilesAreCreatedAreOutput",
       hoi4::World({.states = {.states = {hoi4::State(1, {}), hoi4::State(2, {})}, .province_to_state_id_map = {}}}),
       configuration::UseStories::kNo);

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
       hoi4::World({.strategic_regions = hoi4::StrategicRegions({.strategic_regions = strategic_regions})}),
       configuration::UseStories::kNo);

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

   OutputWorld("BuildingsFileIsCreatedAndOutput",
       hoi4::World({.buildings = buildings}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/BuildingsFileIsCreatedAndOutput/map/buildings.txt"));
}


TEST(Outhoi4WorldOutworld, RailwaysFileIsCreatedAndOutput)
{
   CreateTestFolders("RailwaysFileIsCreatedAndOutput");

   const hoi4::Railways railways_list{.railways = {
                                          hoi4::Railway(1, {1, 2}),
                                          hoi4::Railway(2, {2, 4, 8}),
                                      }};

   OutputWorld("RailwaysFileIsCreatedAndOutput",
       hoi4::World({.railways = railways_list}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/BuildingsFileIsCreatedAndOutput/map/railways.txt"));
}


TEST(Outhoi4WorldOutworld, SupplyNodesFileIsCreatedAndOutput)
{
   CreateTestFolders("SupplyNodesFileIsCreatedAndOutput");

   const hoi4::Railways railways_list{
       .railways =
           {
               hoi4::Railway(1, {1, 2}),
               hoi4::Railway(2, {2, 4, 8}),
           },
       .supply_nodes = {1, 2, 4, 8},
   };

   OutputWorld("SupplyNodesFileIsCreatedAndOutput",
       hoi4::World({.railways = railways_list}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/BuildingsFileIsCreatedAndOutput/map/supply_nodes.txt"));
}


TEST(Outhoi4WorldOutworld, PortraitsFileIsCreated)
{
   CreateTestFolders("WorldPortraitsFileIsCreated");

   OutputWorld("WorldPortraitsFileIsCreated",
       hoi4::World({.countries = {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}}),
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldPortraitsFileIsCreated/portraits/converter_portraits.txt"));
}


TEST(Outhoi4WorldOutworld, LocalizationsAreOutput)
{
   CreateTestFolders("LocalizationsAreOutput");

   commonItems::LocalizationDatabase country_localizations("english", {"spanish"});
   commonItems::LocalizationBlock country_block_one("test_country", "english");
   country_block_one.ModifyLocalization("english", "test");
   country_block_one.ModifyLocalization("spanish", "prueba");
   country_localizations.AddOrModifyLocalizationBlock("test_country", country_block_one);
   commonItems::LocalizationBlock country_block_two("test_country_two", "english");
   country_block_two.ModifyLocalization("english", "test two");
   country_block_two.ModifyLocalization("spanish", "prueba dos");
   country_localizations.AddOrModifyLocalizationBlock("test_country_two", country_block_two);

   commonItems::LocalizationDatabase state_localizations("english", {"spanish"});
   commonItems::LocalizationBlock state_block_one("test_state", "english");
   state_block_one.ModifyLocalization("english", "test");
   state_block_one.ModifyLocalization("spanish", "prueba");
   state_localizations.AddOrModifyLocalizationBlock("test_state", state_block_one);
   commonItems::LocalizationBlock state_block_two("test_state_two", "english");
   state_block_two.ModifyLocalization("english", "test two");
   state_block_two.ModifyLocalization("spanish", "prueba dos");
   state_localizations.AddOrModifyLocalizationBlock("test_state_two", state_block_two);

   commonItems::LocalizationDatabase victory_point_localizations("english", {"spanish"});
   commonItems::LocalizationBlock victory_points_block_one("test_state", "english");
   victory_points_block_one.ModifyLocalization("english", "test");
   victory_points_block_one.ModifyLocalization("spanish", "prueba");
   victory_point_localizations.AddOrModifyLocalizationBlock("test_state", victory_points_block_one);
   commonItems::LocalizationBlock victory_points_block_two("test_state_two", "english");
   victory_points_block_two.ModifyLocalization("english", "test two");
   victory_points_block_two.ModifyLocalization("spanish", "prueba dos");
   victory_point_localizations.AddOrModifyLocalizationBlock("test_state_two", victory_points_block_two);

   commonItems::LocalizationDatabase character_localizations("english", {"spanish"});
   commonItems::LocalizationBlock character_block_one("test_state", "english");
   character_block_one.ModifyLocalization("english", "test");
   character_block_one.ModifyLocalization("spanish", "prueba");
   character_localizations.AddOrModifyLocalizationBlock("test_state", character_block_one);
   commonItems::LocalizationBlock character_block_two("test_state_two", "english");
   character_block_two.ModifyLocalization("english", "test two");
   character_block_two.ModifyLocalization("spanish", "prueba dos");
   character_localizations.AddOrModifyLocalizationBlock("test_state_two", character_block_two);

   commonItems::LocalizationDatabase idea_localizations("english", {"spanish"});

   const hoi4::Localizations localizations(country_localizations,
       state_localizations,
       victory_point_localizations,
       character_localizations,
       idea_localizations);

   OutputWorld("LocalizationsAreOutput", hoi4::World({.localizations = localizations}), configuration::UseStories::kNo);
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/braz_por/countries_l_braz_por.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/english/countries_l_english.yml"));
   ASSERT_TRUE(commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/french/countries_l_french.yml"));
   ASSERT_TRUE(commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/german/countries_l_german.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/japanese/countries_l_japanese.yml"));
   ASSERT_TRUE(commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/polish/countries_l_polish.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/russian/countries_l_russian.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/spanish/countries_l_spanish.yml"));

   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/braz_por/state_names_l_braz_por.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/english/state_names_l_english.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/french/state_names_l_french.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/german/state_names_l_german.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/japanese/state_names_l_japanese.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/polish/state_names_l_polish.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/russian/state_names_l_russian.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/spanish/state_names_l_spanish.yml"));

   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/braz_por/victory_points_l_braz_por.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/english/victory_points_l_english.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/french/victory_points_l_french.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/german/victory_points_l_german.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/japanese/victory_points_l_japanese.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/polish/victory_points_l_polish.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/russian/victory_points_l_russian.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/spanish/victory_points_l_spanish.yml"));

   ASSERT_TRUE(commonItems::DoesFileExist(
       "output/LocalizationsAreOutput/localisation/braz_por/character_names_l_braz_por.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/english/character_names_l_english.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/french/character_names_l_french.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/german/character_names_l_german.yml"));
   ASSERT_TRUE(commonItems::DoesFileExist(
       "output/LocalizationsAreOutput/localisation/japanese/character_names_l_japanese.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/polish/character_names_l_polish.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/russian/character_names_l_russian.yml"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/LocalizationsAreOutput/localisation/spanish/character_names_l_spanish.yml"));
}


TEST(Outhoi4WorldOutworld, BookmarkIsOutput)
{
   CreateTestFolders("BookmarkIsOutput");

   OutputWorld("BookmarkIsOutput",
       hoi4::World({
           .great_powers = {"ONE", "TWO"},
           .major_powers = {"THR", "FOR"},
       }),
       configuration::UseStories::kNo);

   ASSERT_TRUE(commonItems::DoesFileExist("output/BookmarkIsOutput/common/bookmarks/the_grand_campaign.txt"));
   std::ifstream bookmark_file("output/BookmarkIsOutput/common/bookmarks/the_grand_campaign.txt");
   ASSERT_TRUE(bookmark_file.is_open());
   std::stringstream bookmark_file_stream;
   std::copy(std::istreambuf_iterator<char>(bookmark_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(bookmark_file_stream));
   bookmark_file.close();
   EXPECT_EQ(bookmark_file_stream.str(),
       "bookmarks = {\n"
       "\tbookmark = {\n"
       "\t\tname = GRAND_CAMPAIGN_NAME\n"
       "\t\tdesc = GRAND_CAMPAIGN_DESC\n"
       "\t\tdate = 1936.1.1.12\n"
       "\t\tpicture = GFX_select_date_1936\n"
       "\t\tdefault_country = \"---\"\n"
       "\t\tdefault = yes\n"
       "\t\tONE= {}\n"
       "\t\tTWO= {}\n"
       "\t\t\"---\"= {\n"
       "\t\t\thistory = \"OTHER_GRAND_CAMPAIGN_DESC\"\n"
       "\t\t}\n"
       "\t\tFOR = {\n"
       "\t\t\tminor = yes\n"
       "\t\t}\n"
       "\t\tTHR = {\n"
       "\t\t\tminor = yes\n"
       "\t\t}\n"
       "\t\teffect = {\n"
       "\t\t\trandomize_weather = 22345 # <-Obligatory in every bookmark !\n"
       "\t\t\t#123 = { rain_light = yes }\n"
       "\t\t}\n"
       "\t}\n"
       "}\n");
}


TEST(Outhoi4WorldOutworld, ExceptionIfBookmarkFileNotCreated)
{
   CreateTestFolders("ExceptionIfBookmarkFileNotCreated");
   std::filesystem::remove("output/ExceptionIfBookmarkFileNotCreated/common/bookmarks");

   EXPECT_THROW(OutputWorld("ExceptionIfBookmarkFileNotCreated", hoi4::World({}), configuration::UseStories::kNo),
       std::runtime_error);
}

}  // namespace out