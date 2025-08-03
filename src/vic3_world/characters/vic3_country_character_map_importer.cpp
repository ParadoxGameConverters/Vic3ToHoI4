#include "src/vic3_world/characters/vic3_country_character_map_importer.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Log.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>



std::map<int, std::vector<int>> vic3::ImportCountryCharacterMap(std::istream& input_stream)
{
   std::map<int, std::vector<int>> country_character_map;

   int characters = 0;

   commonItems::parser map_parser;
   map_parser.registerRegex(commonItems::integerRegex,
       [&country_character_map, &characters](const std::string& number_string, std::istream& input_stream) {
          const int country_id = static_cast<int>(std::stoul(number_string));
          country_character_map.emplace(country_id, commonItems::getInts(input_stream));
          characters += static_cast<int>(country_character_map.at(country_id).size());
       });
   map_parser.IgnoreUnregisteredItems();
   map_parser.parseStream(input_stream);


   Log(LogLevel::Info) << fmt::format("\tMatched {} countries to {} characters.",
       country_character_map.size(),
       characters);

   return country_character_map;
}