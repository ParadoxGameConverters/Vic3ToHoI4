#ifndef VIC3_TO_HOI4_CONVERTER_H
#define VIC3_TO_HOI4_CONVERTER_H



#include <string_view>

#include "external/commonItems/ConverterVersion.h"



void ConvertVic3ToHoi4(std::string_view output_name, const GameVersion& game_version);



#endif  // VIC3_TO_HOI4_CONVERTER_H