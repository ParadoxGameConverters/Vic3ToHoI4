#ifndef SRC_VIC3WORLD_CHARACTERS_CHARACTERIMPORTER_H
#define SRC_VIC3WORLD_CHARACTERS_CHARACTERIMPORTER_H



#include <istream>

#include "src/vic3_world/characters/character.h"


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

   int culture_id_;
   std::set<std::string> roles_;
   int rank_;
   std::string ideology_;
   std::set<std::string> traits_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_CHARACTERIMPORTER_H