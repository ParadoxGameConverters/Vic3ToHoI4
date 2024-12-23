#ifndef CONFIGURATION_CONFIGURATIONIMPORTER_H
#define CONFIGURATION_CONFIGURATIONIMPORTER_H



#include <external/commonItems/Parser.h>

#include <string_view>

#include "src/configuration/configuration.h"



namespace configuration
{

[[nodiscard]] Configuration ImportConfiguration(std::string_view configuration_file,
    const commonItems::ConverterVersion& converter_version);

}  // namespace configuration



#endif  // CONFIGURATION_CONFIGURATIONIMPORTER_H