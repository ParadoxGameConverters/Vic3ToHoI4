#include "vic3_to_hoi4_converter.h"

#include "external/commonItems/Log.h"
#include "src/out_hoi4/out_mod.h"



void ConvertVic3ToHoi4(std::string_view output_name, const GameVersion& game_version)
{
   out::ClearOutputFolder(output_name);
   out::Output(output_name, game_version);
   Log(LogLevel::Progress) << "100%";
   Log(LogLevel::Notice) << "* Conversion complete *";
}