#include "src/vic3_world/elections/elections_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Log.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>



std::map<int, date> vic3::ImportElections(std::istream& input_stream)
{
   std::map<int, date> elections;

   int empty_elections = 0;
   int democracies = 0;

   date last_election;
   std::optional<int> country_number;

   commonItems::parser election_parser;
   election_parser.registerKeyword("last_election", [&last_election](std::istream& input_stream) {
      last_election = date(commonItems::getString(input_stream));
   });
   election_parser.registerKeyword("country", [&country_number](std::istream& input_stream) {
      country_number = commonItems::getInt(input_stream);
   });
   election_parser.IgnoreUnregisteredItems();

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&elections, &empty_elections, &democracies, &last_election, &country_number, &election_parser](
           const std::string& number_string,
           std::istream& input_stream) {
          const std::string election_string = commonItems::stringOfItem(input_stream).getString();
          if (election_string.find("{") == std::string::npos)
          {
             ++empty_elections;
             return;
          }

          last_election = date{};
          country_number.reset();

          std::istringstream election_stream(election_string);
          election_parser.parseStream(election_stream);

          if (!country_number)
          {
             LOG(LogLevel::Warning)
                 << "Election without a country. Please report to the converters team and upload your save.";
          }

          ++democracies;
          elections.emplace(*country_number, last_election);
       });
   database_parser.IgnoreUnregisteredItems();

   commonItems::parser elections_parser;
   elections_parser.registerKeyword("database", [&database_parser](std::istream& input_stream) {
      database_parser.parseStream(input_stream);
   });
   elections_parser.IgnoreUnregisteredItems();
   elections_parser.parseStream(input_stream);

   LOG(LogLevel::Info) << fmt::format("\tImported {} democracies.", democracies);
   LOG(LogLevel::Info) << fmt::format("\tImported {} democratic backslides.", empty_elections);

   return elections;
}
