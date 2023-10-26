#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "external/fmt/include/fmt/format.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/countries/out_countries.h"



namespace
{

void CreateFolders(std::string_view test_name)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder(fmt::format("output/{}", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/characters", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/countries", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/country_tags", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/national_focus", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/countries", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/units", test_name));
}

}  // namespace



namespace out
{

TEST(Outhoi4CountriesOutcountriesTests, CountriesFilesAreCreated)
{
   CreateFolders("CountriesFilesAreCreated");

   OutputCountries("CountriesFilesAreCreated",
       {
           {"TAG", hoi4::Country({.tag = "TAG"})},
           {"TWO", hoi4::Country({.tag = "TWO"})},
       },
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
   CreateFolders("TagsFileIsCreated");

   OutputCountries("TagsFileIsCreated",
       {
           {"TAG", hoi4::Country({.tag = "TAG"})},
           {"TWO", hoi4::Country({.tag = "TWO"})},
       },
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
                    {
                        {"TAG", hoi4::Country({.tag = "TAG"})},
                        {"TWO", hoi4::Country({.tag = "TWO"})},
                    },
                    {}),
       std::runtime_error);
}


TEST(Outhoi4CountriesOutcountriesTests, CharactersFilesAreCreated)
{
   CreateFolders("CharactersFilesAreCreated");

   OutputCountries("CharactersFilesAreCreated",
       {
           {"TAG", hoi4::Country({.tag = "TAG"})},
           {"TWO", hoi4::Country({.tag = "TWO"})},
       },
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
   CreateFolders("CountryHistoryFilesAreCreated");

   OutputCountries("CountryHistoryFilesAreCreated",
       {
           {"TAG", hoi4::Country({.tag = "TAG"})},
           {"TWO", hoi4::Country({.tag = "TWO"})},
       },
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
   CreateFolders("DivisionTemplatesAreCopied");

   OutputCountries("DivisionTemplatesAreCopied",
       {
           {"TAG", hoi4::Country({.tag = "TAG"})},
           {"TWO", hoi4::Country({.tag = "TWO"})},
       },
       {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/DivisionTemplatesAreCopied/history/units/TAG_1936.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/DivisionTemplatesAreCopied/history/units/TWO_1936.txt"));
}


TEST(Outhoi4CountriesOutcountriesTests, NationalFocusFilesAreCreated)
{
   CreateFolders("NationalFocusFilesAreCreated");

   OutputCountries("NationalFocusFilesAreCreated",
       {
           {"TAG", hoi4::Country({.tag = "TAG"})},
           {"TWO", hoi4::Country({.tag = "TWO"})},
       },
       {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/NationalFocusFilesAreCreated/common/national_focus/TAG_NF.txt"));
   std::ifstream country_file_one("output/NationalFocusFilesAreCreated/common/national_focus/TAG_NF.txt");
   ASSERT_TRUE(country_file_one.is_open());
   std::stringstream country_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_one_stream));
   country_file_one.close();

   EXPECT_TRUE(commonItems::DoesFileExist("output/NationalFocusFilesAreCreated/common/national_focus/TWO_NF.txt"));
   std::ifstream country_file_two("output/NationalFocusFilesAreCreated/common/national_focus/TWO_NF.txt");
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();
}


TEST(Outhoi4CountriesOutcountriesTests, NavyFilesAreCreated)
{
   CreateFolders("NavyFilesAreCreated");

   OutputCountries("NavyFilesAreCreated",
       {
           {"TAG", hoi4::Country({.tag = "TAG"})},
           {"TWO", hoi4::Country({.tag = "TWO"})},
       },
       {});

   EXPECT_TRUE(commonItems::DoesFileExist("output/NavyFilesAreCreated/history/units/TAG_1936_Naval.txt"));
   std::ifstream country_file_one("output/NavyFilesAreCreated/history/units/TAG_1936_Naval.txt");
   ASSERT_TRUE(country_file_one.is_open());
   std::stringstream country_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_one_stream));
   country_file_one.close();

   EXPECT_TRUE(commonItems::DoesFileExist("output/NavyFilesAreCreated/history/units/TAG_1936_Naval_Legacy.txt"));
   std::ifstream legacy_file_one("output/NavyFilesAreCreated/history/units/TAG_1936_Naval_Legacy.txt");
   ASSERT_TRUE(legacy_file_one.is_open());
   std::stringstream legacy_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(legacy_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(legacy_file_one_stream));
   legacy_file_one.close();

   EXPECT_TRUE(commonItems::DoesFileExist("output/NavyFilesAreCreated/history/units/TWO_1936_Naval.txt"));
   std::ifstream country_file_two("output/NavyFilesAreCreated/history/units/TWO_1936_Naval.txt");
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();

   EXPECT_TRUE(commonItems::DoesFileExist("output/NavyFilesAreCreated/history/units/TWO_1936_Naval_Legacy.txt"));
   std::ifstream legacy_file_two("output/NavyFilesAreCreated/history/units/TWO_1936_Naval_Legacy.txt");
   ASSERT_TRUE(legacy_file_two.is_open());
   std::stringstream legacy_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(legacy_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(legacy_file_two_stream));
   legacy_file_two.close();
}

}  // namespace out
