#include "src/vic3_world/characters/vic3_character_importer.h"

#include <external/commonItems/Log.h>
#include <external/commonItems/ParserHelpers.h>
#include <external/fmt/include/fmt/format.h>



vic3::CharacterImporter::CharacterImporter()
{
   character_parser_.registerKeyword("first_name", [this](std::istream& input_stream) {
      first_name_ = commonItems::getString(input_stream);
   });
   character_parser_.registerKeyword("last_name", [this](std::istream& input_stream) {
      last_name_ = commonItems::getString(input_stream);
   });
   character_parser_.registerKeyword("culture", [this](std::istream& input_stream) {
      culture_id_ = commonItems::getInt(input_stream);
   });
   character_parser_.registerKeyword("is_female", [this](std::istream& input_stream) {
      is_female_ = commonItems::getString(input_stream) == "yes";
   });
   character_parser_.registerKeyword("role", [this](std::istream& input_stream) {
      roles_.emplace(commonItems::getString(input_stream));
   });
   character_parser_.registerKeyword("rank", [this](std::istream& input_stream) {
      const std::string rank_string = commonItems::getString(input_stream);
      if (rank_string == "commander_rank_ruler")
      {
         rank_ = 1;
         return;
      }

      try
      {
         rank_ = std::stoi(std::string(1, rank_string.back()));
      }
      catch (const std::exception& e)
      {
         Log(LogLevel::Warning) << fmt::format("Failed to read rank: {}. {}", rank_string, e.what());
      }
   });
   character_parser_.registerKeyword("formation", [this](std::istream& input_stream) {
      formation_id_ = commonItems::getInt(input_stream);
   });
   character_parser_.registerKeyword("ideology", [this](std::istream& input_stream) {
      ideology_ = commonItems::getString(input_stream);
   });
   character_parser_.registerKeyword("traits", [this](std::istream& input_stream) {
      const auto traits = commonItems::getStrings(input_stream);
      std::ranges::copy(traits, std::inserter(traits_, traits_.end()));
   });
   character_parser_.IgnoreUnregisteredItems();
}


vic3::Character vic3::CharacterImporter::ImportCharacter(const int id, std::istream& input_stream)
{
   first_name_.clear();
   last_name_.clear();
   culture_id_ = 0;
   is_female_ = false;
   roles_.clear();
   rank_ = 0;
   ideology_.clear();
   traits_.clear();
   formation_id_.reset();

   character_parser_.parseStream(input_stream);

   return Character({.id = id,
       .first_name = first_name_,
       .last_name = last_name_,
       .culture_id = culture_id_,
       .is_female = is_female_,
       .roles = roles_,
       .rank = rank_,
       .ideology = ideology_,
       .traits = traits_,
       .formation_id = formation_id_});
}