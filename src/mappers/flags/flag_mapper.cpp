#include "src/mappers/flags/flag_mapper.h"

#include <filesystem>

#include "external/commonItems/Log.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"

namespace
{


}  // namespace


bool mappers::FlagMapper::CopyFlags(const std::vector<std::string>& tags)
{
   std::set<std::string> tags_needing_flags;
   for (const auto& tag: tags)
   {
      // Ignore if we already have a custom or mod flag.
      if (available_flags_.contains(tag))
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
   if (available_flags_.contains(tag))
   {
      available_flags_.erase(tag);
      return false;
   }
   if (available_flags_.empty())
   {
      return false;
   }

   auto file_to_copy = available_flags_.extract(available_flags_.begin()).mapped().make_preferred();
   const auto fname = file_to_copy.filename();
   const auto extension = file_to_copy.extension();
   file_to_copy.remove_filename();
   for (const auto& folder: kFlagFolders)
   {
      // This ought to use path's operator/=, but it doesn't do what the
      // documentation says it ought to do. So use primitive string concat
      // instead like Turing intended.
      const auto source = fmt::format("{}{}{}", file_to_copy.string(), folder, fname.string());
      const auto target = fmt::format("{}{}{}{}", base_flag_folder_.string(), folder, tag, extension.string());
      if (!commonItems::TryCopyFile(source, target))
      {
         Log(LogLevel::Warning) << "Could not copy flag file " << source << " to " << target;
         return false;
      }
   }
   return true;
}
