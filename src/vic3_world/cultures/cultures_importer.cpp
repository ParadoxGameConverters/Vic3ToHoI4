#include "src/vic3_world/cultures/cultures_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Log.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>



namespace vic3
{

std::map<int, Culture> ImportCultures(std::istream& input_stream)
{
   std::map<int, Culture> cultures;

   int active_cultures = 0;

   Culture culture;

   commonItems::parser culture_parser;
   // Guaranteed fields
   // name: type = ashkenazi
   // seed: random_seed = ####

   // Optional fields
   // homelands: core_states = { state_names }
   // migration targets: migration = { migration_struct }
   // fomenting migrations: potential_migration = { potential_migration_struct }
   // cultural obsessions: obsessions = { good_names }

   culture_parser.registerKeyword("type", [&culture](std::istream& input_stream) {
      culture.name = commonItems::getString(input_stream);
   });
   culture_parser.registerKeyword("core_states", [&culture](std::istream& input_stream) {
      std::vector<std::string> homeland_list = commonItems::getStrings(input_stream);
      culture.homelands.insert(homeland_list.begin(), homeland_list.end());
   });
   culture_parser.IgnoreUnregisteredItems();

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&cultures, &active_cultures, &culture, &culture_parser](const std::string& number_string,
           std::istream& input_stream) {
          const int culture_number = std::stoi(number_string);
          const std::string culture_string = commonItems::stringOfItem(input_stream).getString();
          if (culture_string.find('{') == std::string::npos)
          {
             Log(LogLevel::Error) << "Broken culture definition in save file. This should not happen.";
             return;
          }

          culture.name.clear();
          culture.homelands.clear();

          std::istringstream culture_stream(culture_string);
          culture_parser.parseStream(culture_stream);

          ++active_cultures;
          cultures.emplace(culture_number, culture);
       });
   database_parser.IgnoreUnregisteredItems();


   commonItems::parser cultures_parser;
   cultures_parser.registerKeyword("database", [&database_parser](std::istream& input_stream) {
      database_parser.parseStream(input_stream);
   });
   cultures_parser.registerKeyword("dead", [](std::istream& input_stream) {
      const std::string culture_string = commonItems::stringOfItem(input_stream).getString();
      const std::regex non_empty(R"(\{[\s\S]*[\S]+[\s\S]*\})");
      if (std::regex_search(culture_string, non_empty))
      {
         Log(LogLevel::Info) << "Dead culture definitions found.";
      }
   });
   cultures_parser.IgnoreUnregisteredItems();
   cultures_parser.parseStream(input_stream);

   LOG(LogLevel::Info) << fmt::format("\tImported {} active cultures.", active_cultures);

   return cultures;
}

}  // namespace vic3