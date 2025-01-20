#include "src/out_hoi4/flags/out_flags.h"

#include <external/commonItems/Log.h>

#include <ranges>

#include "src/mappers/flags/flag_mapper.h"
#include "src/mappers/flags/flag_mapper_builder.h"


namespace out
{

void OutputFlags(const std::filesystem::path& output_name,
    const std::map<std::string, hoi4::Country>& countries,
    const commonItems::ModFilesystem& hoi4_mod_filesystem)
{
   mappers::FlagMapperBuilder builder;
   builder.ReadConfig("configurables/flag_mapper_config.txt");
   if (!builder.CreateTargetFolders(output_name))
   {
      Log(LogLevel::Warning) << "Could not create flags folders, flags will not be copied.";
      return;
   }
   auto flag_mapper = builder.Build(hoi4_mod_filesystem);
   auto key_view = std::views::keys(countries);
   std::vector<std::string> tags(key_view.begin(), key_view.end());
   if (!flag_mapper.CopyFlags(tags))
   {
      Log(LogLevel::Warning) << "Could not copy all flags.";
   }
}

}  // namespace out
