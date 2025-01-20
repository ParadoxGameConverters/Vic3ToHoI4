#include "src/mappers/character/leader_type_mapper_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>



mappers::LeaderTypeMapper mappers::ImportLeaderTypeMapper(const std::filesystem::path& mapping_file)
{
   GovernancePowerMap governance_power_map;
   PowerLeaderMap power_leader_map;
   LeaderType leader_type;

   commonItems::parser power_parser;
   power_parser.registerRegex(commonItems::catchallRegex,
       [&power_leader_map, &leader_type](const std::string& law, [[maybe_unused]] std::istream& input_stream) {
          power_leader_map.emplace(law, leader_type);
       });

   commonItems::parser leader_type_parser;
   leader_type_parser.registerKeyword("council",
       [&leader_type, &power_parser](const std::string& type, std::istream& input_stream) {
          leader_type = type;
          power_parser.parseStream(input_stream);
       });
   leader_type_parser.registerKeyword("prime_minister",
       [&leader_type, &power_parser](const std::string& type, std::istream& input_stream) {
          leader_type = type;
          power_parser.parseStream(input_stream);
       });
   leader_type_parser.IgnoreAndLogUnregisteredItems();

   commonItems::parser governance_parser;
   governance_parser.registerRegex(commonItems::catchallRegex,
       [&leader_type_parser, &governance_power_map, &power_leader_map](const std::string& law,
           std::istream& input_stream) {
          power_leader_map.clear();
          leader_type_parser.parseStream(input_stream);
          governance_power_map.emplace(law, power_leader_map);
       });

   governance_parser.parseFile(mapping_file);

   return LeaderTypeMapper(governance_power_map);
}