#include "src/vic3_world/states/vic3_states_importer.h"

#include <sstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"
#include "src/vic3_world/database/database_parser.h"



std::map<int, vic3::State> vic3::ImportStates(std::istream& input_stream)
{
   std::map<int, State> states;
   StateImporter state_importer;

   const auto& parser_func = [&state_importer, &states](const std::string& number_string, std::istream& input_stream) {
      const int state_number = std::stoi(number_string);
      states.emplace(state_number, state_importer.ImportState(number_string, input_stream));
   };

   DatabaseParser parser(parser_func);

   parser.parseStream(input_stream);
   return states;
}