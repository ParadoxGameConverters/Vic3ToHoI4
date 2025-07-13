#ifndef CONFIGURATION_CONFIGURATION_H
#define CONFIGURATION_CONFIGURATION_H



#include <external/commonItems/ConverterVersion.h>
#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
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
   std::filesystem::path vic3_directory;
   std::filesystem::path vic3_steam_mod_path;
   std::filesystem::path vic3_mod_path;
   std::filesystem::path hoi4_directory;
   std::filesystem::path hoi4_mod_path;
   std::filesystem::path save_game;
   bool debug = false;
   std::string output_name;
   bool dynamic_resources = true;
   UseStories use_stories = UseStories::kNo;

   void VerifyVic3Path()
   {
      if (!commonItems::DoesFolderExist(vic3_directory))
      {
         throw std::runtime_error(fmt::format("Victoria 3 path {} doesn't exist.", vic3_directory.string()));
      }
      if (!commonItems::DoesFileExist(vic3_directory / "binaries/victoria3.exe") &&
          !commonItems::DoesFileExist(vic3_directory / "binaries/victoria3"))
      {
         throw std::runtime_error(fmt::format("{} does not contain Victoria 3.", vic3_directory.string()));
      }
   }

   void VerifyHOI4Path()
   {
      if (!commonItems::DoesFolderExist(hoi4_directory))
      {
         throw std::runtime_error(fmt::format("Hearts of Iron 4 path {} doesn't exist.", hoi4_directory.string()));
      }
      if (!commonItems::DoesFileExist(hoi4_directory / "hoi4.exe") &&
          !commonItems::DoesFileExist(hoi4_directory / "hoi4"))
      {
         throw std::runtime_error(fmt::format("{} does not contain Hearts of Iron 4.", hoi4_directory.string()));
      }
   }

   void VerifyVic3Version(const commonItems::ConverterVersion& converter_version) const
   {
      const auto v3_version =
          GameVersion::extractVersionFromLauncher(vic3_directory / "launcher/launcher-settings.json");
      if (!v3_version)
      {
         Log(LogLevel::Error) << "Vic3 version could not be determined, proceeding blind!";
         return;
      }

      Log(LogLevel::Info) << "Vic3 version: " << v3_version->toShortString();

      if (converter_version.getMinSource() > *v3_version)
      {
         Log(LogLevel::Error) << "Vic3 version is v" << v3_version->toShortString() << ", converter requires minimum v"
                              << converter_version.getMinSource().toShortString() << "!";
         throw std::runtime_error("Converter vs Vic3 installation mismatch!");
      }

      if (!converter_version.getMaxSource().isLargerishThan(*v3_version))
      {
         Log(LogLevel::Error) << "Vic3 version is v" << v3_version->toShortString() << ", converter requires maximum v"
                              << converter_version.getMaxSource().toShortString() << "!";
         throw std::runtime_error("Converter vs Vic3 installation mismatch!");
      }
   }

   void VerifyHOI4Version(const commonItems::ConverterVersion& converter_version) const
   {
      const auto hoi4_version = GameVersion::extractVersionFromLauncher(hoi4_directory / "launcher-settings.json");
      if (!hoi4_version)
      {
         Log(LogLevel::Error) << "HOI4 version could not be determined, proceeding blind!";
         return;
      }

      Log(LogLevel::Info) << "HOI4 version: " << hoi4_version->toShortString();

      if (converter_version.getMinTarget() > *hoi4_version)
      {
         Log(LogLevel::Error) << "HOI4 version is v" << hoi4_version->toShortString()
                              << ", converter requires minimum v" << converter_version.getMinTarget().toShortString()
                              << "!";
         throw std::runtime_error("Converter vs HOI4 installation mismatch!");
      }

      if (!converter_version.getMaxTarget().isLargerishThan(*hoi4_version))
      {
         Log(LogLevel::Error) << "HOI4 version is v" << hoi4_version->toShortString()
                              << ", converter requires maximum v" << converter_version.getMaxTarget().toShortString()
                              << "!";
         throw std::runtime_error("Converter vs HOI4 installation mismatch!");
      }
   }
};

const Configuration kDefaultConfig = {};
const Configuration kDebugConfig = {.debug = true};


}  // namespace configuration


#endif  // SRC_CONFIGURATION_CONFIGURATION_H