#include "src/mappers/flags/flag_mapper_builder.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Log.h>
#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>
#include <map>
#include <string>



using std::filesystem::path;



namespace
{

extern const path kIgnoreFlag("ignore");


}  // namespace


namespace mappers
{

const std::vector<std::string> kFlagFolders{"", "small", "medium"};

void FlagMapperBuilder::ReadConfig(const path& config_file)
{
   commonItems::parser config_parser;
   config_parser.registerKeyword("forbid", [&](std::istream& input_stream) {
      forbid_.insert(commonItems::getString(input_stream));
   });
   config_parser.registerKeyword("custom_flags", [&](std::istream& input_stream) {
      custom_flag_folder_ = commonItems::getString(input_stream);
   });
   // TODO: Add a 'force' keyword to allow tag-to-flag mappings without copying flags to blank_mod.
   config_parser.parseFile(config_file);
}


bool FlagMapperBuilder::CreateTargetFolders(const path& output_name)
{
   base_folder_ = "output" / output_name / "gfx/flags";
   remove_all(base_folder_);
   if (!create_directories(base_folder_))
   {
      Log(LogLevel::Warning) << "Could not create flags folder.";
      return false;
   }
   if (!create_directories(base_folder_ / "small"))
   {
      Log(LogLevel::Warning) << "Could not create small flags folder.";
      return false;
   }
   if (!create_directories(base_folder_ / "medium"))
   {
      Log(LogLevel::Warning) << "Could not create medium flags folder.";
      return false;
   }

   return true;
}


FlagMapper FlagMapperBuilder::Build(const commonItems::ModFilesystem& hoi4_mod_filesystem)
{
   std::map<std::string, path> available_flags;
   std::set<std::string> custom_flags;
   // Check if there are custom flags in blank mod.
   Log(LogLevel::Info) << "Custom flag folder: " << custom_flag_folder_;
   if (!custom_flag_folder_.empty() && commonItems::DoesFolderExist(custom_flag_folder_))
   {
      Log(LogLevel::Info) << "  Exists.";
      for (const auto& filename: commonItems::GetAllFilesInFolder(custom_flag_folder_))
      {
         // Ignore non-tga files.
         if (filename.extension() != ".tga")
         {
            continue;
         }
         const auto& tag = filename.stem().string();
         // Will be removed later when notified of tag list.
         Log(LogLevel::Info) << "  Found tag " << tag << " at " << filename.string();
         custom_flags.insert(tag);
         forbid_.insert(tag);
      }
   }

   // Find all available flags in HoI mod.
   for (const auto& file: hoi4_mod_filesystem.GetAllFilesInFolder("gfx/flags"))
   {
      const auto& tag = file.stem().string();
      if (tag.size() != 3)
      {
         // Don't use government-specific flags to redistribute; but do mark
         // their existence so we don't try to get the tag a base flag.
         // TODO: Use the government-specific flags as well.
         const auto real_tag = tag.substr(0, 3);
         if (!available_flags.contains(real_tag))
         {
            available_flags[real_tag] = kIgnoreFlag;
         }
         continue;
      }
      if (forbid_.contains(tag))
      {
         continue;
      }
      available_flags[tag] = file;
   }

   // Remove the ignore flags so they don't get used for redistribution.
   std::erase_if(available_flags, [](const auto& item) {
      return item.second == kIgnoreFlag;
   });
   FlagMapper flag_mapper(base_folder_, available_flags, custom_flags);
   return flag_mapper;
}


}  // namespace mappers
