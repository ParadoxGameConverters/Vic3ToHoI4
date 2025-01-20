#include "src/vic3_world/laws/laws_importer.h"

#include <external/commonItems/Log.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>
#include <src/vic3_world/database/database_parser.h>



std::map<int, std::set<std::string>> vic3::ImportLaws(std::istream& input_stream)
{
   std::map<int, std::set<std::string>> laws;

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

   const auto law_parser_function = [&laws,
                                        &law_is_active,
                                        &inactive_laws,
                                        &active_laws,
                                        &law_name,
                                        &country_number,
                                        &law_parser](std::istream& input_stream) {
      law_is_active = false;
      law_name.clear();
      country_number.reset();
      law_parser.parseStream(input_stream);
      if (!law_is_active)
      {
         ++inactive_laws;
         return;
      }
      if (!country_number)
      {
         LOG(LogLevel::Warning) << "Law without a country. Please report to the converters team and upload your save.";
         return;
      }

      ++active_laws;
      auto [itr, success] = laws.emplace(*country_number, std::set{law_name});
      if (!success)
      {
         itr->second.emplace(law_name);
      }
   };

   DatabaseParser database_parser(law_parser_function);
   database_parser.registerKeyword("dead", commonItems::ignoreItem);
   database_parser.parseStream(input_stream);

   LOG(LogLevel::Info) << fmt::format("\tImported {} active laws.", active_laws);
   LOG(LogLevel::Info) << fmt::format("\tImported {} inactive laws.", inactive_laws);

   return laws;
}