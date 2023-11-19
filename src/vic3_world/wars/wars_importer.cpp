#include "src/vic3_world/wars/wars_importer.h"

#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "src/vic3_world/wars/war_importer.h"


std::vector<vic3::War> vic3::ImportWars(std::istream& input_stream)
{
   std::vector<War> wars;
   WarImporter war_importer;

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&war_importer, &wars](const std::string& unused, std::istream& input_stream) {
          const std::string war_string = commonItems::stringOfItem(input_stream).getString();
          if (war_string.find('{') == std::string::npos)
          {
             return;
          }
          std::istringstream war_stream(war_string);

          if (const std::optional<War> possible_war = war_importer.ImportWar(war_stream); possible_war)
          {
             wars.emplace_back(*possible_war);
          }
       });
   database_parser.IgnoreUnregisteredItems();

   commonItems::parser wars_parser;
   wars_parser.registerKeyword("database", [&wars, &database_parser](std::istream& input_stream) {
      wars.clear();
      database_parser.parseStream(input_stream);
   });
   wars_parser.IgnoreUnregisteredItems();
   wars_parser.parseStream(input_stream);

   Log(LogLevel::Info) << fmt::format("\tImported {} wars.", wars.size());

   return wars;
}