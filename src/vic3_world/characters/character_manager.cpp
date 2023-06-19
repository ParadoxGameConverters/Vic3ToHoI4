#include "src/vic3_world/characters/character_manager.h"

#include <ranges>

#include "external/commonItems/ParserHelpers.h"
#include "external/fmt/include/fmt/format.h"
#include "src/vic3_world/characters/characters_importer.h"
#include "src/vic3_world/characters/country_character_map_importer.h"

namespace
{
std::map<int, std::string> ImportExileOriginMap(std::istream& input_stream)
{
   std::map<int, std::string> exile_origin_map;
   commonItems::parser map_parser;
   map_parser.registerRegex(commonItems::integerRegex,
       [&exile_origin_map](const std::string& number_string, std::istream& input_stream) {
          const int exile_id = std::stoi(number_string);
          exile_origin_map.emplace(exile_id, commonItems::getString(input_stream));
       });
   map_parser.IgnoreUnregisteredItems();
   map_parser.parseStream(input_stream);


   Log(LogLevel::Info) << fmt::format("\tFound home countries of {} exiles.", exile_origin_map.size());
   return exile_origin_map;
}
std::map<int, int> ImportCharacterIgMap(std::istream& input_stream)
{
   std::map<int, int> character_ig_map;
   commonItems::parser map_parser;
   map_parser.registerRegex(commonItems::integerRegex,
       [&character_ig_map](const std::string& number_string, std::istream& input_stream) {
          const int character_id = std::stoi(number_string);
          character_ig_map.emplace(character_id, commonItems::getInt(input_stream));
       });
   map_parser.IgnoreUnregisteredItems();
   map_parser.parseStream(input_stream);

   // Everyone but homeless exilees.
   Log(LogLevel::Info) << fmt::format("\tFound IGs for {} characters.", character_ig_map.size());
   return character_ig_map;
}
}  // namespace

vic3::CharacterManager::CharacterManager(std::istream& input_stream)
{
   character_manager_parser_.registerKeyword("database", [this](std::istream& input_stream) {
      characters_ = ImportCharacters(input_stream);
   });
   character_manager_parser_.registerKeyword("exile_origin_map", [this](std::istream& input_stream) {
      country_character_map_ = ImportCountryCharacterMap(input_stream);
   });
   character_manager_parser_.registerKeyword("exile_country_map", [this](std::istream& input_stream) {
      exile_origin_map_ = ImportExileOriginMap(input_stream);
   });
   character_manager_parser_.registerKeyword("character_ig_map", [this](std::istream& input_stream) {
      character_ig_map_ = ImportCharacterIgMap(input_stream);
   });
   character_manager_parser_.IgnoreUnregisteredItems();
   character_manager_parser_.parseStream(input_stream);

   AssignHomeTagToExiles();
}

void vic3::CharacterManager::AssignHomeTagToExiles()
{
   for (auto& character: characters_ | std::views::values)
   {
      if (!character.GetRoles().contains("agitator"))
      {
         continue;
      }

      if (const auto itr = exile_origin_map_.find(character.GetId()); itr != exile_origin_map_.end())
      {
         character.SetHomeTag(itr->second);
      }
      else
      {
         Log(LogLevel::Warning) << fmt::format("Agitator {} {} with ID: {} has no home country.",
             character.GetFirstName(),
             character.GetLastName(),
             character.GetId());
      }
   }
}