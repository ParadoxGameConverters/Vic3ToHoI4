#ifndef OUT_HOI4_OUT_MOD_H
#define OUT_HOI4_OUT_MOD_H



#include <external/commonItems/GameVersion.h>

#include <filesystem>
#include <string_view>



namespace out
{

void ClearOutputFolder(const std::filesystem::path& output_name);
void OutputMod(std::string_view output_name, const GameVersion& game_version);

}  // namespace out



#endif  // OUT_HOI4_OUT_MOD_H