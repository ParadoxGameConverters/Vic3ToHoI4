#include "src/mappers/flags/flag_mapper.h"

#include <filesystem>

#include "external/commonItems/Log.h"
#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"

namespace
{

const std::vector<std::string> folders{"\\", "\\small\\", "\\medium\\"};

}  // namespace


bool mappers::FlagMapper::CreateFlagFolders() const
{
   for (const auto& folder: folders)
   {
      auto target = fmt::format("{}{}", base_flag_folder_.string(), folder);
      if (!commonItems::DoesFolderExist(target))
      {
         if (!commonItems::TryCreateFolder(target))
         {
            Log(LogLevel::Warning) << "Could not create " << target << ", flags will not be copied.";
            return false;
         }
      }
   }
   return true;
}

bool mappers::FlagMapper::CopyFlag(const std::string& tag)
{
   if (available_flags_.empty())
   {
      Log(LogLevel::Warning) << "Ran out of available flags.";
      return false;
   }
   auto file_to_copy = available_flags_.extract(available_flags_.begin()).mapped().make_preferred();
   const auto fname = file_to_copy.filename();
   const auto extension = file_to_copy.extension();
   file_to_copy.remove_filename();
   for (const auto& folder: folders)
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
