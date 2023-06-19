#ifndef SRC_VIC3WORLD_CHARACTERS_CHARACTERMANAGER_H
#define SRC_VIC3WORLD_CHARACTERS_CHARACTERMANAGER_H



#include <istream>

#include "src/vic3_world/characters/character.h"


namespace vic3
{

class CharacterManager
{
  public:
   explicit CharacterManager(std::istream& input_stream);

   [[nodiscard]] const std::map<int, Character>& GetCharacters() const { return characters_; }
   [[nodiscard]] const std::map<int, std::vector<int>>& GetCountryCharacterMap() const
   {
      return country_character_map_;
   }

  private:
   void AssignHomeTagToExiles();

   commonItems::parser character_manager_parser_;

   std::map<int, Character> characters_;
   std::map<int, std::vector<int>> country_character_map_;
   std::map<int, std::string> exile_origin_map_;
   std::map<int, int> character_ig_map_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_CHARACTERS_CHARACTERMANAGER_H