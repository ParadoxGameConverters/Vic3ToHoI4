#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/out_hoi4/localizations/out_localizations.h"



using std::filesystem::create_directories;
using std::filesystem::path;
using std::filesystem::remove_all;



namespace out
{

TEST(Outhoi4LocalizationsOutlocalizationsTests, FoldersAreCreated)
{
   remove_all("output/Outhoi4Localizations/FoldersAreCreated/");
   create_directories("output/Outhoi4Localizations/FoldersAreCreated/");

   const hoi4::Localizations localizations;

   OutputLocalizations("Outhoi4Localizations/FoldersAreCreated", localizations);

   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation/braz_por"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation/english"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation/french"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation/german"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation/japanese"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation/polish"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation/russian"));
   EXPECT_TRUE(commonItems::DoesFolderExist("output/Outhoi4Localizations/FoldersAreCreated/localisation/spanish"));
}


TEST(Outhoi4LocalizationsOutlocalizationsTests, CountryLocalizationsAreOutput)
{
   remove_all("output/Outhoi4Localizations/CountryLocalizationsAreOutput/");
   create_directories("output/Outhoi4Localizations/CountryLocalizationsAreOutput/");

   commonItems::LocalizationDatabase country_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("test_country", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   country_localizations.AddOrModifyLocalizationBlock("test_country", block_one);
   commonItems::LocalizationBlock block_two("test_country_two", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   country_localizations.AddOrModifyLocalizationBlock("test_country_two", block_two);

   const commonItems::LocalizationDatabase state_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase victory_point_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase character_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase idea_localizations("english", {"spanish"});

   const hoi4::Localizations localizations(country_localizations,
       state_localizations,
       victory_point_localizations,
       character_localizations,
       idea_localizations);

   OutputLocalizations("Outhoi4Localizations/CountryLocalizationsAreOutput", localizations);

   path portuguese_path(
       "output/Outhoi4Localizations/CountryLocalizationsAreOutput/localisation/braz_por/countries_l_braz_por.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(portuguese_path));
   std::ifstream braz_por_file(portuguese_path);
   ASSERT_TRUE(braz_por_file.is_open());
   std::stringstream braz_por_file_stream;
   std::copy(std::istreambuf_iterator<char>(braz_por_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(braz_por_file_stream));
   braz_por_file.close();
   EXPECT_EQ(braz_por_file_stream.str(),
       "﻿l_braz_por:\n"
       " test_country:0 \"test\"\n"
       " test_country_two:0 \"test two\"\n");

   path english_path(
       "output/Outhoi4Localizations/CountryLocalizationsAreOutput/localisation/english/countries_l_english.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(english_path));
   std::ifstream english_file(english_path);
   ASSERT_TRUE(english_file.is_open());
   std::stringstream english_file_stream;
   std::copy(std::istreambuf_iterator<char>(english_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(english_file_stream));
   english_file.close();
   EXPECT_EQ(english_file_stream.str(),
       "﻿l_english:\n"
       " test_country:0 \"test\"\n"
       " test_country_two:0 \"test two\"\n");

   path french_path(
       "output/Outhoi4Localizations/CountryLocalizationsAreOutput/localisation/french/countries_l_french.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(french_path));
   std::ifstream french_file(french_path);
   ASSERT_TRUE(french_file.is_open());
   std::stringstream french_file_stream;
   std::copy(std::istreambuf_iterator<char>(french_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(french_file_stream));
   french_file.close();
   EXPECT_EQ(french_file_stream.str(),
       "﻿l_french:\n"
       " test_country:0 \"test\"\n"
       " test_country_two:0 \"test two\"\n");

   path german_path(
       "output/Outhoi4Localizations/CountryLocalizationsAreOutput/localisation/german/countries_l_german.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(german_path));
   std::ifstream german_file(german_path);
   ASSERT_TRUE(german_file.is_open());
   std::stringstream german_file_stream;
   std::copy(std::istreambuf_iterator<char>(german_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(german_file_stream));
   german_file.close();
   EXPECT_EQ(german_file_stream.str(),
       "﻿l_german:\n"
       " test_country:0 \"test\"\n"
       " test_country_two:0 \"test two\"\n");

   path japanese_path(
       "output/Outhoi4Localizations/CountryLocalizationsAreOutput/localisation/japanese/countries_l_japanese.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(japanese_path));
   std::ifstream japanese_file(japanese_path);
   ASSERT_TRUE(japanese_file.is_open());
   std::stringstream japanese_file_stream;
   std::copy(std::istreambuf_iterator<char>(japanese_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(japanese_file_stream));
   japanese_file.close();
   EXPECT_EQ(japanese_file_stream.str(),
       "﻿l_japanese:\n"
       " test_country:0 \"test\"\n"
       " test_country_two:0 \"test two\"\n");

   path polish_path(
       "output/Outhoi4Localizations/CountryLocalizationsAreOutput/localisation/polish/countries_l_polish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(polish_path));
   std::ifstream polish_file(polish_path);
   ASSERT_TRUE(polish_file.is_open());
   std::stringstream polish_file_stream;
   std::copy(std::istreambuf_iterator<char>(polish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(polish_file_stream));
   polish_file.close();
   EXPECT_EQ(polish_file_stream.str(),
       "﻿l_polish:\n"
       " test_country:0 \"test\"\n"
       " test_country_two:0 \"test two\"\n");

   path russian_path(
       "output/Outhoi4Localizations/CountryLocalizationsAreOutput/localisation/russian/countries_l_russian.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(russian_path));
   std::ifstream russian_file(russian_path);
   ASSERT_TRUE(russian_file.is_open());
   std::stringstream russian_file_stream;
   std::copy(std::istreambuf_iterator<char>(russian_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(russian_file_stream));
   russian_file.close();
   EXPECT_EQ(russian_file_stream.str(),
       "﻿l_russian:\n"
       " test_country:0 \"test\"\n"
       " test_country_two:0 \"test two\"\n");

   path spanish_path(
       "output/Outhoi4Localizations/CountryLocalizationsAreOutput/localisation/spanish/countries_l_spanish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(spanish_path));
   std::ifstream spanish_file(spanish_path);
   ASSERT_TRUE(spanish_file.is_open());
   std::stringstream spanish_file_stream;
   std::copy(std::istreambuf_iterator<char>(spanish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(spanish_file_stream));
   spanish_file.close();
   EXPECT_EQ(spanish_file_stream.str(),
       "﻿l_spanish:\n"
       " test_country:0 \"prueba\"\n"
       " test_country_two:0 \"prueba dos\"\n");
}


TEST(Outhoi4LocalizationsOutlocalizationsTests, StateLocalizationsAreOutput)
{
   remove_all("output/Outhoi4Localizations/StateLocalizationsAreOutput/");
   create_directories("output/Outhoi4Localizations/StateLocalizationsAreOutput/");

   const commonItems::LocalizationDatabase country_localizations("english", {"spanish"});

   commonItems::LocalizationDatabase state_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("test_state", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   state_localizations.AddOrModifyLocalizationBlock("test_state", block_one);
   commonItems::LocalizationBlock block_two("test_state_two", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   state_localizations.AddOrModifyLocalizationBlock("test_state_two", block_two);

   const commonItems::LocalizationDatabase victory_point_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase character_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase idea_localizations("english", {"spanish"});

   const hoi4::Localizations localizations(country_localizations,
       state_localizations,
       victory_point_localizations,
       character_localizations,
       idea_localizations);

   OutputLocalizations("Outhoi4Localizations/StateLocalizationsAreOutput", localizations);

   path portuguese_path(
       "output/Outhoi4Localizations/StateLocalizationsAreOutput/localisation/braz_por/state_names_l_braz_por.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(portuguese_path));
   std::ifstream braz_por_file(portuguese_path);
   ASSERT_TRUE(braz_por_file.is_open());
   std::stringstream braz_por_file_stream;
   std::copy(std::istreambuf_iterator<char>(braz_por_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(braz_por_file_stream));
   braz_por_file.close();
   EXPECT_EQ(braz_por_file_stream.str(),
       "﻿l_braz_por:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path english_path(
       "output/Outhoi4Localizations/StateLocalizationsAreOutput/localisation/english/state_names_l_english.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(english_path));
   std::ifstream english_file(english_path);
   ASSERT_TRUE(english_file.is_open());
   std::stringstream english_file_stream;
   std::copy(std::istreambuf_iterator<char>(english_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(english_file_stream));
   english_file.close();
   EXPECT_EQ(english_file_stream.str(),
       "﻿l_english:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path french_path(
       "output/Outhoi4Localizations/StateLocalizationsAreOutput/localisation/french/state_names_l_french.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(french_path));
   std::ifstream french_file(french_path);
   ASSERT_TRUE(french_file.is_open());
   std::stringstream french_file_stream;
   std::copy(std::istreambuf_iterator<char>(french_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(french_file_stream));
   french_file.close();
   EXPECT_EQ(french_file_stream.str(),
       "﻿l_french:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path german_path(
       "output/Outhoi4Localizations/StateLocalizationsAreOutput/localisation/german/state_names_l_german.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(german_path));
   std::ifstream german_file(german_path);
   ASSERT_TRUE(german_file.is_open());
   std::stringstream german_file_stream;
   std::copy(std::istreambuf_iterator<char>(german_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(german_file_stream));
   german_file.close();
   EXPECT_EQ(german_file_stream.str(),
       "﻿l_german:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path japanese_path(
       "output/Outhoi4Localizations/StateLocalizationsAreOutput/localisation/japanese/state_names_l_japanese.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(japanese_path));
   std::ifstream japanese_file(japanese_path);
   ASSERT_TRUE(japanese_file.is_open());
   std::stringstream japanese_file_stream;
   std::copy(std::istreambuf_iterator<char>(japanese_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(japanese_file_stream));
   japanese_file.close();
   EXPECT_EQ(japanese_file_stream.str(),
       "﻿l_japanese:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path polish_path(
       "output/Outhoi4Localizations/StateLocalizationsAreOutput/localisation/polish/state_names_l_polish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(polish_path));
   std::ifstream polish_file(polish_path);
   ASSERT_TRUE(polish_file.is_open());
   std::stringstream polish_file_stream;
   std::copy(std::istreambuf_iterator<char>(polish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(polish_file_stream));
   polish_file.close();
   EXPECT_EQ(polish_file_stream.str(),
       "﻿l_polish:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path russian_path(
       "output/Outhoi4Localizations/StateLocalizationsAreOutput/localisation/russian/state_names_l_russian.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(russian_path));
   std::ifstream russian_file(russian_path);
   ASSERT_TRUE(russian_file.is_open());
   std::stringstream russian_file_stream;
   std::copy(std::istreambuf_iterator<char>(russian_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(russian_file_stream));
   russian_file.close();
   EXPECT_EQ(russian_file_stream.str(),
       "﻿l_russian:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path spanish_path(
       "output/Outhoi4Localizations/StateLocalizationsAreOutput/localisation/spanish/state_names_l_spanish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(spanish_path));
   std::ifstream spanish_file(spanish_path);
   ASSERT_TRUE(spanish_file.is_open());
   std::stringstream spanish_file_stream;
   std::copy(std::istreambuf_iterator<char>(spanish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(spanish_file_stream));
   spanish_file.close();
   EXPECT_EQ(spanish_file_stream.str(),
       "﻿l_spanish:\n"
       " test_state:0 \"prueba\"\n"
       " test_state_two:0 \"prueba dos\"\n");
}


TEST(Outhoi4LocalizationsOutlocalizationsTests, VictoryPointLocalizationsAreOutput)
{
   remove_all("output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/");
   create_directories("output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/");

   const commonItems::LocalizationDatabase country_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase state_localizations("english", {"spanish"});

   commonItems::LocalizationDatabase victory_point_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("test_state", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   victory_point_localizations.AddOrModifyLocalizationBlock("test_state", block_one);
   commonItems::LocalizationBlock block_two("test_state_two", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   victory_point_localizations.AddOrModifyLocalizationBlock("test_state_two", block_two);

   const commonItems::LocalizationDatabase character_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase idea_localizations("english", {"spanish"});

   const hoi4::Localizations localizations(country_localizations,
       state_localizations,
       victory_point_localizations,
       character_localizations,
       idea_localizations);

   OutputLocalizations("Outhoi4Localizations/VictoryPointLocalizationsAreOutput", localizations);

   path portuguese_path(
       "output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/localisation/braz_por/"
       "victory_points_l_braz_por.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(portuguese_path));
   std::ifstream braz_por_file(portuguese_path);
   ASSERT_TRUE(braz_por_file.is_open());
   std::stringstream braz_por_file_stream;
   std::copy(std::istreambuf_iterator<char>(braz_por_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(braz_por_file_stream));
   braz_por_file.close();
   EXPECT_EQ(braz_por_file_stream.str(),
       "﻿l_braz_por:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path english_path(
       "output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/localisation/english/"
       "victory_points_l_english.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(english_path));
   std::ifstream english_file(english_path);
   ASSERT_TRUE(english_file.is_open());
   std::stringstream english_file_stream;
   std::copy(std::istreambuf_iterator<char>(english_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(english_file_stream));
   english_file.close();
   EXPECT_EQ(english_file_stream.str(),
       "﻿l_english:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path french_path(
       "output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/localisation/french/"
       "victory_points_l_french.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(french_path));
   std::ifstream french_file(french_path);
   ASSERT_TRUE(french_file.is_open());
   std::stringstream french_file_stream;
   std::copy(std::istreambuf_iterator<char>(french_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(french_file_stream));
   french_file.close();
   EXPECT_EQ(french_file_stream.str(),
       "﻿l_french:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path german_path(
       "output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/localisation/german/"
       "victory_points_l_german.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(german_path));
   std::ifstream german_file(german_path);
   ASSERT_TRUE(german_file.is_open());
   std::stringstream german_file_stream;
   std::copy(std::istreambuf_iterator<char>(german_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(german_file_stream));
   german_file.close();
   EXPECT_EQ(german_file_stream.str(),
       "﻿l_german:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path japanese_path(
       "output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/localisation/japanese/"
       "victory_points_l_japanese.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(japanese_path));
   std::ifstream japanese_file(japanese_path);
   ASSERT_TRUE(japanese_file.is_open());
   std::stringstream japanese_file_stream;
   std::copy(std::istreambuf_iterator<char>(japanese_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(japanese_file_stream));
   japanese_file.close();
   EXPECT_EQ(japanese_file_stream.str(),
       "﻿l_japanese:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path polish_path(
       "output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/localisation/polish/"
       "victory_points_l_polish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(polish_path));
   std::ifstream polish_file(polish_path);
   ASSERT_TRUE(polish_file.is_open());
   std::stringstream polish_file_stream;
   std::copy(std::istreambuf_iterator<char>(polish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(polish_file_stream));
   polish_file.close();
   EXPECT_EQ(polish_file_stream.str(),
       "﻿l_polish:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path russian_path(
       "output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/localisation/russian/"
       "victory_points_l_russian.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(russian_path));
   std::ifstream russian_file(russian_path);
   ASSERT_TRUE(russian_file.is_open());
   std::stringstream russian_file_stream;
   std::copy(std::istreambuf_iterator<char>(russian_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(russian_file_stream));
   russian_file.close();
   EXPECT_EQ(russian_file_stream.str(),
       "﻿l_russian:\n"
       " test_state:0 \"test\"\n"
       " test_state_two:0 \"test two\"\n");

   path spanish_path(
       "output/Outhoi4Localizations/VictoryPointLocalizationsAreOutput/localisation/spanish/"
       "victory_points_l_spanish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(spanish_path));
   std::ifstream spanish_file(spanish_path);
   ASSERT_TRUE(spanish_file.is_open());
   std::stringstream spanish_file_stream;
   std::copy(std::istreambuf_iterator<char>(spanish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(spanish_file_stream));
   spanish_file.close();
   EXPECT_EQ(spanish_file_stream.str(),
       "﻿l_spanish:\n"
       " test_state:0 \"prueba\"\n"
       " test_state_two:0 \"prueba dos\"\n");
}


TEST(Outhoi4LocalizationsOutlocalizationsTests, CharacterLocalizationsAreOutput)
{
   remove_all("output/Outhoi4Localizations/CharacterLocalizationsAreOutput/");
   create_directories("output/Outhoi4Localizations/CharacterLocalizationsAreOutput/");

   const commonItems::LocalizationDatabase country_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase state_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase victory_point_localizations("english", {"spanish"});

   commonItems::LocalizationDatabase character_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("Lizzy", "english");
   block_one.ModifyLocalization("english", "Elizabeth");
   block_one.ModifyLocalization("spanish", "Isabel");
   character_localizations.AddOrModifyLocalizationBlock("Lizzy", block_one);
   commonItems::LocalizationBlock block_two("Stanton", "english");
   block_two.ModifyLocalization("english", "Cady Stanton");
   block_two.ModifyLocalization("spanish", "Cady Stanton");
   character_localizations.AddOrModifyLocalizationBlock("Stanton", block_two);
   commonItems::LocalizationBlock block_three("Lizzy_Stanton", "english");
   block_three.ModifyLocalization("english", "$Lizzy$ $Stanton$");
   block_three.ModifyLocalization("spanish", "$Lizzy$ $Stanton$");
   character_localizations.AddOrModifyLocalizationBlock("Lizzy_Stanton", block_three);

   const commonItems::LocalizationDatabase idea_localizations("english", {"spanish"});

   const hoi4::Localizations localizations(country_localizations,
       state_localizations,
       victory_point_localizations,
       character_localizations,
       idea_localizations);

   OutputLocalizations("Outhoi4Localizations/CharacterLocalizationsAreOutput", localizations);

   path portuguese_path(
       "output/Outhoi4Localizations/CharacterLocalizationsAreOutput/localisation/braz_por/"
       "character_names_l_braz_por.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(portuguese_path));
   std::ifstream braz_por_file(portuguese_path);
   ASSERT_TRUE(braz_por_file.is_open());
   std::stringstream braz_por_file_stream;
   std::copy(std::istreambuf_iterator<char>(braz_por_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(braz_por_file_stream));
   braz_por_file.close();
   EXPECT_EQ(braz_por_file_stream.str(),
       "﻿l_braz_por:\n"
       " Lizzy:0 \"Elizabeth\"\n"
       " Lizzy_Stanton:0 \"$Lizzy$ $Stanton$\"\n"
       " Stanton:0 \"Cady Stanton\"\n");

   path english_path(
       "output/Outhoi4Localizations/CharacterLocalizationsAreOutput/localisation/english/"
       "character_names_l_english.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(english_path));
   std::ifstream english_file(english_path);
   ASSERT_TRUE(english_file.is_open());
   std::stringstream english_file_stream;
   std::copy(std::istreambuf_iterator<char>(english_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(english_file_stream));
   english_file.close();
   EXPECT_EQ(english_file_stream.str(),
       "﻿l_english:\n"
       " Lizzy:0 \"Elizabeth\"\n"
       " Lizzy_Stanton:0 \"$Lizzy$ $Stanton$\"\n"
       " Stanton:0 \"Cady Stanton\"\n");

   path french_path(
       "output/Outhoi4Localizations/CharacterLocalizationsAreOutput/localisation/french/"
       "character_names_l_french.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(french_path));
   std::ifstream french_file(french_path);
   ASSERT_TRUE(french_file.is_open());
   std::stringstream french_file_stream;
   std::copy(std::istreambuf_iterator<char>(french_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(french_file_stream));
   french_file.close();
   EXPECT_EQ(french_file_stream.str(),
       "﻿l_french:\n"
       " Lizzy:0 \"Elizabeth\"\n"
       " Lizzy_Stanton:0 \"$Lizzy$ $Stanton$\"\n"
       " Stanton:0 \"Cady Stanton\"\n");

   path german_path(
       "output/Outhoi4Localizations/CharacterLocalizationsAreOutput/localisation/german/"
       "character_names_l_german.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(german_path));
   std::ifstream german_file(german_path);
   ASSERT_TRUE(german_file.is_open());
   std::stringstream german_file_stream;
   std::copy(std::istreambuf_iterator<char>(german_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(german_file_stream));
   german_file.close();
   EXPECT_EQ(german_file_stream.str(),
       "﻿l_german:\n"
       " Lizzy:0 \"Elizabeth\"\n"
       " Lizzy_Stanton:0 \"$Lizzy$ $Stanton$\"\n"
       " Stanton:0 \"Cady Stanton\"\n");

   path japanese_path(
       "output/Outhoi4Localizations/CharacterLocalizationsAreOutput/localisation/japanese/"
       "character_names_l_japanese.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(japanese_path));
   std::ifstream japanese_file(japanese_path);
   ASSERT_TRUE(japanese_file.is_open());
   std::stringstream japanese_file_stream;
   std::copy(std::istreambuf_iterator<char>(japanese_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(japanese_file_stream));
   japanese_file.close();
   EXPECT_EQ(japanese_file_stream.str(),
       "﻿l_japanese:\n"
       " Lizzy:0 \"Elizabeth\"\n"
       " Lizzy_Stanton:0 \"$Lizzy$ $Stanton$\"\n"
       " Stanton:0 \"Cady Stanton\"\n");

   path polish_path(
       "output/Outhoi4Localizations/CharacterLocalizationsAreOutput/localisation/polish/"
       "character_names_l_polish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(polish_path));
   std::ifstream polish_file(polish_path);
   ASSERT_TRUE(polish_file.is_open());
   std::stringstream polish_file_stream;
   std::copy(std::istreambuf_iterator<char>(polish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(polish_file_stream));
   polish_file.close();
   EXPECT_EQ(polish_file_stream.str(),
       "﻿l_polish:\n"
       " Lizzy:0 \"Elizabeth\"\n"
       " Lizzy_Stanton:0 \"$Lizzy$ $Stanton$\"\n"
       " Stanton:0 \"Cady Stanton\"\n");

   path russian_path(
       "output/Outhoi4Localizations/CharacterLocalizationsAreOutput/localisation/russian/"
       "character_names_l_russian.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(russian_path));
   std::ifstream russian_file(russian_path);
   ASSERT_TRUE(russian_file.is_open());
   std::stringstream russian_file_stream;
   std::copy(std::istreambuf_iterator<char>(russian_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(russian_file_stream));
   russian_file.close();
   EXPECT_EQ(russian_file_stream.str(),
       "﻿l_russian:\n"
       " Lizzy:0 \"Elizabeth\"\n"
       " Lizzy_Stanton:0 \"$Lizzy$ $Stanton$\"\n"
       " Stanton:0 \"Cady Stanton\"\n");

   path spanish_path(
       "output/Outhoi4Localizations/CharacterLocalizationsAreOutput/localisation/spanish/"
       "character_names_l_spanish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(spanish_path));
   std::ifstream spanish_file(spanish_path);
   ASSERT_TRUE(spanish_file.is_open());
   std::stringstream spanish_file_stream;
   std::copy(std::istreambuf_iterator<char>(spanish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(spanish_file_stream));
   spanish_file.close();
   EXPECT_EQ(spanish_file_stream.str(),
       "﻿l_spanish:\n"
       " Lizzy:0 \"Isabel\"\n"
       " Lizzy_Stanton:0 \"$Lizzy$ $Stanton$\"\n"
       " Stanton:0 \"Cady Stanton\"\n");
}


TEST(Outhoi4LocalizationsOutlocalizationsTests, IdeaLocalizationsAreOutput)
{
   remove_all("output/Outhoi4Localizations/IdeaLocalizationsAreOutput/");
   create_directories("output/Outhoi4Localizations/IdeaLocalizationsAreOutput/");

   const commonItems::LocalizationDatabase country_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase state_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase victory_point_localizations("english", {"spanish"});
   const commonItems::LocalizationDatabase character_localizations("english", {"spanish"});

   commonItems::LocalizationDatabase idea_localizations("english", {"spanish"});
   commonItems::LocalizationBlock block_one("test_idea", "english");
   block_one.ModifyLocalization("english", "test");
   block_one.ModifyLocalization("spanish", "prueba");
   idea_localizations.AddOrModifyLocalizationBlock("test_idea", block_one);
   commonItems::LocalizationBlock block_two("test_idea_desc", "english");
   block_two.ModifyLocalization("english", "test two");
   block_two.ModifyLocalization("spanish", "prueba dos");
   idea_localizations.AddOrModifyLocalizationBlock("test_idea_desc", block_two);

   const hoi4::Localizations localizations(country_localizations,
       state_localizations,
       victory_point_localizations,
       character_localizations,
       idea_localizations);

   OutputLocalizations("Outhoi4Localizations/IdeaLocalizationsAreOutput", localizations);

   path portuguese_path(
       "output/Outhoi4Localizations/IdeaLocalizationsAreOutput/localisation/braz_por/converter_ideas_l_braz_por.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(portuguese_path));
   std::ifstream braz_por_file(portuguese_path);
   ASSERT_TRUE(braz_por_file.is_open());
   std::stringstream braz_por_file_stream;
   std::copy(std::istreambuf_iterator<char>(braz_por_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(braz_por_file_stream));
   braz_por_file.close();
   EXPECT_EQ(braz_por_file_stream.str(),
       "﻿l_braz_por:\n"
       " test_idea:0 \"test\"\n"
       " test_idea_desc:0 \"test two\"\n");

   path english_path(
       "output/Outhoi4Localizations/IdeaLocalizationsAreOutput/localisation/english/converter_ideas_l_english.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(english_path));
   std::ifstream english_file(english_path);
   ASSERT_TRUE(english_file.is_open());
   std::stringstream english_file_stream;
   std::copy(std::istreambuf_iterator<char>(english_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(english_file_stream));
   english_file.close();
   EXPECT_EQ(english_file_stream.str(),
       "﻿l_english:\n"
       " test_idea:0 \"test\"\n"
       " test_idea_desc:0 \"test two\"\n");

   path french_path(
       "output/Outhoi4Localizations/IdeaLocalizationsAreOutput/localisation/french/converter_ideas_l_french.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(french_path));
   std::ifstream french_file(french_path);
   ASSERT_TRUE(french_file.is_open());
   std::stringstream french_file_stream;
   std::copy(std::istreambuf_iterator<char>(french_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(french_file_stream));
   french_file.close();
   EXPECT_EQ(french_file_stream.str(),
       "﻿l_french:\n"
       " test_idea:0 \"test\"\n"
       " test_idea_desc:0 \"test two\"\n");

   path german_path(
       "output/Outhoi4Localizations/IdeaLocalizationsAreOutput/localisation/german/converter_ideas_l_german.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(german_path));
   std::ifstream german_file(german_path);
   ASSERT_TRUE(german_file.is_open());
   std::stringstream german_file_stream;
   std::copy(std::istreambuf_iterator<char>(german_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(german_file_stream));
   german_file.close();
   EXPECT_EQ(german_file_stream.str(),
       "﻿l_german:\n"
       " test_idea:0 \"test\"\n"
       " test_idea_desc:0 \"test two\"\n");

   path japanese_path(
       "output/Outhoi4Localizations/IdeaLocalizationsAreOutput/localisation/japanese/converter_ideas_l_japanese.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(japanese_path));
   std::ifstream japanese_file(japanese_path);
   ASSERT_TRUE(japanese_file.is_open());
   std::stringstream japanese_file_stream;
   std::copy(std::istreambuf_iterator<char>(japanese_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(japanese_file_stream));
   japanese_file.close();
   EXPECT_EQ(japanese_file_stream.str(),
       "﻿l_japanese:\n"
       " test_idea:0 \"test\"\n"
       " test_idea_desc:0 \"test two\"\n");

   path polish_path(
       "output/Outhoi4Localizations/IdeaLocalizationsAreOutput/localisation/polish/converter_ideas_l_polish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(polish_path));
   std::ifstream polish_file(polish_path);
   ASSERT_TRUE(polish_file.is_open());
   std::stringstream polish_file_stream;
   std::copy(std::istreambuf_iterator<char>(polish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(polish_file_stream));
   polish_file.close();
   EXPECT_EQ(polish_file_stream.str(),
       "﻿l_polish:\n"
       " test_idea:0 \"test\"\n"
       " test_idea_desc:0 \"test two\"\n");

   path russian_path(
       "output/Outhoi4Localizations/IdeaLocalizationsAreOutput/localisation/russian/converter_ideas_l_russian.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(russian_path));
   std::ifstream russian_file(russian_path);
   ASSERT_TRUE(russian_file.is_open());
   std::stringstream russian_file_stream;
   std::copy(std::istreambuf_iterator<char>(russian_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(russian_file_stream));
   russian_file.close();
   EXPECT_EQ(russian_file_stream.str(),
       "﻿l_russian:\n"
       " test_idea:0 \"test\"\n"
       " test_idea_desc:0 \"test two\"\n");

   path spanish_path(
       "output/Outhoi4Localizations/IdeaLocalizationsAreOutput/localisation/spanish/converter_ideas_l_spanish.yml");
   ASSERT_TRUE(commonItems::DoesFileExist(spanish_path));
   std::ifstream spanish_file(spanish_path);
   ASSERT_TRUE(spanish_file.is_open());
   std::stringstream spanish_file_stream;
   std::copy(std::istreambuf_iterator<char>(spanish_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(spanish_file_stream));
   spanish_file.close();
   EXPECT_EQ(spanish_file_stream.str(),
       "﻿l_spanish:\n"
       " test_idea:0 \"prueba\"\n"
       " test_idea_desc:0 \"prueba dos\"\n");
}

}  // namespace out