#ifndef VIC3_TO_HOI4_CONVERTER_H
#define VIC3_TO_HOI4_CONVERTER_H



#include "external/commonItems/ConverterVersion.h"
#include "src/configuration/configuration.h"



void ConvertVic3ToHoi4(const configuration::Configuration& configuration,
    const commonItems::ConverterVersion& converter_version);



#endif  // VIC3_TO_HOI4_CONVERTER_H