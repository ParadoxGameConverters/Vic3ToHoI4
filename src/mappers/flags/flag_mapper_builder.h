#ifndef SRC_MAPPERS_FLAGS_FLAGMAPPERBUILDER_H
#define SRC_MAPPERS_FLAGS_FLAGMAPPERBUILDER_H

#include <external/commonItems/ModLoader/ModFilesystem.h>

#include <filesystem>
#include <set>
#include <string>

#include "src/mappers/flags/flag_mapper.h"


namespace mappers
{
class FlagMapperBuilder
{
  public:
   void ReadConfig(const std::string& config_file);
   bool CreateTargetFolders(const std::string output_name);
   FlagMapper Build(const commonItems::ModFilesystem& hoi4_mod_filesystem);

  private:
   std::set<std::string> forbid_;
   std::filesystem::path base_folder_;
   std::string custom_flag_folder_;
};

}  // namespace mappers

#endif  // SRC_MAPPERS_FLAGS_FLAGMAPPERBUILDER_H
