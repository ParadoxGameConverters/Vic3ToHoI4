#include "src/vic3_world/characters/vic3_characters_importer.h"

#include "external/commonItems/Log.h"
#include "external/commonItems/Parser.h"
#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "src/vic3_world/characters/vic3_character_importer.h"



std::map<int, vic3::Character> vic3::ImportCharacters(std::istream& input_stream)
{
   std::map<int, Character> characters;
   CharacterImporter character_importer;

   commonItems::parser database_parser;
   database_parser.registerRegex(commonItems::integerRegex,
       [&character_importer, &characters](const std::string& number_string, std::istream& input_stream) {
          const int id = std::stoi(number_string);
          const std::string character_string = commonItems::stringOfItem(input_stream).getString();
          if (character_string.find('{') == std::string::npos)
          {
             return;
          }
          std::istringstream character_stream(character_string);

          characters.emplace(id, character_importer.ImportCharacter(id, character_stream));
       });
   database_parser.IgnoreUnregisteredItems();
   database_parser.parseStream(input_stream);


   Log(LogLevel::Info) << fmt::format("\tImported {} living characters.", characters.size());

   return characters;
}