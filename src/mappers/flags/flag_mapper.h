#ifndef SRC_MAPPERS_FLAGS_FLAGMAPPER_H
#define SRC_MAPPERS_FLAGS_FLAGMAPPER_H

#include <filesystem>
#include <map>
#include <string>

namespace mappers
{

class FlagMapper
{
  public:
   explicit FlagMapper(const std::filesystem::path& base, std::map<std::string, std::filesystem::path>&& flags):
       base_flag_folder_(base),
       available_flags_(flags)
   {
   }

   bool CreateFlagFolders() const;
   bool CopyFlag(const std::string& tag);

  private:
   std::filesystem::path base_flag_folder_;
   std::map<std::string, std::filesystem::path> available_flags_;
};

}  // namespace mappers


#endif  // SRC_MAPPERS_FLAGS_FLAGMAPPER_H
