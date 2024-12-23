#ifndef OUT_HOI4_OUT_MOD_H_
#define OUT_HOI4_OUT_MOD_H_



#include <external/commonItems/GameVersion.h>

#include <string_view>



namespace out
{

void ClearOutputFolder(std::string_view output_name);
void OutputMod(std::string_view output_name, const GameVersion& game_version);

}  // namespace out



#endif  // OUT_HOI4_OUT_MOD_H_