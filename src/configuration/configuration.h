#ifndef CONFIGURATION_CONFIGURATION_H
#define CONFIGURATION_CONFIGURATION_H



#include <external/commonItems/ConverterVersion.h>
#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/fmt/include/fmt/format.h>

#include <string>



namespace configuration
{

enum class UseStories
{
   kNo,
   kYes
};


struct Configuration
{
   std::string vic3_directory;
   std::string vic3_steam_mod_path;
   std::string vic3_mod_path;
   std::string hoi4_directory;
   std::string hoi4_mod_path;
   std::string save_game;
   bool debug = false;
   std::string output_name;
   bool dynamic_resources = true;
   UseStories use_stories = UseStories::kNo;

   void verifyVic3Path()
   {
      if (!commonItems::DoesFolderExist(vic3_directory))
      {
         throw std::runtime_error(fmt::format("Victoria 3 path {} doesn't exist.", vic3_directory));
      }
      if (!commonItems::DoesFileExist(vic3_directory + "/binaries/victoria3.exe") &&
          !commonItems::DoesFileExist(vic3_directory + "/binaries/victoria3"))
      {
         throw std::runtime_error(fmt::format("{} does not contain Victoria 3.", vic3_directory));
      }
   }

   void verifyHOI4Path()
   {
      if (!commonItems::DoesFolderExist(hoi4_directory))
      {
         throw std::runtime_error(fmt::format("Hearts of Iron 4 path {} doesn't exist.", hoi4_directory));
      }
      if (!commonItems::DoesFileExist(hoi4_directory + "/hoi4.exe") &&
          !commonItems::DoesFileExist(hoi4_directory + "/hoi4"))
      {
         throw std::runtime_error(fmt::format("{} does not contain Hearts of Iron 4.", hoi4_directory));
      }
   }

   void verifyVic3Version(const commonItems::ConverterVersion& converterVersion) const
   {
      const auto V3Version =
          GameVersion::extractVersionFromLauncher(vic3_directory + "/launcher/launcher-settings.json");
      if (!V3Version)
      {
         Log(LogLevel::Error) << "Vic3 version could not be determined, proceeding blind!";
         return;
      }

      Log(LogLevel::Info) << "Vic3 version: " << V3Version->toShortString();

      if (converterVersion.getMinSource() > *V3Version)
      {
         Log(LogLevel::Error) << "Vic3 version is v" << V3Version->toShortString() << ", converter requires minimum v"
                              << converterVersion.getMinSource().toShortString() << "!";
         throw std::runtime_error("Converter vs Vic3 installation mismatch!");
      }

      if (!converterVersion.getMaxSource().isLargerishThan(*V3Version))
      {
         Log(LogLevel::Error) << "Vic3 version is v" << V3Version->toShortString() << ", converter requires maximum v"
                              << converterVersion.getMaxSource().toShortString() << "!";
         throw std::runtime_error("Converter vs Vic3 installation mismatch!");
      }
   }

   void verifyHOI4Version(const commonItems::ConverterVersion& converterVersion) const
   {
      const auto HOI4Version = GameVersion::extractVersionFromLauncher(hoi4_directory + "/launcher-settings.json");
      if (!HOI4Version)
      {
         Log(LogLevel::Error) << "HOI4 version could not be determined, proceeding blind!";
         return;
      }

      Log(LogLevel::Info) << "HOI4 version: " << HOI4Version->toShortString();

      if (converterVersion.getMinTarget() > *HOI4Version)
      {
         Log(LogLevel::Error) << "HOI4 version is v" << HOI4Version->toShortString() << ", converter requires minimum v"
                              << converterVersion.getMinTarget().toShortString() << "!";
         throw std::runtime_error("Converter vs HOI4 installation mismatch!");
      }

      if (!converterVersion.getMaxTarget().isLargerishThan(*HOI4Version))
      {
         Log(LogLevel::Error) << "HOI4 version is v" << HOI4Version->toShortString() << ", converter requires maximum v"
                              << converterVersion.getMaxTarget().toShortString() << "!";
         throw std::runtime_error("Converter vs HOI4 installation mismatch!");
      }
   }
};

const Configuration defaultConfig = {};
const Configuration debugConfig = {.debug = true};


}  // namespace configuration


#endif  // SRC_CONFIGURATION_CONFIGURATION_H