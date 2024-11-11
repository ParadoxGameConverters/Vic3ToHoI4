#include "src/vic3_world/interest_groups/interest_groups_importer.h"

#include "external/commonItems/CommonRegexes.h"
#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "src/vic3_world/interest_groups/interest_group_importer.h"



std::map<int, vic3::InterestGroup> vic3::ImportInterestGroups(std::istream& input_stream)
{
   std::map<int, InterestGroup> igs;
   InterestGroupImporter ig_importer;

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&ig_importer, &igs](const std::string& number_string, std::istream& input_stream) {
          const int id = std::stoi(number_string);
          const std::string ig_string = commonItems::stringOfItem(input_stream).getString();
          if (ig_string.find('{') == std::string::npos)
          {
             return;
          }
          std::istringstream ig_stream(ig_string);

          igs.emplace(id, ig_importer.ImportInterestGroup(id, ig_stream));
       });
   database_parser.IgnoreUnregisteredItems();

   commonItems::parser igs_parser;
   igs_parser.registerKeyword("database", [&igs, &database_parser](std::istream& input_stream) {
      igs.clear();
      database_parser.parseStream(input_stream);
   });
   igs_parser.IgnoreUnregisteredItems();
   igs_parser.parseStream(input_stream);

   Log(LogLevel::Info) << fmt::format("\tImported {} IGs.", igs.size());

   return igs;
}