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

TEST(Outhoi4CountriesCountry, CountriesFileIsCreated)
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
   EXPECT_EQ(country_file_stream.str(), "color = rgb { 1 2 3 }");

   ASSERT_TRUE(commonItems::DoesFileExist("output/CountriesFileIsCreated/common/countries/TWO.txt"));
   std::ifstream country_file_two("output/CountriesFileIsCreated/common/countries/TWO.txt");
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();
   EXPECT_EQ(country_file_two_stream.str(), "color = rgb { 2 4 6 }");
}


TEST(Outhoi4CountriesCountry, ExceptionIfCountriesFileNotOpened)
{
   const hoi4::Country country({.tag = "TAG"});

   EXPECT_THROW(OutputCommonCountriesFile("/dev/null/COM", country), std::runtime_error);
}


TEST(Outhoi4CountriesCountry, TagIsAddedToTagsFile)
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


TEST(Outhoi4CountriesCountry, CountryHistoryFileIsCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CountryHistoryFileIsCreated");
   commonItems::TryCreateFolder("output/CountryHistoryFileIsCreated/history");
   commonItems::TryCreateFolder("output/CountryHistoryFileIsCreated/history/countries");

   const hoi4::Country country({.tag = "TAG"});
   const hoi4::Country country_two({.tag = "TWO"});

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
   EXPECT_EQ(country_file_stream.str(), "");

   ASSERT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFileIsCreated/history/countries/TWO.txt"));
   std::ifstream country_file_two("output/CountryHistoryFileIsCreated/history/countries/TWO.txt");
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();
   EXPECT_EQ(country_file_two_stream.str(), "");
}


TEST(Outhoi4CountriesCountry, ExceptionIfHistoryFileNotOpened)
{
   const hoi4::Country country({.tag = "TAG"});

   EXPECT_THROW(OutputCountryHistory("/dev/null/COM", country), std::runtime_error);
}

}  // namespace out