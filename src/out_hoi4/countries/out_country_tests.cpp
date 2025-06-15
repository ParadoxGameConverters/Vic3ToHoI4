#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/countries/out_country.h"



using std::filesystem::create_directories;
using std::filesystem::path;
using std::filesystem::remove_all;



namespace out
{

TEST(Outhoi4CountriesOutcountryTests, CommonCountriesFileIsCreatedWithTagForName)
{
   create_directories("output/CommonCountriesFileIsCreatedWithTagForName/common/countries");

   const hoi4::Country country({.tag = "TAG", .color = commonItems::Color(std::array{1, 2, 3})});
   const hoi4::Country country_two({.tag = "TWO", .color = commonItems::Color(std::array{2, 4, 6})});

   OutputCommonCountriesFile("CommonCountriesFileIsCreatedWithTagForName", country);
   OutputCommonCountriesFile("CommonCountriesFileIsCreatedWithTagForName", country_two);

   const path tag_path("output/CommonCountriesFileIsCreatedWithTagForName/common/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   std::stringstream expected_one;
   expected_one << "graphical_culture = western_european_gfx\n";
   expected_one << "graphical_culture_2d = western_european_2d\n";
   expected_one << "color = rgb { 1 2 3 }";
   EXPECT_EQ(country_file_stream.str(), expected_one.str());

   const path two_path("output/CommonCountriesFileIsCreatedWithTagForName/common/countries/TWO.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(two_path));
   std::ifstream country_file_two(two_path);
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();

   std::stringstream expected_two;
   expected_two << "graphical_culture = western_european_gfx\n";
   expected_two << "graphical_culture_2d = western_european_2d\n";
   expected_two << "color = rgb { 2 4 6 }";
   EXPECT_EQ(country_file_two_stream.str(), expected_two.str());
}


TEST(Outhoi4CountriesOutcountryTests, ExceptionIfCountriesFileNotOpened)
{
   const hoi4::Country country({.tag = "TAG"});

   EXPECT_THROW(OutputCommonCountriesFile("/dev/null/COM", country), std::runtime_error);
}


TEST(Outhoi4CountriesOutcountryTests, DefaultsAreOutputToCommonCountriesFile)
{
   create_directories("output/DefaultsAreOutputToCommonCountriesFile/common/countries");

   const hoi4::Country country({.tag = "TAG"});
   OutputCommonCountriesFile("DefaultsAreOutputToCommonCountriesFile", country);

   const path tag_path("output/DefaultsAreOutputToCommonCountriesFile/common/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   std::stringstream expected_one;
   expected_one << "graphical_culture = western_european_gfx\n";
   expected_one << "graphical_culture_2d = western_european_2d\n";
   expected_one << "color = rgb { 0 0 0 }";
   EXPECT_EQ(country_file_stream.str(), expected_one.str());
}


TEST(Outhoi4CountriesOutcountryTests, ColorCanBeSetInCommonCountriesFile)
{
   create_directories("output/ColorCanBeSetInCommonCountriesFile/common/countries");

   const hoi4::Country country({.tag = "TAG", .color = commonItems::Color(std::array{1, 2, 3})});
   OutputCommonCountriesFile("ColorCanBeSetInCommonCountriesFile", country);

   const path tag_path("output/ColorCanBeSetInCommonCountriesFile/common/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   std::stringstream expected_one;
   expected_one << "graphical_culture = western_european_gfx\n";
   expected_one << "graphical_culture_2d = western_european_2d\n";
   expected_one << "color = rgb { 1 2 3 }";
   EXPECT_EQ(country_file_stream.str(), expected_one.str());
}


TEST(Outhoi4CountriesOutcountryTests, TagsAreAddedToTagsFile)
{
   const hoi4::Country country({.tag = "TAG"});
   const hoi4::Country country_two({.tag = "TWO"});

   std::ofstream tags_file("tags_file.txt");
   ASSERT_TRUE(tags_file.is_open());

   OutputCommonCountryTag(country, tags_file);
   OutputCommonCountryTag(country_two, tags_file);

   tags_file.close();

   std::ifstream country_file("tags_file.txt");
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


TEST(Outhoi4CountriesOutcountryTests, CommonCharactersFileIsCreatedWithTagForName)
{
   create_directories("output/CommonCharactersFileIsCreatedWithTagForName/common/characters");

   const hoi4::Country country({.tag = "TAG", .character_ids = {1, 2}});
   const hoi4::Country country_two({.tag = "TWO", .character_ids = {3, 4}});
   const std::map<int, hoi4::Character> characters{
       {1, hoi4::Character({.id = 1})},
       {2, hoi4::Character({.id = 2})},
       {3, hoi4::Character({.id = 3})},
       {4, hoi4::Character({.id = 4})},
   };

   OutputCommonCharactersFile("CommonCharactersFileIsCreatedWithTagForName", country, characters);
   OutputCommonCharactersFile("CommonCharactersFileIsCreatedWithTagForName", country_two, characters);

   const path tag_path("output/CommonCharactersFileIsCreatedWithTagForName/common/characters/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream characters_file(tag_path);
   ASSERT_TRUE(characters_file.is_open());
   std::stringstream characters_file_stream;
   std::copy(std::istreambuf_iterator<char>(characters_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(characters_file_stream));
   characters_file.close();

   std::stringstream expected_one;
   expected_one << "characters = {\n";
   expected_one << "\tTAG_1 = {\n";
   expected_one << "\t\tname = _\n";
   expected_one << "\t\tportraits = {\n";
   expected_one << "\t\t}\n";
   expected_one << "\t}\n";
   expected_one << "\tTAG_2 = {\n";
   expected_one << "\t\tname = _\n";
   expected_one << "\t\tportraits = {\n";
   expected_one << "\t\t}\n";
   expected_one << "\t}\n";
   expected_one << "}\n";
   EXPECT_EQ(characters_file_stream.str(), expected_one.str());

   const path two_path("output/CommonCharactersFileIsCreatedWithTagForName/common/characters/TWO.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(two_path));
   std::ifstream characters_file_two(two_path);
   ASSERT_TRUE(characters_file_two.is_open());
   std::stringstream characters_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(characters_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(characters_file_two_stream));
   characters_file_two.close();

   std::stringstream expected_two;
   expected_two << "characters = {\n";
   expected_two << "\tTWO_3 = {\n";
   expected_two << "\t\tname = _\n";
   expected_two << "\t\tportraits = {\n";
   expected_two << "\t\t}\n";
   expected_two << "\t}\n";
   expected_two << "\tTWO_4 = {\n";
   expected_two << "\t\tname = _\n";
   expected_two << "\t\tportraits = {\n";
   expected_two << "\t\t}\n";
   expected_two << "\t}\n";
   expected_two << "}\n";
   EXPECT_EQ(characters_file_two_stream.str(), expected_two.str());
}


TEST(Outhoi4CountriesOutcountryTests, CountryHistoryFileIsCreatedWithTagForName)
{
   create_directories("output/CountryHistoryFileIsCreatedWithTagForName/history/countries");

   const hoi4::Country country({.tag = "TAG"});
   const hoi4::Country country_two({.tag = "TWO"});

   OutputCountryHistory("CountryHistoryFileIsCreatedWithTagForName", country, {});
   OutputCountryHistory("CountryHistoryFileIsCreatedWithTagForName", country_two, {});

   ASSERT_TRUE(
       commonItems::DoesFileExist("output/CountryHistoryFileIsCreatedWithTagForName/history/countries/TAG.txt"));
   ASSERT_TRUE(
       commonItems::DoesFileExist("output/CountryHistoryFileIsCreatedWithTagForName/history/countries/TWO.txt"));
}


TEST(Outhoi4CountriesOutcountryTests, ExceptionIfHistoryFileNotOpened)
{
   const hoi4::Country country({.tag = "TAG"});

   EXPECT_THROW(OutputCountryHistory("/dev/null/COM", country, {}), std::runtime_error);
}


TEST(Outhoi4CountriesOutcountryTests, DefaultsAreSetInCountryHistoryFile)
{
   create_directories("output/DefaultsAreSetInCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "TAG"});
   OutputCountryHistory("DefaultsAreSetInCountryHistoryFile", country, {});

   const path tag_path("output/DefaultsAreSetInCountryHistoryFile/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   std::stringstream expected_one;
   expected_one << "\n";
   expected_one << "oob = \"TAG_1936\"\n";
   expected_one << "set_research_slots = 3\n";
   expected_one << "set_convoys = 0\n";
   expected_one << "\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { has_dlc = \"La Resistance\" }\n";
   expected_one << "}\n";
   expected_one << "\n";
   expected_one << "set_politics = {\n";
   expected_one << "\truling_party = neutrality\n";
   expected_one << "\tlast_election = \"1933.1.1\"\n";
   expected_one << "\telection_frequency = 48\n";
   expected_one << "\telections_allowed = no\n";
   expected_one << "}\n";
   expected_one << "\n";
   expected_one << "set_popularities = {\n";
   expected_one << "}\n";
   expected_one << "\n";
   expected_one << "add_ideas = {\n";
   expected_one << "\tcivilian_economy\n";
   expected_one << "\texport_focus\n";
   expected_one << "\tvolunteer_only\n";
   expected_one << "}\n";
   expected_one << "set_stability = 0\n";
   expected_one << "set_war_support = 0.60\n";
   expected_one << "\n";
   expected_one << "# Starting tech\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { not = { has_dlc = \"Man the Guns\" } }\n";
   expected_one << "\tset_naval_oob = TAG_1936_Naval_Legacy\n";
   expected_one << "}\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { has_dlc = \"Man the Guns\" }\n";
   expected_one << "\tset_naval_oob = TAG_1936_Naval\n";
   expected_one << "}\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { has_dlc = \"By Blood Alone\" }\n";
   expected_one << "}\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { has_dlc = \"No Step Back\" }\n";
   expected_one << "}\n";
   EXPECT_EQ(country_file_stream.str(), expected_one.str());
}

TEST(Outhoi4CountriesOutcountryTests, ConvoysAreWrittenToCountryHistoryFile)
{
   create_directories("output/ConvoysAreWrittenToCountryHistoryFile/history/countries");

   const hoi4::Country country({
       .tag = "TAG",
       .convoys = 11,
   });
   OutputCountryHistory("ConvoysAreWrittenToCountryHistoryFile", country, {});

   const path tag_path("output/ConvoysAreWrittenToCountryHistoryFile/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();
   EXPECT_THAT(country_file_stream.str(), testing::HasSubstr("set_convoys = 11"));
}

TEST(Outhoi4CountriesOutcountryTests, IdeologyIsSetCountryHistoryFile)
{
   create_directories("output/IdeologyIsSetCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "TAG", .ideology = "test_ideology"});
   OutputCountryHistory("IdeologyIsSetCountryHistoryFile", country, {});

   const path tag_path("output/IdeologyIsSetCountryHistoryFile/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   EXPECT_THAT(country_file_stream.str(), testing::HasSubstr("ruling_party = test_ideology"));
}


TEST(Outhoi4CountriesOutcountryTests, IdeasAreOutputToCountryHistoryFile)
{
   create_directories("output/IdeasAreOutputToCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "TAG", .ideas = {"idea_one", "idea_two"}});
   OutputCountryHistory("IdeasAreOutputToCountryHistoryFile", country, {});

   const path tag_path("output/IdeasAreOutputToCountryHistoryFile/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   std::stringstream expected_one;
   expected_one << "\n";
   expected_one << "oob = \"TAG_1936\"\n";
   expected_one << "set_research_slots = 3\n";
   expected_one << "set_convoys = 0\n";
   expected_one << "\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { has_dlc = \"La Resistance\" }\n";
   expected_one << "}\n";
   expected_one << "\n";
   expected_one << "set_politics = {\n";
   expected_one << "\truling_party = neutrality\n";
   expected_one << "\tlast_election = \"1933.1.1\"\n";
   expected_one << "\telection_frequency = 48\n";
   expected_one << "\telections_allowed = no\n";
   expected_one << "}\n";
   expected_one << "\n";
   expected_one << "set_popularities = {\n";
   expected_one << "}\n";
   expected_one << "\n";
   expected_one << "add_ideas = {\n";
   expected_one << "\tidea_one\n";
   expected_one << "\tidea_two\n";
   expected_one << "\tcivilian_economy\n";
   expected_one << "\texport_focus\n";
   expected_one << "\tvolunteer_only\n";
   expected_one << "}\n";
   expected_one << "set_stability = 0\n";
   expected_one << "set_war_support = 0.60\n";
   expected_one << "\n";
   expected_one << "# Starting tech\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { not = { has_dlc = \"Man the Guns\" } }\n";
   expected_one << "\tset_naval_oob = TAG_1936_Naval_Legacy\n";
   expected_one << "}\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { has_dlc = \"Man the Guns\" }\n";
   expected_one << "\tset_naval_oob = TAG_1936_Naval\n";
   expected_one << "}\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { has_dlc = \"By Blood Alone\" }\n";
   expected_one << "}\n";
   expected_one << "if = {\n";
   expected_one << "\tlimit = { has_dlc = \"No Step Back\" }\n";
   expected_one << "}\n";
   EXPECT_EQ(country_file_stream.str(), expected_one.str());
}


TEST(Outhoi4CountriesOutcountryTests, LawsAreOutputToCountryHistoryFile)
{
   create_directories("output/IdeasAreOutputToCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "TAG",
       .economy_law = "test_economy_law",
       .trade_law = "test_trade_law",
       .military_law = "test_military_law"});
   OutputCountryHistory("IdeasAreOutputToCountryHistoryFile", country, {});

   const path tag_path("output/IdeasAreOutputToCountryHistoryFile/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   std::stringstream expected_one;
   expected_one << "add_ideas = {\n";
   expected_one << "\ttest_economy_law\n";
   expected_one << "\ttest_trade_law\n";
   expected_one << "\ttest_military_law\n";
   expected_one << "}\n";
   EXPECT_THAT(country_file_stream.str(), testing::HasSubstr(expected_one.str()));
}


TEST(Outhoi4CountriesOutcountryTests, CapitalCanBeSetInCountryHistoryFile)
{
   create_directories("output/CapitalCanBeSetInCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "TAG", .capital_state = 42});
   OutputCountryHistory("CapitalCanBeSetInCountryHistoryFile", country, {});

   const path tag_path("output/CapitalCanBeSetInCountryHistoryFile/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   EXPECT_THAT(country_file_stream.str(), testing::HasSubstr("capital = 42\n"));
}


TEST(Outhoi4CountriesOutcountryTests, CharactersAreRecruitedInCountryHistoryFile)
{
   create_directories("output/CharactersAreRecruitedInCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "TAG", .character_ids = {1, 2}});
   const std::map<int, hoi4::Character> characters{
       {1, hoi4::Character({.id = 1})},
       {2, hoi4::Character({.id = 2})},
   };
   OutputCountryHistory("CharactersAreRecruitedInCountryHistoryFile", country, characters);

   const path tag_path("output/CharactersAreRecruitedInCountryHistoryFile/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   EXPECT_THAT(country_file_stream.str(),
       testing::HasSubstr("recruit_character = TAG_1\n"
                          "recruit_character = TAG_2\n"));
}


TEST(Outhoi4CountriesOutcountryTests, SpiesAreDefinedInCountryHistoryFile)
{
   create_directories("output/SpiesAreDefinedInCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "TAG", .spy_ids = {1, 2}});
   const std::map<int, hoi4::Character> characters{
       {1, hoi4::Character({.spy_data = hoi4::Spy{}})},
       {2, hoi4::Character({.spy_data = hoi4::Spy{}})},
   };
   OutputCountryHistory("SpiesAreDefinedInCountryHistoryFile", country, characters);

   const path tag_path("output/SpiesAreDefinedInCountryHistoryFile/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   EXPECT_THAT(country_file_stream.str(),
       testing::HasSubstr("if = {\n"
                          "\tlimit = { has_dlc = \"La Resistance\" }\n"
                          "\tcreate_operative_leader = {\n"
                          "\t\tname = _\n"
                          "\t\tGFX = GFX_portrait_operative_unknown\n"
                          "\t\ttraits = {  }\n"
                          "\t\tbypass_recruitment = no\n"
                          "\t\tavailable_to_spy_master = yes\n"
                          "\t\tnationalities = {  }\n"
                          "\t}\n"
                          "\tcreate_operative_leader = {\n"
                          "\t\tname = _\n"
                          "\t\tGFX = GFX_portrait_operative_unknown\n"
                          "\t\ttraits = {  }\n"
                          "\t\tbypass_recruitment = no\n"
                          "\t\tavailable_to_spy_master = yes\n"
                          "\t\tnationalities = {  }\n"
                          "\t}\n"
                          "}\n"));
}


TEST(Outhoi4CountriesOutcountryTests, EquipmentVariantsAreOutput)
{
   create_directories("output/EquipmentVariantsAreOutput/history/countries");

   const std::vector<hoi4::EquipmentVariant> legacy_ship_variants = {
       hoi4::EquipmentVariant("legacy_ship: variant_one", "", {}, {}, {}),
       hoi4::EquipmentVariant("legacy_ship: variant_two", "", {}, {"blocking_tech_missing"}, {}),
   };
   const std::vector<hoi4::EquipmentVariant> ship_variants = {
       hoi4::EquipmentVariant("ship: variant_one", "", {}, {}, {}),
       hoi4::EquipmentVariant("ship: variant_two", "", {}, {"blocking_tech_missing"}, {}),
   };
   const std::vector<hoi4::EquipmentVariant> plane_variants = {
       hoi4::EquipmentVariant("plane: variant_one", "", {}, {}, {}),
       hoi4::EquipmentVariant("plane: variant_two", "", {}, {"blocking_tech_missing"}, {}),
   };
   const std::vector<hoi4::EquipmentVariant> tank_variants = {
       hoi4::EquipmentVariant("tank: variant_one", "", {}, {}, {}),
       hoi4::EquipmentVariant("tank: variant_two", "", {}, {"blocking_tech_missing"}, {}),
   };

   const hoi4::Country country({.tag = "TAG",
       .legacy_ship_variants = legacy_ship_variants,
       .ship_variants = ship_variants,
       .plane_variants = plane_variants,
       .tank_variants = tank_variants});
   OutputCountryHistory("EquipmentVariantsAreOutput", country, {});

   const path tag_path("output/EquipmentVariantsAreOutput/history/countries/TAG.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file(tag_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   const std::string expected_output =
       "if = {\n"
       "\tlimit = { not = { has_dlc = \"Man the Guns\" } }\n"
       "\tset_naval_oob = TAG_1936_Naval_Legacy\n"
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"legacy_ship: variant_one\"\n"
       "\t}\n"
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"legacy_ship: variant_two\"\n"
       "\t}\n"
       "}\n"
       "if = {\n"
       "\tlimit = { has_dlc = \"Man the Guns\" }\n"
       "\tset_naval_oob = TAG_1936_Naval\n"
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"ship: variant_one\"\n"
       "\t}\n"
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"ship: variant_two\"\n"
       "\t}\n"
       "}\n"
       "if = {\n"
       "\tlimit = { has_dlc = \"By Blood Alone\" }\n"
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"plane: variant_one\"\n"
       "\t}\n"
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"plane: variant_two\"\n"
       "\t}\n"
       "}\n"
       "if = {\n"
       "\tlimit = { has_dlc = \"No Step Back\" }\n"
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"tank: variant_one\"\n"
       "\t}\n"
       "\tcreate_equipment_variant = {\n"
       "\t\tname = \"tank: variant_two\"\n"
       "\t}\n"
       "}\n";

   EXPECT_THAT(country_file_stream.str(), testing::HasSubstr(expected_output));
}

TEST(Outhoi4CountriesOutcountryTests, OverlordsAreOutputToCountryHistoryFile)
{
   create_directories("output/OverlordsAreOutputToCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "BBB", .overlord = {"AAA"}});
   OutputCountryHistory("OverlordsAreOutputToCountryHistoryFile", country, {});

   const path bbb_path("output/OverlordsAreOutputToCountryHistoryFile/history/countries/BBB.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(bbb_path));
   std::ifstream country_file(bbb_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   const char* expected =
       "AAA = {\n"
       "\tif = {\n"
       "\t\tlimit = {\n"
       "\t\t\thas_dlc = \"Together for Victory\"\n"
       "\t\t}\n"
       "\t\tset_autonomy = {\n"
       "\t\t\ttarget = BBB\n"
       "\t\t\tautonomous_state = autonomy_puppet\n"
       "\t\t\tfreedom_level = 0.4\n"
       "\t\t}\n"
       "\t}\n"
       "\telse = {\n"
       "\t\tpuppet = BBB\n"
       "\t}\n"
       "}\n";

   EXPECT_THAT(country_file_stream.str(), testing::HasSubstr(expected));
}

TEST(Outhoi4CountriesOutcountryTests, FascistOverlordsAreOutputToCountryHistoryFile)
{
   create_directories("output/FascistOverlordsAreOutputToCountryHistoryFile/history/countries");

   const hoi4::Country country({.tag = "CCC", .ideology = "fascism", .overlord = {"FAC"}});
   OutputCountryHistory("FascistOverlordsAreOutputToCountryHistoryFile", country, {});

   const path ccc_path("output/FascistOverlordsAreOutputToCountryHistoryFile/history/countries/CCC.txt");
   ASSERT_TRUE(commonItems::DoesFileExist(ccc_path));
   std::ifstream country_file(ccc_path);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   const char* expected =
       "FAC = {\n"
       "\tif = {\n"
       "\t\tlimit = {\n"
       "\t\t\thas_dlc = \"Together for Victory\"\n"
       "\t\t}\n"
       "\t\tset_autonomy = {\n"
       "\t\t\ttarget = CCC\n"
       "\t\t\tautonomous_state = autonomy_integrated_puppet\n"
       "\t\t}\n"
       "\t}\n"
       "\telse = {\n"
       "\t\tset_autonomy = {\n"
       "\t\t\ttarget = CCC\n"
       "\t\t\tautonomous_state = autonomy_puppet\n"
       "\t\t}\n"
       "\t}\n"
       "}\n";

   EXPECT_THAT(country_file_stream.str(), testing::HasSubstr(expected));
}


TEST(Outhoi4CountriesOutcountryTests, UnitsAreOutputToCountryOOBFile)
{
   remove_all("output/UnitsAreOutputToCountryOOBFile/history/units");
   create_directories("output/UnitsAreOutputToCountryOOBFile/history/units");

   const path oob_file = "output/UnitsAreOutputToCountryOOBFile/history/units/TAG_1936.txt";
   ASSERT_TRUE(copy_file("configurables/division_templates.txt", oob_file));

   const hoi4::Country country(
       {.tag = "TAG", .units = {{.unit_template = "test", .equipment = 60, .location = 11666}}});
   OutputCountryUnits(oob_file, country);

   ASSERT_TRUE(commonItems::DoesFileExist(oob_file));
   std::ifstream country_file(oob_file);
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   const char* expected =
       "contents don't matter for tests\n\n"
       "units = {\n"
       "\tdivision = {\n"
       "\t\tdivision_name = {\n"
       "\t\t\tis_name_ordered = yes\n"
       "\t\t\tname_order = 1\n"
       "\t\t}\n"
       "\t\tlocation = 11666\n"
       "\t\tdivision_template = \"test\"\n"
       "\t\tstart_equipment_factor = 0.6\n"
       "\t\tstart_experience_factor = 0.2\n"
       "\t}\n"
       "}\n";

   EXPECT_THAT(country_file_stream.str(), testing::HasSubstr(expected));
}

TEST(Outhoi4CountriesOutcountryTests, NaviesAreOutputToCountryNavalFiles)
{
   create_directories("output/NaviesAreOutputToCountryNavalFiles/history/units");

   const hoi4::Country country({.tag = "TAG", .task_forces = {}});
   OutputCountryNavy("NaviesAreOutputToCountryNavalFiles", country);

   const path naval_file = "output/NaviesAreOutputToCountryNavalFiles/history/units/TAG_1936_Naval.txt";
   ASSERT_TRUE(commonItems::DoesFileExist(naval_file));
   std::ifstream navy(naval_file);
   ASSERT_TRUE(navy.is_open());
   std::stringstream navy_stream;
   std::copy(std::istreambuf_iterator<char>(navy),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(navy_stream));
   navy.close();
   const char* expected_navy = "Naval OOB\n";
   EXPECT_THAT(navy_stream.str(), testing::HasSubstr(expected_navy));

   const path legacy_file = "output/NaviesAreOutputToCountryNavalFiles/history/units/TAG_1936_Naval_Legacy.txt";
   ASSERT_TRUE(commonItems::DoesFileExist(legacy_file));
   std::ifstream legacy(legacy_file);
   ASSERT_TRUE(legacy.is_open());
   std::stringstream legacy_stream;
   std::copy(std::istreambuf_iterator<char>(legacy),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(legacy_stream));
   legacy.close();
   const char* expected_legacy = "Naval non-MTG OOB\n";
   EXPECT_THAT(legacy_stream.str(), testing::HasSubstr(expected_legacy));
}

TEST(Outhoi4CountriesOutcountryTests, ShipsAreOutputInBothFormats)
{
   create_directories("output/ShipsAreOutputInBothFormats/history/units");

   const hoi4::Country country({
       .tag = "TAG",
       .task_forces =
           {
               {
                   .name = "123 Fleet",
                   .ships =
                       {
                           hoi4::Ship("Test Ship",
                               "test_ship_type",
                               "mtg_equipment_template",
                               "legacy_equipment_template",
                               "Test Class"),
                       },
                   .location = 123,
               },
               {
                   .name = "456 Fleet",
                   .ships =
                       {
                           hoi4::Ship("Test Ship 2",
                               "another_ship_type",
                               "mtg_equipment_template_2",
                               "legacy_equipment_template_2",
                               "Another Class"),
                       },
                   .location = 456,
               },
           },
   });
   OutputCountryNavy("ShipsAreOutputInBothFormats", country);

   const path naval_file = "output/ShipsAreOutputInBothFormats/history/units/TAG_1936_Naval.txt";
   ASSERT_TRUE(commonItems::DoesFileExist(naval_file));
   std::ifstream navy(naval_file);
   ASSERT_TRUE(navy.is_open());
   std::stringstream navy_stream;
   std::copy(std::istreambuf_iterator<char>(navy),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(navy_stream));
   navy.close();
   const char* expected_navy =
       "Naval OOB\n"
       "units = {\n"
       "\tfleet = {\n"
       "\t\tname = \"123 Fleet\"\n"
       "\t\tnaval_base = 123\n"
       "\t\ttask_force = {\n"
       "\t\t\tname = \"123 Fleet\"\n"
       "\t\t\tlocation = 123\n"
       "\t\t\tship = {"
       " name = \"Test Ship\""
       " definition = test_ship_type"
       " equipment = { mtg_equipment_template = { "
       " amount = 1 "
       " owner = TAG "
       " version_name = \"Test Class\" } } }\n"
       "\t\t}\n"
       "\t}\n"
       "\tfleet = {\n"
       "\t\tname = \"456 Fleet\"\n"
       "\t\tnaval_base = 456\n"
       "\t\ttask_force = {\n"
       "\t\t\tname = \"456 Fleet\"\n"
       "\t\t\tlocation = 456\n"
       "\t\t\tship = {"
       " name = \"Test Ship 2\""
       " definition = another_ship_type"
       " equipment = { mtg_equipment_template_2 = { "
       " amount = 1 "
       " owner = TAG "
       " version_name = \"Another Class\" } } }\n"
       "\t\t}\n"
       "\t}\n"
       "}\n";

   EXPECT_THAT(navy_stream.str(), testing::HasSubstr(expected_navy));

   const path legacy_file = "output/ShipsAreOutputInBothFormats/history/units/TAG_1936_Naval_Legacy.txt";
   ASSERT_TRUE(commonItems::DoesFileExist(legacy_file));
   std::ifstream legacy(legacy_file);
   ASSERT_TRUE(legacy.is_open());
   std::stringstream legacy_stream;
   std::copy(std::istreambuf_iterator<char>(legacy),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(legacy_stream));
   legacy.close();
   const char* expected_legacy =
       "Naval non-MTG OOB\n"
       "units = {\n"
       "\tfleet = {\n"
       "\t\tname = \"123 Fleet\"\n"
       "\t\tnaval_base = 123\n"
       "\t\ttask_force = {\n"
       "\t\t\tname = \"123 Fleet\"\n"
       "\t\t\tlocation = 123\n"
       "\t\t\tship = {"
       " name = \"Test Ship\""
       " definition = test_ship_type"
       " equipment = { legacy_equipment_template = { "
       " amount = 1 "
       " owner = TAG "
       " version_name = \"Test Class\" } } }\n"
       "\t\t}\n"
       "\t}\n"
       "\tfleet = {\n"
       "\t\tname = \"456 Fleet\"\n"
       "\t\tnaval_base = 456\n"
       "\t\ttask_force = {\n"
       "\t\t\tname = \"456 Fleet\"\n"
       "\t\t\tlocation = 456\n"
       "\t\t\tship = {"
       " name = \"Test Ship 2\""
       " definition = another_ship_type"
       " equipment = { legacy_equipment_template_2 = { "
       " amount = 1 "
       " owner = TAG "
       " version_name = \"Another Class\" } } }\n"
       "\t\t}\n"
       "\t}\n"
       "}\n";
   EXPECT_THAT(legacy_stream.str(), testing::HasSubstr(expected_legacy));
}



}  // namespace out
