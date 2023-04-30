#include "src/mappers/ideology/ideology_mapper_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"



mappers::IdeologyMapper mappers::ImportIdeologyMapper(std::string_view mapping_file)
{
   std::map<std::string, IdeologyPointsMap> rules;

   std::string law;
   IdeologyPointsMap ideology_points_map;

   commonItems::parser rule_parser;
   rule_parser.registerKeyword("law", [&law](std::istream& input_stream) {
      law = commonItems::getString(input_stream);
   });
   rule_parser.registerRegex(commonItems::catchallRegex,
       [&ideology_points_map](const std::string& ideology, std::istream& input_stream) {
          ideology_points_map.emplace(ideology, commonItems::getInt(input_stream));
       });

   commonItems::parser file_parser;
   file_parser.registerKeyword("link", [&rule_parser, &rules, &law, &ideology_points_map](std::istream& input_stream) {
      law.clear();
      ideology_points_map.clear();
      rule_parser.parseStream(input_stream);

      if (law.empty())
      {
         return;
      }

      rules.emplace(law, ideology_points_map);
   });

   file_parser.parseFile(mapping_file);

   return IdeologyMapper(rules);
}