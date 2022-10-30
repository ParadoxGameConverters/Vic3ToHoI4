#include "src/configuration/configuration_importer.h"

#include "external/commonItems/CommonFunctions.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"



namespace
{

std::string DetermineOutputName(std::string_view save_name)
{
   std::string output_name = trimPath(std::string(save_name));
   if (getExtension(output_name) != "v3")
   {
      throw std::invalid_argument("The save was not a Vic3 save. Choose a save ending in '.v3' and convert again.");
   }

   output_name = trimExtension(output_name);

   return output_name;
}

}  // namespace


configuration::ConfigurationImporter::ConfigurationImporter()
{
   configuration_parser_.registerKeyword("Vic3directory", [this](std::istream& stream) {
      configuration_.vic3_directory = commonItems::getString(stream);
      if (!commonItems::DoesFolderExist(configuration_.vic3_directory))
      {
         throw std::runtime_error(fmt::format("Victoria 3 path {} doesn't exist.", configuration_.vic3_directory));
      }
      if (!commonItems::DoesFileExist(configuration_.vic3_directory + "/binaries/victoria3.exe") &&
          !commonItems::DoesFileExist(configuration_.vic3_directory + "/binaries/victoria3"))
      {
         throw std::runtime_error(fmt::format("{} does not contain Victoria 3.", configuration_.vic3_directory));
      }
      Log(LogLevel::Info) << "\tVictoria 3 install path is " << configuration_.vic3_directory;
   });
   configuration_parser_.registerKeyword("sourceGameModPath", [this](std::istream& stream) {
      configuration_.vic3_mod_path = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tVictoria 3 mod path is " << configuration_.vic3_mod_path;
   });
   configuration_parser_.registerKeyword("HoI4directory", [this](std::istream& stream) {
      configuration_.hoi4_directory = commonItems::getString(stream);
      if (!commonItems::DoesFolderExist(configuration_.hoi4_directory))
      {
         throw std::runtime_error(
             fmt::format("Hearts of Iron 4 path {} doesn't exist.", configuration_.hoi4_directory));
      }
      if (!commonItems::DoesFileExist(configuration_.hoi4_directory + "/hoi4.exe") &&
          !commonItems::DoesFileExist(configuration_.hoi4_directory + "/hoi4"))
      {
         throw std::runtime_error(fmt::format("{} does not contain Hearts of Iron 4.", configuration_.hoi4_directory));
      }
      Log(LogLevel::Info) << "\tHearts of Iron 4 install path is " << configuration_.hoi4_directory;
   });
   configuration_parser_.registerKeyword("targetGameModPath", [this](std::istream& stream) {
      configuration_.hoi4_mod_path = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tHearts of Iron 4 mod path is " << configuration_.hoi4_mod_path;
   });
   configuration_parser_.registerKeyword("SaveGame", [this](std::istream& stream) {
      configuration_.save_game = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tSave game is " << configuration_.save_game;
   });
   configuration_parser_.registerKeyword("debug", [this](std::istream& stream) {
      configuration_.debug = commonItems::getString(stream) == "yes";
      if (configuration_.debug)
      {
         Log(LogLevel::Info) << "\tDebug is active";
      }
   });
   configuration_parser_.registerKeyword("output_name", [this](std::istream& stream) {
      configuration_.output_name = commonItems::getString(stream);
   });
}


configuration::Configuration configuration::ConfigurationImporter::ImportConfiguration(
    std::string_view configuration_file)
{
   configuration_parser_.parseFile(configuration_file);

   if (configuration_.output_name.empty())
   {
      configuration_.output_name = DetermineOutputName(configuration_.save_game);
   }
   configuration_.output_name = normalizeStringPath(configuration_.output_name);

   Log(LogLevel::Info) << "Using output name " << configuration_.output_name;

   return configuration_;
}