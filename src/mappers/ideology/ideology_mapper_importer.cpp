#include "src/mappers/ideology/ideology_mapper_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"



mappers::IdeologyMapper mappers::ImportIdeologyMapper(std::string_view mapping_file)
{
   ItemToPointsMap rules;

   IdeologyPointsMap ideology_points_map;

   commonItems::parser rule_parser;
   rule_parser.registerRegex(commonItems::catchallRegex,
       [&ideology_points_map](const std::string& ideology, std::istream& input_stream) {
          ideology_points_map.emplace(ideology, commonItems::getInt(input_stream));
       });

   commonItems::parser file_parser;
   file_parser.registerRegex(commonItems::catchallRegex,
       [&rule_parser, &rules, &ideology_points_map](const std::string& law, std::istream& input_stream) {
          ideology_points_map.clear();
          rule_parser.parseStream(input_stream);
          rules.emplace(law, ideology_points_map);
       });

   file_parser.parseFile(mapping_file); 

   return IdeologyMapper(rules, {});
}