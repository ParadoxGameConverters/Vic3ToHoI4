#ifndef SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTERIMPORTER_H
#define SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTERIMPORTER_H



#include <external/commonItems/Parser.h>

#include <istream>

#include "src/vic3_world/characters/vic3_character.h"



namespace vic3
{

class CharacterImporter
{
  public:
   CharacterImporter();

   [[nodiscard]] Character ImportCharacter(int id, std::istream& input_stream);

  private:
   commonItems::parser character_parser_;

   std::string first_name_;
   std::string last_name_;

   int culture_id_ = 0;
   bool is_female_ = false;
   std::set<std::string> roles_;
   int rank_ = 0;
   std::string ideology_;
   std::set<std::string> traits_;
   std::optional<int> formation_id_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_VIC3CHARACTERIMPORTER_H