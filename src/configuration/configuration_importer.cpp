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


configuration::Configuration configuration::ImportConfiguration(std::string_view configuration_file,
    const commonItems::ConverterVersion& converter_version)
{
   commonItems::parser configuration_parser;
   Configuration configuration;

   configuration_parser.registerKeyword("Vic3directory", [&configuration, &converter_version](std::istream& stream) {
      configuration.vic3_directory = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tVictoria 3 install path is " << configuration.vic3_directory;
      configuration.verifyVic3Path();
      configuration.verifyVic3Version(converter_version);
   });
   configuration_parser.registerKeyword("Vic3SteamModDirectory", [&configuration](std::istream& stream) {
      configuration.vic3_steam_mod_path = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tVictoria 3 Steam mod path is " << configuration.vic3_steam_mod_path;
   });
   configuration_parser.registerKeyword("sourceGameModPath", [&configuration](std::istream& stream) {
      configuration.vic3_mod_path = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tVictoria 3 mod path is " << configuration.vic3_mod_path;
   });
   configuration_parser.registerKeyword("HoI4directory", [&configuration, &converter_version](std::istream& stream) {
      configuration.hoi4_directory = commonItems::getString(stream);
      Log(LogLevel::Info) << "\tHearts of Iron 4 install path is " << configuration.hoi4_directory;
      configuration.verifyHOI4Path();
      configuration.verifyHOI4Version(converter_version);
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
   configuration_parser.registerKeyword("stories_system", [&configuration](std::istream& stream) {
       configuration.use_stories = commonItems::getString(stream) == "yes" ? UseStories::kYes : UseStories::kNo;
       if (configuration.use_stories == UseStories::kYes)
       {
           Log(LogLevel::Info) << "\tStories system is active";
       }
       else
       {
           Log(LogLevel::Info) << "\tStories system is not active";
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
