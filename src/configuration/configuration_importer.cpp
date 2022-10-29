#include "src/configuration/configuration_importer.h"

#include "external/commonItems/CommonFunctions.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/ParserHelpers.h"



namespace
{

std::string DetermineOutputName(std::string_view save_name)
{
   std::string output_name = trimPath(std::string(save_name));
   if (getExtension(output_name) != "v3")
   {
      throw std::invalid_argument("The save was not a Vic3 save. Choose a save ending in '.v3' and convert again.");
   }

   output_name = trimExtension(output_name);

   return output_name;
}

}  // namespace


configuration::ConfigurationImporter::ConfigurationImporter()
{
   configuration_parser_.registerKeyword("Vic3directory", [this](std::istream& stream) {
      configuration_.vic3_directory = commonItems::getString(stream);
   });
   configuration_parser_.registerKeyword("sourceGameModPath", [this](std::istream& stream) {
      configuration_.vic3_mod_path = commonItems::getString(stream);
   });
   configuration_parser_.registerKeyword("HoI4directory", [this](std::istream& stream) {
      configuration_.hoi4_directory = commonItems::getString(stream);
   });
   configuration_parser_.registerKeyword("targetGameModPath", [this](std::istream& stream) {
      configuration_.hoi4_mod_path = commonItems::getString(stream);
   });
   configuration_parser_.registerKeyword("SaveGame", [this](std::istream& stream) {
      configuration_.save_game = commonItems::getString(stream);
   });
   configuration_parser_.registerKeyword("debug", [this](std::istream& stream) {
      configuration_.debug = commonItems::getString(stream) == "yes";
   });
   configuration_parser_.registerKeyword("output_name", [this](std::istream& stream) {
      configuration_.output_name = commonItems::getString(stream);
   });
}


configuration::Configuration configuration::ConfigurationImporter::ImportConfiguration(
    std::string_view configuration_file)
{
   configuration_parser_.parseFile(configuration_file);

   if (configuration_.output_name.empty())
   {
      configuration_.output_name = DetermineOutputName(configuration_.save_game);
   }
   configuration_.output_name = normalizeStringPath(configuration_.output_name);

   Log(LogLevel::Info) << "Using output name " << configuration_.output_name;

   return configuration_;
}