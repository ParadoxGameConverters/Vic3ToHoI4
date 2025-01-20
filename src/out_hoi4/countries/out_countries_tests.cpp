#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/countries/out_countries.h"



using std::filesystem::create_directories;
using std::filesystem::path;



namespace
{

void CreateFolders(std::string_view test_name)
{
   const path test_path(test_name);
   create_directories("output");
   create_directories("output" / test_path);
   create_directories("output" / test_path / "common");
   create_directories("output" / test_path / "common/characters");
   create_directories("output" / test_path / "common/countries");
   create_directories("output" / test_path / "common/country_tags");
   create_directories("output" / test_path / "common/national_focus");
   create_directories("output" / test_path / "history");
   create_directories("output" / test_path / "history/countries");
   create_directories("output" / test_path / "history/units");
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
       {},
       configuration::UseStories::kNo);

   const path tag_path("output/CountriesFilesAreCreated/common/countries/TAG.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file_one(tag_path);
   ASSERT_TRUE(country_file_one.is_open());
   std::stringstream country_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_one_stream));
   country_file_one.close();

   const path two_path("output/CountriesFilesAreCreated/common/countries/TWO.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(two_path));
   std::ifstream country_file_two(two_path);
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
       {},
       configuration::UseStories::kNo);

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
                    {},
                    configuration::UseStories::kNo),
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
       {},
       configuration::UseStories::kNo);

   const path tag_path("output/CharactersFilesAreCreated/common/characters/TAG.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream character_file_one(tag_path);
   ASSERT_TRUE(character_file_one.is_open());
   std::stringstream character_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(character_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(character_file_one_stream));
   character_file_one.close();

   const path two_path("output/CharactersFilesAreCreated/common/characters/TAG.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(two_path));
   std::ifstream character_file_two(two_path);
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
       {},
       configuration::UseStories::kNo);

   const path tag_path("output/CountryHistoryFilesAreCreated/history/countries/TAG.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file_one(tag_path);
   ASSERT_TRUE(country_file_one.is_open());
   std::stringstream country_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_one_stream));
   country_file_one.close();

   const path two_path("output/CountryHistoryFilesAreCreated/history/countries/TWO.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(two_path));
   std::ifstream country_file_two(two_path);
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
       {},
       configuration::UseStories::kNo);

   EXPECT_TRUE(commonItems::DoesFileExist(path("output/DivisionTemplatesAreCopied/history/units/TAG_1936.txt")));
   EXPECT_TRUE(commonItems::DoesFileExist(path("output/DivisionTemplatesAreCopied/history/units/TWO_1936.txt")));
}


TEST(Outhoi4CountriesOutcountriesTests, NationalFocusFilesAreCreated)
{
   CreateFolders("NationalFocusFilesAreCreated");

   OutputCountries("NationalFocusFilesAreCreated",
       {
           {"TAG", hoi4::Country({.tag = "TAG"})},
           {"TWO", hoi4::Country({.tag = "TWO"})},
       },
       {},
       configuration::UseStories::kNo);

   const path tag_path("output/NationalFocusFilesAreCreated/common/national_focus/TAG_NF.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file_one(tag_path);
   ASSERT_TRUE(country_file_one.is_open());
   std::stringstream country_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_one_stream));
   country_file_one.close();

   const path two_path("output/NationalFocusFilesAreCreated/common/national_focus/TWO_NF.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(two_path));
   std::ifstream country_file_two(two_path);
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
       {},
       configuration::UseStories::kNo);

   const path tag_path("output/NavyFilesAreCreated/history/units/TAG_1936_Naval.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(tag_path));
   std::ifstream country_file_one(tag_path);
   ASSERT_TRUE(country_file_one.is_open());
   std::stringstream country_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_one_stream));
   country_file_one.close();

   const path tag_legacy_path("output/NavyFilesAreCreated/history/units/TAG_1936_Naval_Legacy.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(tag_legacy_path));
   std::ifstream legacy_file_one(tag_legacy_path);
   ASSERT_TRUE(legacy_file_one.is_open());
   std::stringstream legacy_file_one_stream;
   std::copy(std::istreambuf_iterator<char>(legacy_file_one),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(legacy_file_one_stream));
   legacy_file_one.close();

   const path two_path("output/NavyFilesAreCreated/history/units/TWO_1936_Naval.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(two_path));
   std::ifstream country_file_two(two_path);
   ASSERT_TRUE(country_file_two.is_open());
   std::stringstream country_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(country_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_two_stream));
   country_file_two.close();

   const path two_legacy_path("output/NavyFilesAreCreated/history/units/TWO_1936_Naval.txt");
   EXPECT_TRUE(commonItems::DoesFileExist(two_legacy_path));
   std::ifstream legacy_file_two(two_legacy_path);
   ASSERT_TRUE(legacy_file_two.is_open());
   std::stringstream legacy_file_two_stream;
   std::copy(std::istreambuf_iterator<char>(legacy_file_two),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(legacy_file_two_stream));
   legacy_file_two.close();
}

}  // namespace out
