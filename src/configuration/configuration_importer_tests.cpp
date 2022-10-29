#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/configuration/configuration_importer.h"



namespace configuration
{

TEST(ConfigurationTest, DefaultsAreDefaulted)
{
   const auto configuration =
       configuration::ConfigurationImporter{}.ImportConfiguration("test_files/configuration/blank_configuration.txt");

   EXPECT_TRUE(configuration.vic3_directory.empty());
   EXPECT_TRUE(configuration.vic3_mod_path.empty());
   EXPECT_TRUE(configuration.hoi4_directory.empty());
   EXPECT_TRUE(configuration.hoi4_mod_path.empty());
   EXPECT_FALSE(configuration.debug);
}


TEST(ConfigurationTest, ItemsCanBeImported)
{
   const auto configuration =
       configuration::ConfigurationImporter{}.ImportConfiguration("test_files/configuration/test_configuration.txt");

   EXPECT_EQ(configuration.vic3_directory, "vic3_install_directory");
   EXPECT_EQ(configuration.vic3_mod_path, "vic3_mod_directory");
   EXPECT_EQ(configuration.hoi4_directory, "hoi4_install_directory");
   EXPECT_EQ(configuration.hoi4_mod_path, "hoi4_mod_directory");
   EXPECT_EQ(configuration.save_game, "test_save.v3");
   EXPECT_TRUE(configuration.debug);
   EXPECT_EQ(configuration.output_name, "test_output_name");
}


TEST(ConfigurationTest, BadSaveNameThrowsException)
{
   EXPECT_THROW(
       configuration::ConfigurationImporter{}.ImportConfiguration("test_files/configuration/bad_save_name.txt"),
       std::invalid_argument);
}


TEST(ConfigurationTest, OutputNameIsFromSave)
{
   const auto configuration = configuration::ConfigurationImporter{}.ImportConfiguration(
       "test_files/configuration/output_name_from_save_configuration.txt");

   EXPECT_EQ(configuration.output_name, "test_save_with_spaces");
}


TEST(ConfigurationTest, CustomOutputOverridesSaveOutputName)
{
   const auto configuration =
       configuration::ConfigurationImporter{}.ImportConfiguration("test_files/configuration/output_name_override.txt");

   EXPECT_EQ(configuration.output_name, "path_has_not__been__removed__override_name_with_spaces");
}

}  // namespace configuration