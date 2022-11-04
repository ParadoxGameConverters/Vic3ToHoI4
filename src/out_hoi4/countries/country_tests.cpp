#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/country.h"
#include "src/out_hoi4/countries/country.h"



namespace out
{

TEST(Outhoi4CountriesCountry, CountriesFileIsCreated)
{
   const hoi4::Country country("TAG");
   const hoi4::Country country_two("TWO");

   OutputCommonCountriesFile("Outhoi4CountriesCountry/CountriesFileIsCreated", country);
   OutputCommonCountriesFile("Outhoi4CountriesCountry/CountriesFileIsCreated", country_two);

   ASSERT_TRUE(commonItems::DoesFileExist("Outhoi4CountriesCountry/CountriesFileIsCreated/TAG.txt"));
   std::ifstream country_file("Outhoi4CountriesCountry/CountriesFileIsCreated/TAG.txt");
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();
   EXPECT_EQ(country_file_stream.str(), "");

   ASSERT_TRUE(commonItems::DoesFileExist("Outhoi4CountriesCountry/CountriesFileIsCreated/TWO.txt"));
   std::ifstream country_file_two("Outhoi4CountriesCountry/CountriesFileIsCreated/TWO.txt");
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();
   EXPECT_EQ(country_file_two_stream.str(), "");
}


TEST(Outhoi4CountriesCountry, TagIsAddedToTagsFile)
{
   const hoi4::Country country("TAG");
   const hoi4::Country country_two("TWO");

   std::ofstream tags_file("Outhoi4CountriesCountry/TagIsAddedToTagsFile/tags_file.txt");
   ASSERT_TRUE(tags_file.is_open());

   OutputCommonCountryTag(country, tags_file);
   OutputCommonCountryTag(country_two, tags_file);

   tags_file.close();

   std::ifstream country_file("Outhoi4CountriesCountry/TagIsAddedToTagsFile/tags_file.txt");
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
    const hoi4::Country country("TAG");
    const hoi4::Country country_two("TWO");

    OutputCommonCountriesFile("Outhoi4CountriesCountry/CountryHistoryFileIsCreated", country);
    OutputCommonCountriesFile("Outhoi4CountriesCountry/CountryHistoryFileIsCreated", country_two);

    ASSERT_TRUE(commonItems::DoesFileExist("Outhoi4CountriesCountry/CountryHistoryFileIsCreated/TAG.txt"));
    std::ifstream country_file("Outhoi4CountriesCountry/CountryHistoryFileIsCreated/TAG.txt");
    ASSERT_TRUE(country_file.is_open());
    std::stringstream country_file_stream;
    std::copy(std::istreambuf_iterator<char>(country_file),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(country_file_stream));
    country_file.close();
    EXPECT_EQ(country_file_stream.str(), "");

    ASSERT_TRUE(commonItems::DoesFileExist("Outhoi4CountriesCountry/CountryHistoryFileIsCreated/TWO.txt"));
    std::ifstream country_file_two("Outhoi4CountriesCountry/CountryHistoryFileIsCreated/TWO.txt");
    ASSERT_TRUE(country_file_two.is_open());
    std::stringstream country_file_two_stream;
    std::copy(std::istreambuf_iterator<char>(country_file_two),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(country_file_two_stream));
    country_file_two.close();
    EXPECT_EQ(country_file_two_stream.str(), "");
}

}  // namespace out