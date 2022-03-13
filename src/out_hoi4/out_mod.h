#ifndef OUT_HOI4_OUT_MOD_H_
#define OUT_HOI4_OUT_MOD_H_



#include <string_view>

#include "external/commonItems/GameVersion.h"



namespace out
{

void ClearOutputFolder(std::string_view output_name);
void Output(std::string_view output_name, const GameVersion& game_version);

}  // namespace out



#endif  // OUT_HOI4_OUT_MOD_H_