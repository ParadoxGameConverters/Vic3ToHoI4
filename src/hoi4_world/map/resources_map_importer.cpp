#include "src/hoi4_world/map/resources_map_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"



hoi4::ResourcesMap hoi4::ImportResources(std::string_view resources_file)
{
   ResourcesMap resources_map;
   std::optional<Province> province;
   Resources resources;

   commonItems::parser resource_parser;
   resource_parser.registerRegex(commonItems::stringRegex,
       [&resources](const std::string& resource_name, std::istream& input_stream) {
          resources.emplace(resource_name, commonItems::getDouble(input_stream));
       });
   resource_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

   commonItems::parser resources_parser;
   resources_parser.registerKeyword("province", [&province](std::istream& input_stream) {
      province = commonItems::getInt(input_stream);
   });
   resources_parser.registerKeyword("resources", [&resource_parser](std::istream& input_stream) {
      resource_parser.parseStream(input_stream);
   });
   resources_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

   commonItems::parser resources_map_parser;
   resources_map_parser.registerKeyword("link",
       [&resources_map, &resources_parser, &province, &resources](std::istream& input_stream) {
          province.reset();
          resources.clear();
          resources_parser.parseStream(input_stream);
          if (province)
          {
             resources_map.emplace(*province, resources);
          }
       });
   resources_map_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

   resources_map_parser.parseFile(resources_file);

   return resources_map;
}