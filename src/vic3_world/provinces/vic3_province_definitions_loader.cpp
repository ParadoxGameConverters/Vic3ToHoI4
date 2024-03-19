#include "src/vic3_world/provinces/vic3_province_definitions_loader.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



vic3::ProvinceDefinitions vic3::LoadProvinceDefinitions()
{
   std::vector<std::string> province_definitions;

   commonItems::parser parser;
   parser.registerRegex("[xX][0-9a-fA-F]{6}",
       [&province_definitions](const std::string& province_color, std::istream& input_stream) {
          commonItems::ignoreItem(province_color, input_stream);
          std::string updated_color = province_color;
          std::transform(++updated_color.begin(), updated_color.end(), ++updated_color.begin(), ::toupper);
          updated_color[0] = 'x';
          province_definitions.push_back(updated_color);
       });
   parser.IgnoreAndLogUnregisteredItems();

   parser.parseFile("configurables/province_terrains.txt");

   return ProvinceDefinitions(province_definitions);
}