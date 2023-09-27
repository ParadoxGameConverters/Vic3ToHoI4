#ifndef SRC_MAPPERS_FLAGS_FLAGMAPPER_H
#define SRC_MAPPERS_FLAGS_FLAGMAPPER_H

#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace mappers
{

extern const std::vector<std::string> kFlagFolders;

class FlagMapper
{
  public:
   explicit FlagMapper(const std::filesystem::path& base,
       std::set<std::string>&& custom_flags,
       std::map<std::string, std::filesystem::path>&& flags):
       base_flag_folder_(base),
       custom_flags_(custom_flags),
       available_flags_(flags)
   {
   }

   bool CopyFlags(const std::vector<std::string>& tags);
   bool CopyFlag(const std::string& tag);

  private:
   std::filesystem::path base_flag_folder_;
   std::map<std::string, std::filesystem::path> available_flags_;
   std::set<std::string> custom_flags_;
};

}  // namespace mappers


#endif  // SRC_MAPPERS_FLAGS_FLAGMAPPER_H
