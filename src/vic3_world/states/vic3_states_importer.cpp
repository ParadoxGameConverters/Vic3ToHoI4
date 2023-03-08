#include "src/vic3_world/states/vic3_states_importer.h"

#include <sstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



std::map<int, vic3::State> vic3::ImportStates(std::istream& input_stream)
{
   std::map<int, State> states;

   StateImporter state_importer;

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&state_importer, &states](const std::string& number_string, std::istream& input_stream) {
          const int state_number = std::stoi(number_string);
          const auto state_string = commonItems::stringOfItem(input_stream).getString();
          if (state_string.find("{") == std::string::npos)
          {
             return;
          }
          std::istringstream state_stream(state_string);
          states.emplace(state_number, state_importer.ImportState(state_stream));
       });

   commonItems::parser states_parser;
   states_parser.registerKeyword("database", [&database_parser](std::istream& input_stream) {
      database_parser.parseStream(input_stream);
   });
   states_parser.IgnoreUnregisteredItems();

   states_parser.parseStream(input_stream);
   return states;
}