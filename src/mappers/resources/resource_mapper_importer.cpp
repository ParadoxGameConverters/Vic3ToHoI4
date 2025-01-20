#include "src/mappers/resources/resource_mapper_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>



mappers::ResourceMapper mappers::ImportResourceMapper(const std::filesystem::path& mapping_file)
{
   std::map<std::string, mappers::ResourceScore> scores;
   mappers::ResourceScore current;


   commonItems::parser base_parser;
   base_parser.registerRegex(commonItems::catchallRegex,
       [&current](const std::string& key, std::istream& input_stream) {
          current.base.emplace(key, static_cast<float>(commonItems::getDouble(input_stream)));
       });

   commonItems::parser score_parser;
   score_parser.registerKeyword("base", [&base_parser](const std::string& key, std::istream& input_stream) {
      base_parser.parseStream(input_stream);
   });
   score_parser.registerKeyword("bonus", [&current](std::istream& input_stream) {
      current.bonus = commonItems::getDouble(input_stream);
   });
   score_parser.registerKeyword("total", [&current](std::istream& input_stream) {
      current.total = commonItems::getDouble(input_stream);
   });
   score_parser.registerKeyword("bonus_building", [&current](std::istream& input_stream) {
      current.bonus_buildings.emplace(commonItems::getString(input_stream));
   });

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&score_parser, &scores, &current](const std::string& resource, std::istream& input_stream) {
          current.base.clear();
          current.bonus_buildings.clear();
          current.bonus = 0;
          score_parser.parseStream(input_stream);
          scores.emplace(resource, current);
       });

   file_parser.parseFile(mapping_file);
   return mappers::ResourceMapper(scores);
}
