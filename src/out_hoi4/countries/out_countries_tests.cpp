#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/countries/out_countries.h"



namespace out
{

TEST(Outhoi4CountriesOutcountriesTests, CountriesFilesAreCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/common");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/common/characters");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/common/countries");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/common/country_tags");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/history");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/history/countries");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/history/units");

   OutputCountries("CountriesFilesAreCreated",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
       {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/CountriesFilesAreCreated/common/countries/TAG.txt"));
   std::ifstream country_file_one("output/CountriesFilesAreCreated/common/countries/TAG.txt");
   ASSERT_TRUE(country_file_one.is_open());
   std::stringstream country_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_one_stream));
   country_file_one.close();

   EXPECT_TRUE(commonItems::DoesFileExist("output/CountriesFilesAreCreated/common/countries/TWO.txt"));
   std::ifstream country_file_two("output/CountriesFilesAreCreated/common/countries/TWO.txt");
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();
}


TEST(Outhoi4CountriesOutcountriesTests, TagsFileIsCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/TagsFileIsCreated");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/common");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/common/characters");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/common/countries");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/common/country_tags");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/history");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/history/countries");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/history/units");

   OutputCountries("TagsFileIsCreated",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
       {});

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


TEST(Outhoi4CountriesOutcountriesTests, ExceptionIfTagsFileNotOpened)
{
   EXPECT_THROW(OutputCountries("/dev/null/COM",
                    {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
                    {}),
       std::runtime_error);
}


TEST(Outhoi4CountriesOutcountriesTests, CharactersFilesAreCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CharactersFilesAreCreated");
   commonItems::TryCreateFolder("output/CharactersFilesAreCreated/common");
   commonItems::TryCreateFolder("output/CharactersFilesAreCreated/common/characters");
   commonItems::TryCreateFolder("output/CharactersFilesAreCreated/common/countries");
   commonItems::TryCreateFolder("output/CharactersFilesAreCreated/common/country_tags");
   commonItems::TryCreateFolder("output/CharactersFilesAreCreated/history");
   commonItems::TryCreateFolder("output/CharactersFilesAreCreated/history/countries");
   commonItems::TryCreateFolder("output/CharactersFilesAreCreated/history/units");

   OutputCountries("CharactersFilesAreCreated",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
       {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/CharactersFilesAreCreated/common/characters/TAG.txt"));
   std::ifstream character_file_one("output/CharactersFilesAreCreated/common/characters/TAG.txt");
   ASSERT_TRUE(character_file_one.is_open());
   std::stringstream character_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(character_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(character_file_one_stream));
   character_file_one.close();

   EXPECT_TRUE(commonItems::DoesFileExist("output/CharactersFilesAreCreated/common/countries/TWO.txt"));
   std::ifstream character_file_two("output/CharactersFilesAreCreated/common/countries/TWO.txt");
   ASSERT_TRUE(character_file_two.is_open());
   std::stringstream character_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(character_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(character_file_two_stream));
   character_file_two.close();
}


TEST(Outhoi4CountriesOutcountriesTests, CountryHistoryFilesAreCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/common");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/common/characters");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/common/countries");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/common/country_tags");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/history");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/history/countries");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/history/units");

   OutputCountries("CountryHistoryFilesAreCreated",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
       {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFilesAreCreated/history/countries/TAG.txt"));
   std::ifstream country_file_one("output/CountryHistoryFilesAreCreated/history/countries/TAG.txt");
   ASSERT_TRUE(country_file_one.is_open());
   std::stringstream country_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_one_stream));
   country_file_one.close();

   EXPECT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFilesAreCreated/history/countries/TWO.txt"));
   std::ifstream country_file_two("output/CountryHistoryFilesAreCreated/history/countries/TWO.txt");
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();
}


TEST(Outhoi4CountriesOutcountriesTests, DivisionTemplatesAreCopied)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/DivisionTemplatesAreCopied");
   commonItems::TryCreateFolder("output/DivisionTemplatesAreCopied/common");
   commonItems::TryCreateFolder("output/DivisionTemplatesAreCopied/common/countries");
   commonItems::TryCreateFolder("output/DivisionTemplatesAreCopied/common/characters");
   commonItems::TryCreateFolder("output/DivisionTemplatesAreCopied/common/country_tags");
   commonItems::TryCreateFolder("output/DivisionTemplatesAreCopied/history");
   commonItems::TryCreateFolder("output/DivisionTemplatesAreCopied/history/countries");
   commonItems::TryCreateFolder("output/DivisionTemplatesAreCopied/history/units");

   OutputCountries("DivisionTemplatesAreCopied",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
       {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/DivisionTemplatesAreCopied/history/units/TAG_1936.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/DivisionTemplatesAreCopied/history/units/TWO_1936.txt"));
}

}  // namespace out