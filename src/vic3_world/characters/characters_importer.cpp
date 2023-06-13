#include "src/vic3_world/characters/characters_importer.h"

#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"



std::map<int, vic3::Character> vic3::ImportCharacters(std::istream& input_stream)
{
   std::map<int, Character> characters;

   int living_characters = 0;

   std::string first_name;
   std::string last_name;
   int culture_id;
   std::set<std::string> roles;
   int rank = 0;
   std::string ideology;
   std::set<std::string> traits;

   commonItems::parser character_parser;

   character_parser.registerKeyword("first_name", [&first_name](std::istream& input_stream) {
      first_name = commonItems::getString(input_stream);
   });
   character_parser.registerKeyword("last_name", [&last_name](std::istream& input_stream) {
      last_name = commonItems::getString(input_stream);
   });
   character_parser.registerKeyword("culture", [&culture_id](std::istream& input_stream) {
      culture_id = commonItems::getInt(input_stream);
   });
   character_parser.registerKeyword("role", [&roles](std::istream& input_stream) {
      roles.emplace(commonItems::getString(input_stream));
   });
   character_parser.registerKeyword("rank", [&rank](std::istream& input_stream) {
      try
      {
         rank = std::stoi(std::string(1, commonItems::getString(input_stream).back()));
      }
      catch (const std::exception& e)
      {
         Log(LogLevel::Warning) << fmt::format("Failed to read rank: {}", e.what());
      }
   });
   character_parser.registerKeyword("ideology", [&ideology](std::istream& input_stream) {
      ideology = commonItems::getString(input_stream);
   });
   character_parser.registerKeyword("traits", [&traits](std::istream& input_stream) {
      const auto traits_vec = commonItems::getStrings(input_stream);
      std::ranges::copy(traits_vec, std::inserter(traits, traits.end()));
   });
   character_parser.IgnoreUnregisteredItems();

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&characters,
           &living_characters,
           &first_name,
           &last_name,
           &culture_id,
           &roles,
           &rank,
           &ideology,
           &traits,
           &character_parser](const std::string& number_string, std::istream& input_stream) {
          const int id = std::stoi(number_string);
          const std::string character_string = commonItems::stringOfItem(input_stream).getString();
          if (character_string.find('{') == std::string::npos)
          {
             Log(LogLevel::Error) << "Broken culture definition in save file. This should not happen.";
             return;
          }

          first_name.clear();
          last_name.clear();
          culture_id = 0;
          roles.clear();
          ideology.clear();
          traits.clear();

          std::istringstream character_stream(character_string);
          character_parser.parseStream(character_stream);

          ++living_characters;
          characters.emplace(id, Character(id, first_name, last_name, culture_id, roles, rank, ideology, traits));
       });
   database_parser.IgnoreUnregisteredItems();


   commonItems::parser characters_parser;
   characters_parser.registerKeyword("database", [&database_parser](std::istream& input_stream) {
      database_parser.parseStream(input_stream);
   });
   characters_parser.IgnoreUnregisteredItems();
   characters_parser.parseStream(input_stream);

   Log(LogLevel::Info) << fmt::format("\tImported {} living characters.", living_characters);

   return characters;
}