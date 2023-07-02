#include "src/vic3_world/pacts/pacts_importer.h"

#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "src/vic3_world/pacts/pact_importer.h"



std::map<int, vic3::Pact> vic3::ImportPacts(std::istream& input_stream)
{
   std::map<int, Pact> pacts;
   PactImporter pact_importer;

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&pact_importer, &pacts](const std::string& number_string, std::istream& input_stream) {
          const int id = std::stoi(number_string);
          const std::string pact_string = commonItems::stringOfItem(input_stream).getString();
          if (pact_string.find('{') == std::string::npos)
          {
             Log(LogLevel::Error) << "Broken pact in save file. This should not happen.";
             return;
          }
          std::istringstream pact_stream(pact_string);

          pacts.emplace(id, pact_importer.ImportPact(id, pact_stream));
       });
   database_parser.IgnoreUnregisteredItems();

   commonItems::parser pacts_parser;
   pacts_parser.registerKeyword("database", [&pacts, &database_parser](std::istream& input_stream) {
      pacts.clear();
      database_parser.parseStream(input_stream);
   });
   pacts_parser.IgnoreUnregisteredItems();
   pacts_parser.parseStream(input_stream);

   Log(LogLevel::Info) << fmt::format("\tImported {} diplomatic pacts.", pacts.size());

   return pacts;
}