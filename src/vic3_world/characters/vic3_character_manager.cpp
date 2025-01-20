#include "src/vic3_world/characters/vic3_character_manager.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Log.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>

#include <ranges>

#include "src/vic3_world/characters/vic3_characters_importer.h"
#include "src/vic3_world/characters/vic3_country_character_map_importer.h"



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

   // Everyone but homeless exiles.
   Log(LogLevel::Info) << fmt::format("\tFound IGs for {} characters.", character_ig_map.size());
   return character_ig_map;
}
std::set<int> ImportExilePool(std::istream& input_stream)
{
   std::set<int> exile_pool;
   commonItems::parser map_parser;
   map_parser.registerRegex(commonItems::integerRegex,
       [&exile_pool](const std::string& number_string, std::istream& input_stream) {
          exile_pool.emplace(std::stoi(number_string));
          commonItems::ignoreItem("interest_group", input_stream);
       });
   map_parser.IgnoreUnregisteredItems();
   map_parser.parseStream(input_stream);

   // Homeless exiles.
   Log(LogLevel::Info) << fmt::format("\tFound {} homeless agitators.", exile_pool.size());
   return exile_pool;
}

//// For backwards compatibility with pre 1.5 Vic - uses HQs
std::set<int> ImportEmployedCommanders(std::istream& input_stream)
{
   std::set<int> hired_commanders;
   commonItems::parser map_parser;
   map_parser.registerRegex(commonItems::integerRegex,
       [&hired_commanders]([[maybe_unused]] const std::string& hq_number_string, std::istream& input_stream) {
          std::ranges::copy(commonItems::getInts(input_stream),
              std::inserter(hired_commanders, hired_commanders.begin()));
       });
   map_parser.IgnoreUnregisteredItems();
   map_parser.parseStream(input_stream);

   Log(LogLevel::Info) << fmt::format("\tFound {} commanders employed in HQs.", hired_commanders.size());
   return hired_commanders;
}
//

int ImportObituary(std::istream& input_stream)
{
   int obituary = 0;
   commonItems::parser map_parser;
   map_parser.registerKeyword("object", [&obituary](std::istream& input_stream) {
      obituary = commonItems::getInt(input_stream);
   });
   map_parser.IgnoreUnregisteredItems();
   map_parser.parseStream(input_stream);

   return obituary;
}
std::set<int> ImportObituaries(std::istream& input_stream)
{
   std::set<int> obituaries;
   const auto& blobs = commonItems::blobList(input_stream);
   for (const std::string& blob: blobs.getBlobs())
   {
      auto obit_stream = std::stringstream(blob);
      obituaries.insert(ImportObituary(obit_stream));
   }
   Log(LogLevel::Info) << fmt::format("\tFound {} dead characters. Hiding the bodies.", obituaries.size());
   return obituaries;
}
}  // namespace

vic3::CharacterManager::CharacterManager(std::istream& input_stream)
{
   character_manager_parser_.registerKeyword("database", [this](std::istream& input_stream) {
      characters_ = ImportCharacters(input_stream);
   });
   character_manager_parser_.registerKeyword("country_character_map", [this](std::istream& input_stream) {
      country_character_map_ = ImportCountryCharacterMap(input_stream);
   });
   character_manager_parser_.registerKeyword("exile_country_map", [this](std::istream& input_stream) {
      exile_origin_map_ = ImportExileOriginMap(input_stream);
   });
   character_manager_parser_.registerKeyword("character_ig_map", [this](std::istream& input_stream) {
      character_ig_map_ = ImportCharacterIgMap(input_stream);
   });
   character_manager_parser_.registerKeyword("home_hq_character_map", [this](std::istream& input_stream) {
      hired_commanders_ = ImportEmployedCommanders(input_stream);
   });
   character_manager_parser_.registerKeyword("dead_objects", [this](std::istream& input_stream) {
      commonItems::parser dead_parser;
      dead_parser.registerKeyword("dead_objects", [this](std::istream& input_stream) {
         obituaries_ = ImportObituaries(input_stream);
      });
      dead_parser.IgnoreUnregisteredItems();
      dead_parser.parseStream(input_stream);
   });
   character_manager_parser_.registerKeyword("exile_pool", [this](std::istream& input_stream) {
      exile_pool_ = ImportExilePool(input_stream);
   });
   character_manager_parser_.IgnoreUnregisteredItems();
   character_manager_parser_.parseStream(input_stream);

   AssignHomeTagToExiles();
   AssignIgToCharacters();
   HireCommanders();
}

void vic3::CharacterManager::AssignHomeTagToExiles()
{
   int count = 0;
   for (auto& character: characters_ | std::views::values)
   {
      if (const auto itr = exile_origin_map_.find(character.GetId()); itr != exile_origin_map_.end())
      {
         character.SetHomeTag(itr->second);
         ++count;
      }
   }

   Log(LogLevel::Info) << fmt::format("\tFound {} agitators working abroad.", count);
}

void vic3::CharacterManager::AssignIgToCharacters()
{
   for (auto& character: characters_ | std::views::values)
   {
      if (exile_pool_.contains(character.GetId()))
      {
         continue;
      }

      if (obituaries_.contains(character.GetId()))
      {
         continue;
      }

      if (const auto itr = character_ig_map_.find(character.GetId()); itr != character_ig_map_.end())
      {
         character.SetIgId(itr->second);
      }
      else
      {
         Log(LogLevel::Warning) << fmt::format("Character {} {} with ID: {} has no IG.",
             character.GetFirstName(),
             character.GetLastName(),
             character.GetId());
      }
   }
}

void vic3::CharacterManager::HireCommanders()
{
   for (auto& character: characters_ | std::views::values)
   {
      if (character.IsCommander())
      {
         break;
      }

      if (hired_commanders_.contains(character.GetId()))
      {
         character.SetCommander();
      }
   }
}