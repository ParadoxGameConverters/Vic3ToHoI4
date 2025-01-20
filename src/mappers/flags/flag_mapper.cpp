#include "src/mappers/flags/flag_mapper.h"

#include <external/commonItems/Log.h>
#include <external/commonItems/OSCompatibilityLayer.h>
#include <external/fmt/include/fmt/format.h>

#include <filesystem>



using std::filesystem::copy_options;
using std::filesystem::path;



namespace
{


}  // namespace


bool mappers::FlagMapper::CopyFlags(const std::vector<std::string>& tags)
{
   std::set<std::string> tags_needing_flags;
   for (const auto& tag: tags)
   {
      // Ignore if we already have a custom or mod flag.
      if (available_flags_.contains(tag) || custom_flags_.contains(tag))
      {
         available_flags_.erase(tag);
         continue;
      }
      tags_needing_flags.insert(tag);
   }
   for (const auto& tag: tags_needing_flags)
   {
      if (available_flags_.empty())
      {
         Log(LogLevel::Warning) << "Ran out of available flags.";
         return false;
      }
      if (!CopyFlag(tag))
      {
         return false;
      }
   }
   return true;
}

bool mappers::FlagMapper::CopyFlag(const std::string& tag)
{
   if (available_flags_.contains(tag) || custom_flags_.contains(tag))
   {
      available_flags_.erase(tag);
      return false;
   }
   if (available_flags_.empty())
   {
      return false;
   }

   path file_to_copy = available_flags_.extract(available_flags_.begin()).mapped();
   const path filename = file_to_copy.filename();
   const path extension = file_to_copy.extension();
   file_to_copy.remove_filename();
   for (const auto& folder: kFlagFolders)
   {
      const path source = file_to_copy / folder / filename;
      path target = base_flag_folder_ / folder / tag;
      target += extension;
      try
      {
         if (!copy_file(source, target, copy_options::overwrite_existing))
         {
            Log(LogLevel::Warning) << "Could not copy flag file " << source << " to " << target;
            return false;
         }
      }
      catch (...)
      {
         Log(LogLevel::Warning) << "Could not copy flag file " << source << " to " << target;
         return false;
      }
   }
   return true;
}
