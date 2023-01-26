#include "src/vic3_world/states/vic3_states_importer.h"

#include <sstream>

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



vic3::StatesImporter::StatesImporter()
{
   states_parser_.registerKeyword("database", [this](std::istream& input_stream) {
      database_parser_.parseStream(input_stream);
   });
   states_parser_.IgnoreUnregisteredItems();

   database_parser_.registerRegex(commonItems::integerRegex,
       [this](const std::string& number_string, std::istream& input_stream) {
          const int state_number = std::stoi(number_string);
          const auto state_string = commonItems::stringOfItem(input_stream).getString();
          if (state_string.find("none") != std::string::npos)
          {
             return;
          }
          std::istringstream state_stream(state_string);
          states_.emplace(state_number, state_importer_.ImportState(state_stream));
       });
}


std::map<int, vic3::State> vic3::StatesImporter::ImportStates(std::istream& input_stream)
{
   states_.clear();
   states_parser_.parseStream(input_stream);
   return states_;
}