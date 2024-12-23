#include "src/vic3_world/military/military_formations_importer.h"

#include <external/commonItems/ParserHelpers.h>

#include "src/vic3_world/database/database_parser.h"



std::map<int, vic3::MilitaryFormation> vic3::ImportMilitaryFormations(std::istream& input)
{
   std::map<int, vic3::MilitaryFormation> military_formations;

   MilitaryFormationImporter formation_importer;

   const auto& parser_function = [&formation_importer, &military_formations](const std::string& number_string,
                                     std::istream& input_stream) {
      const int formation_number = std::stoi(number_string);
      if (std::optional<MilitaryFormation> formation = formation_importer.ImportMilitaryFormation(input_stream);
          formation.has_value())
      {
         military_formations.emplace(formation_number, *formation);
      }
   };

   DatabaseParser parser(parser_function);
   parser.registerKeyword("dead", commonItems::ignoreItem);
   parser.registerKeyword("front_to_distribution_data_map", commonItems::ignoreItem);
   parser.registerKeyword("front_to_distribution_counter_map", commonItems::ignoreItem);
   parser.parseStream(input);

   return military_formations;
}