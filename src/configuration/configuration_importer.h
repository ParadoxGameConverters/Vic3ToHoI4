#ifndef CONFIGURATION_CONFIGURATIONIMPORTER_H
#define CONFIGURATION_CONFIGURATIONIMPORTER_H



#include "external/commonItems/Parser.h"
#include "src/configuration/configuration.h"
#include <string_view>



namespace configuration
{

class ConfigurationImporter
{
  public:
   ConfigurationImporter();

   Configuration ImportConfiguration(std::string_view configuration_file);

  private:
   commonItems::parser configuration_parser_;
   Configuration configuration_;
};

}  // namespace configuration



#endif  // CONFIGURATION_CONFIGURATIONIMPORTER_H