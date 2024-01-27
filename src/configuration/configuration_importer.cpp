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


configuration::Configuration configuration::ImportConfiguration(std::string_view configuration_file)
{
   commonItems::parser configuration_parser;
   Configuration configuration;

   configuration_parser.registerKeyword("Vic3directory", [&configuration](std::istream& stream) {
      configuration.vic3_directory = commonItems::getString(stream);
      if (!commonItems::DoesFolderExist(configuration.vic3_directory))
      {
         throw std::runtime_error(fmt::format("Victoria 3 path {} doesn't exist.", configuration.vic3_directory));
      }
      if (!commonItems::DoesFileExist(configuration.vic3_directory + "/binaries/victoria3.exe") &&
          !commonItems::DoesFileExist(configuration.vic3_directory + "/binaries/victoria3"))
      {
         throw std::runtime_error(fmt::format("{} does not contain Victoria 3.", configuration.vic3_directory));
      }
      Log(LogLevel::Info) << "\tVictoria 3 install path is " << configuration.vic3_directory;
   });
   configuration_parser.registerKeyword("Vic3SteamModDirectory", [&configuration](std::istream& stream) {
      configuration.vic3_steam_mod_path = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tVictoria 3 Steam mod path is " << configuration.vic3_steam_mod_path;
   });
   configuration_parser.registerKeyword("sourceGameModPath", [&configuration](std::istream& stream) {
      configuration.vic3_mod_path = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tVictoria 3 mod path is " << configuration.vic3_mod_path;
   });
   configuration_parser.registerKeyword("HoI4directory", [&configuration](std::istream& stream) {
      configuration.hoi4_directory = commonItems::getString(stream);
      if (!commonItems::DoesFolderExist(configuration.hoi4_directory))
      {
         throw std::runtime_error(fmt::format("Hearts of Iron 4 path {} doesn't exist.", configuration.hoi4_directory));
      }
      if (!commonItems::DoesFileExist(configuration.hoi4_directory + "/hoi4.exe") &&
          !commonItems::DoesFileExist(configuration.hoi4_directory + "/hoi4"))
      {
         throw std::runtime_error(fmt::format("{} does not contain Hearts of Iron 4.", configuration.hoi4_directory));
      }
      Log(LogLevel::Info) << "\tHearts of Iron 4 install path is " << configuration.hoi4_directory;
   });
   configuration_parser.registerKeyword("targetGameModPath", [&configuration](std::istream& stream) {
      configuration.hoi4_mod_path = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tHearts of Iron 4 mod path is " << configuration.hoi4_mod_path;
   });
   configuration_parser.registerKeyword("SaveGame", [&configuration](std::istream& stream) {
      configuration.save_game = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tSave game is " << configuration.save_game;
   });
   configuration_parser.registerKeyword("debug", [&configuration](std::istream& stream) {
      configuration.debug = commonItems::getString(stream) == "yes";
      if (configuration.debug)
      {
         Log(LogLevel::Info) << "\tDebug is active";
      }
      else
      {
         Log(LogLevel::Info) << "\tDebug is not active";
      }
   });
   configuration_parser.registerKeyword("output_name", [&configuration](std::istream& stream) {
      configuration.output_name = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tOutput name is " << configuration.output_name;
   });
   configuration_parser.registerKeyword("dynamic_resources", [&configuration](std::istream& stream) {
      configuration.dynamic_resources = commonItems::getString(stream) == "yes";
      if (configuration.dynamic_resources)
      {
         Log(LogLevel::Info) << "\tDynamic resources is active";
      }
      else
      {
         Log(LogLevel::Info) << "\tDynamic resources is not active";
      }
   });

   configuration_parser.parseFile(configuration_file);

   if (configuration.output_name.empty())
   {
      configuration.output_name = DetermineOutputName(configuration.save_game);
   }
   configuration.output_name = normalizeStringPath(configuration.output_name);

   Log(LogLevel::Info) << "Using output name " << configuration.output_name;

   return configuration;
}
