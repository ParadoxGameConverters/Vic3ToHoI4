#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/configuration/configuration_importer.h"



namespace configuration
{


TEST(ConfigurationTest, DefaultsAreDefaulted)
{
   const auto configuration =
       ImportConfiguration("test_files/configuration/blank_configuration.txt", commonItems::ConverterVersion());

   EXPECT_TRUE(configuration.vic3_directory.empty());
   EXPECT_TRUE(configuration.vic3_steam_mod_path.empty());
   EXPECT_TRUE(configuration.vic3_mod_path.empty());
   EXPECT_TRUE(configuration.hoi4_directory.empty());
   EXPECT_TRUE(configuration.hoi4_mod_path.empty());
   EXPECT_EQ(configuration.save_game,
       "test_save.v3");  // a missing save would throw an exception, so this is set in the 'blank' config
   EXPECT_FALSE(configuration.debug);
   EXPECT_EQ(configuration.output_name, "test_save");  // if not specified, derived from the save
   EXPECT_TRUE(configuration.dynamic_resources);
   EXPECT_EQ(configuration.use_stories, UseStories::kNo);
}


TEST(ConfigurationTest, ExceptionForMissingVic3Directory)
{
   EXPECT_THROW(const auto _ = ImportConfiguration("test_files/configuration/missing_vic3_directory.txt",
                    commonItems::ConverterVersion()),
       std::runtime_error);
}


TEST(ConfigurationTest, ExceptionForBadVic3Directory)
{
   EXPECT_THROW(const auto _ = ImportConfiguration("test_files/configuration/bad_vic3_directory.txt",
                    commonItems::ConverterVersion()),
       std::runtime_error);
}


TEST(ConfigurationTest, ExceptionForMissingHoI4Directory)
{
   EXPECT_THROW(const auto _ = ImportConfiguration("test_files/configuration/missing_hoi4_directory.txt",
                    commonItems::ConverterVersion()),
       std::runtime_error);
}


TEST(ConfigurationTest, ExceptionForBadHoI4Directory)
{
   EXPECT_THROW(const auto _ = ImportConfiguration("test_files/configuration/bad_hoi4_directory.txt",
                    commonItems::ConverterVersion()),
       std::runtime_error);
}


TEST(ConfigurationTest, ItemsCanBeImported)
{
   const auto configuration =
       ImportConfiguration("test_files/configuration/test_configuration.txt", commonItems::ConverterVersion());

   EXPECT_EQ(configuration.vic3_directory, R"(test_files/test_folders/vic3_folder)");
   EXPECT_EQ(configuration.vic3_steam_mod_path, "vic3_steam_mod_directory");
   EXPECT_EQ(configuration.vic3_mod_path, "vic3_mod_directory");
   EXPECT_EQ(configuration.hoi4_directory, R"(test_files/test_folders/hoi4_folder)");
   EXPECT_EQ(configuration.hoi4_mod_path, "hoi4_mod_directory");
   EXPECT_EQ(configuration.save_game, "test_save.v3");
   EXPECT_TRUE(configuration.debug);
   EXPECT_EQ(configuration.output_name, "test_output_name");
   EXPECT_FALSE(configuration.dynamic_resources);
   EXPECT_EQ(configuration.use_stories, UseStories::kYes);
}


TEST(ConfigurationTest, ItemsAreLoggedWhenImported)
{
   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const auto _ =
       ImportConfiguration("test_files/configuration/test_configuration.txt", commonItems::ConverterVersion());

   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Victoria 3 install path is "test_files/test_folders/vic3_folder")"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Victoria 3 Steam mod path is "vic3_steam_mod_directory")"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Victoria 3 mod path is "vic3_mod_directory")"));
   EXPECT_THAT(log.str(),
       testing::HasSubstr(R"(Hearts of Iron 4 install path is "test_files/test_folders/hoi4_folder")"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Hearts of Iron 4 mod path is "hoi4_mod_directory")"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Save game is "test_save.v3")"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Debug is active)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Using output name test_output_name)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Dynamic resources is not active)"));
   EXPECT_THAT(log.str(), testing::HasSubstr(R"(Stories system is active)"));

   std::cout.rdbuf(cout_buffer);
}


TEST(ConfigurationTest, BadSaveNameThrowsException)
{
   EXPECT_THROW(const auto _ =
                    ImportConfiguration("test_files/configuration/bad_save_name.txt", commonItems::ConverterVersion()),
       std::invalid_argument);
}


TEST(ConfigurationTest, OutputNameIsFromSave)
{
   const auto configuration = ImportConfiguration("test_files/configuration/output_name_from_save_configuration.txt",
       commonItems::ConverterVersion());

   EXPECT_EQ(configuration.output_name, "test_save_with_spaces");
}


TEST(ConfigurationTest, CustomOutputOverridesSaveOutputName)
{
   const auto configuration =
       ImportConfiguration("test_files/configuration/output_name_override.txt", commonItems::ConverterVersion());

   EXPECT_EQ(configuration.output_name, "path_has_not__been__removed__override_name_with_spaces");
}

}  // namespace configuration