#include "src/vic3_world/laws/laws_importer.h"

#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"



std::map<int, std::set<std::string>> vic3::ImportLaws(std::istream& input_stream)
{
   std::map<int, std::set<std::string>> laws;

   int empty_laws = 0;
   int inactive_laws = 0;
   int active_laws = 0;

   bool law_is_active = false;
   std::string law_name;
   std::optional<int> country_number;

   commonItems::parser law_parser;
   law_parser.registerKeyword("active", [&law_is_active](std::istream& input_stream) {
      if (commonItems::getString(input_stream) == "yes")
      {
         law_is_active = true;
      }
      else
      {
         LOG(LogLevel::Warning) << fmt::format(
             "Law had odd active block. Please report to the converters team and upload your save.");
      }
   });
   law_parser.registerKeyword("law", [&law_name](std::istream& input_stream) {
      law_name = commonItems::getString(input_stream);
   });
   law_parser.registerKeyword("country", [&country_number](std::istream& input_stream) {
      country_number = commonItems::getInt(input_stream);
   });
   law_parser.IgnoreUnregisteredItems();


   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&laws, &empty_laws, &law_is_active, &inactive_laws, &active_laws, &law_name, &country_number, &law_parser](
           const std::string& number_string,
           std::istream& input_stream) {
          const int law_number = std::stoi(number_string);
          const std::string law_string = commonItems::stringOfItem(input_stream).getString();
          if (law_string.find("{") == std::string::npos)
          {
             ++empty_laws;
             return;
          }

          law_is_active = false;
          law_name.clear();
          country_number.reset();

          std::istringstream law_stream(law_string);
          law_parser.parseStream(law_stream);
          if (!law_is_active)
          {
             ++inactive_laws;
             return;
          }
          if (!country_number)
          {
             LOG(LogLevel::Warning)
                 << "Law without a country. Please report to the converters team and upload your save.";
          }

          ++active_laws;
          auto [itr, success] = laws.emplace(*country_number, std::set{law_name});
          if (!success)
          {
             itr->second.emplace(law_name);
          }
       });
   database_parser.IgnoreUnregisteredItems();

   commonItems::parser laws_parser;
   laws_parser.registerKeyword("database", [&database_parser](std::istream& input_stream) {
      database_parser.parseStream(input_stream);
   });
   laws_parser.IgnoreUnregisteredItems();
   laws_parser.parseStream(input_stream);

   LOG(LogLevel::Info) << fmt::format("\tImported {} active laws.", active_laws);
   LOG(LogLevel::Info) << fmt::format("\tImported {} inactive laws.", inactive_laws);
   LOG(LogLevel::Info) << fmt::format("\tImported {} empty laws.", empty_laws);

   return laws;
}