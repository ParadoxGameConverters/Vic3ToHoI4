#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/countries/out_country.h"



namespace out
{

TEST(Outhoi4CountriesOutcountry, CommonCountriesFileIsCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CountriesFileIsCreated");
   commonItems::TryCreateFolder("output/CountriesFileIsCreated/common");
   commonItems::TryCreateFolder("output/CountriesFileIsCreated/common/countries");

   const hoi4::Country country({.tag = "TAG", .color = commonItems::Color(std::array{1, 2, 3})});
   const hoi4::Country country_two({.tag = "TWO", .color = commonItems::Color(std::array{2, 4, 6})});

   OutputCommonCountriesFile("CountriesFileIsCreated", country);
   OutputCommonCountriesFile("CountriesFileIsCreated", country_two);

   ASSERT_TRUE(commonItems::DoesFileExist("output/CountriesFileIsCreated/common/countries/TAG.txt"));
   std::ifstream country_file("output/CountriesFileIsCreated/common/countries/TAG.txt");
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

   ASSERT_TRUE(commonItems::DoesFileExist("output/CountriesFileIsCreated/common/countries/TWO.txt"));
   std::ifstream country_file_two("output/CountriesFileIsCreated/common/countries/TWO.txt");
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


TEST(Outhoi4CountriesOutcountry, ExceptionIfCountriesFileNotOpened)
{
   const hoi4::Country country({.tag = "TAG"});

   EXPECT_THROW(OutputCommonCountriesFile("/dev/null/COM", country), std::runtime_error);
}


TEST(Outhoi4CountriesOutcountry, TagIsAddedToTagsFile)
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


TEST(Outhoi4CountriesOutcountry, CountryHistoryFileIsCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CountryHistoryFileIsCreated");
   commonItems::TryCreateFolder("output/CountryHistoryFileIsCreated/history");
   commonItems::TryCreateFolder("output/CountryHistoryFileIsCreated/history/countries");

   const hoi4::Country country({.tag = "TAG"});  // no capital
   const hoi4::Country country_two({.tag = "TWO", .capital_state = 2});

   OutputCountryHistory("CountryHistoryFileIsCreated", country);
   OutputCountryHistory("CountryHistoryFileIsCreated", country_two);

   ASSERT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFileIsCreated/history/countries/TAG.txt"));
   std::ifstream country_file("output/CountryHistoryFileIsCreated/history/countries/TAG.txt");
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();

   std::stringstream expected_one;
   expected_one << "set_research_slots = 3\n";
   expected_one << "set_convoys = 0\n";
   expected_one << "\n";
   expected_one << "set_politics = {\n";
   expected_one << "\truling_party = neutrality\n";
   expected_one << "\tlast_election = \"1836.1.1\"\n";
   expected_one << "election_frequency = 48\n";
   expected_one << "elections_allowed = no\n";
   expected_one << "}\n";
   expected_one << "\n";
   expected_one << "set_popularities = {\n";
   expected_one << "\tneutrality = 100\n";
   expected_one << "}\n";
   expected_one << "\n";
   expected_one << "add_ideas = {\n";
   expected_one << "\tlimited_conscription\n";
   expected_one << "\tcivilian_economy\n";
   expected_one << "\texport_focus\n";
   expected_one << "}\n";
   expected_one << "set_stability = 0.60\n";
   expected_one << "set_war_support = 0.60\n";
   EXPECT_EQ(country_file_stream.str(), expected_one.str());

   ASSERT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFileIsCreated/history/countries/TWO.txt"));
   std::ifstream country_file_two("output/CountryHistoryFileIsCreated/history/countries/TWO.txt");
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();

   std::stringstream expected_two;
   expected_two << "capital = 2\n";
   expected_two << "set_research_slots = 3\n";
   expected_two << "set_convoys = 0\n";
   expected_two << "\n";
   expected_two << "set_politics = {\n";
   expected_two << "\truling_party = neutrality\n";
   expected_two << "\tlast_election = \"1836.1.1\"\n";
   expected_two << "election_frequency = 48\n";
   expected_two << "elections_allowed = no\n";
   expected_two << "}\n";
   expected_two << "\n";
   expected_two << "set_popularities = {\n";
   expected_two << "\tneutrality = 100\n";
   expected_two << "}\n";
   expected_two << "\n";
   expected_two << "add_ideas = {\n";
   expected_two << "\tlimited_conscription\n";
   expected_two << "\tcivilian_economy\n";
   expected_two << "\texport_focus\n";
   expected_two << "}\n";
   expected_two << "set_stability = 0.60\n";
   expected_two << "set_war_support = 0.60\n";
   EXPECT_EQ(country_file_two_stream.str(), expected_two.str());
}


TEST(Outhoi4CountriesOutcountry, ExceptionIfHistoryFileNotOpened)
{
   const hoi4::Country country({.tag = "TAG"});

   EXPECT_THROW(OutputCountryHistory("/dev/null/COM", country), std::runtime_error);
}

}  // namespace out