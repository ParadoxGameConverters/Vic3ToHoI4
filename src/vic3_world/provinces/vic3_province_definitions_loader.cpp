#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



vic3::ProvinceDefinitionsLoader::ProvinceDefinitionsLoader()
{
   parser_.registerRegex("x[0-9A-F]{6}", [this](const std::string& province_color, std::istream& input_stream) {
      commonItems::ignoreItem(province_color, input_stream);
      province_definitions_.push_back(province_color);
   });
   parser_.IgnoreAndLogUnregisteredItems();
}


vic3::ProvinceDefinitions vic3::ProvinceDefinitionsLoader::LoadProvinceDefinitions(
    const commonItems::ModFilesystem& mod_filesystem)
{
   province_definitions_.clear();

   const auto& terrains_file = mod_filesystem.GetActualFileLocation("/game/map_data/province_terrains.txt");
   if (!terrains_file.has_value())
   {
      throw std::runtime_error("Vic3 install did not contain /game/map_data/province_terrains.txt");
   }
   parser_.parseFile(*terrains_file);

   return ProvinceDefinitions(province_definitions_);
}