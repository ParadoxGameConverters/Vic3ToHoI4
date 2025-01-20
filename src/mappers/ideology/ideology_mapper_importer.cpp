#include "src/mappers/ideology/ideology_mapper_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>



mappers::IdeologyMapper mappers::ImportIdeologyMapper(const std::filesystem::path& mapping_file)
{
   ItemToPointsMap rules;
   std::map<std::string, ItemToPointsMap> sub_ideology_rules;

   IdeologyPointsMap ideology_points_map;
   ItemToPointsMap current_sub_ideology_rules;

   commonItems::parser rule_parser;
   rule_parser.registerRegex(commonItems::catchallRegex,
       [&ideology_points_map](const std::string& ideology, std::istream& input_stream) {
          ideology_points_map.emplace(ideology, commonItems::getInt(input_stream));
       });

   commonItems::parser sub_ideology_parser;
   sub_ideology_parser.registerRegex(commonItems::catchallRegex,
       [&rule_parser, &current_sub_ideology_rules, &ideology_points_map](const std::string& law,
           std::istream& input_stream) {
          ideology_points_map.clear();
          rule_parser.parseStream(input_stream);
          current_sub_ideology_rules.emplace(law, ideology_points_map);
       });

   commonItems::parser sub_ideologies_parser;
   sub_ideologies_parser.registerRegex(commonItems::catchallRegex,
       [&sub_ideology_rules, &current_sub_ideology_rules, &sub_ideology_parser](const std::string& ideology,
           std::istream& input_stream) {
          current_sub_ideology_rules.clear();
          sub_ideology_parser.parseStream(input_stream);
          sub_ideology_rules.emplace(ideology, current_sub_ideology_rules);
       });

   commonItems::parser file_parser;
   file_parser.registerKeyword("sub_ideologies",
       [&sub_ideologies_parser, &sub_ideology_rules]([[maybe_unused]] const std::string& unused, std::istream& input_stream) {
          sub_ideology_rules.clear();
          sub_ideologies_parser.parseStream(input_stream);
       });
   file_parser.registerRegex(commonItems::catchallRegex,
       [&rule_parser, &rules, &ideology_points_map](const std::string& law, std::istream& input_stream) {
          ideology_points_map.clear();
          rule_parser.parseStream(input_stream);
          rules.emplace(law, ideology_points_map);
       });

   file_parser.parseFile(mapping_file);

   return IdeologyMapper(rules, {sub_ideology_rules});
}