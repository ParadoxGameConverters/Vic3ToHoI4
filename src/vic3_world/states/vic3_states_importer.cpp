#include "src/vic3_world/states/vic3_states_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/ParserHelpers.h"



vic3::StatesImporter::StatesImporter(bool debug): state_importer_(debug)
{
   states_parser_.registerKeyword("database", [this](std::istream& input_stream) {
      database_parser_.parseStream(input_stream);
   });
   if (debug)
   {
      states_parser_.registerRegex(commonItems::catchallRegex, commonItems::ignoreAndLogItem);
   }
   else
   {
      states_parser_.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
   }

   database_parser_.registerRegex(commonItems::integerRegex,
       [this](const std::string& number_string, std::istream& input_stream) {
          const int state_number = std::stoi(number_string);
          states_.emplace(state_number, state_importer_.ImportState(input_stream));
       });
}


std::map<int, vic3::State> vic3::StatesImporter::ImportStates(std::istream& input_stream)
{
   states_.clear();
   states_parser_.parseStream(input_stream);
   return states_;
}