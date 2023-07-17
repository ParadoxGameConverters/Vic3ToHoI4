#ifndef SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H
#define SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H



#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/character/character_trait_mapper.h"
#include "src/mappers/character/culture_queue.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/characters/vic3_character.h"



namespace hoi4
{
struct RoleIds
{
   int leader_id = 0;
   std::set<int> admiral_ids;
   std::set<int> field_marshal_ids;
   std::set<int> general_ids;
   std::set<int> advisor_ids;
   std::set<int> spy_ids;
};


bool HasMonarchs(const std::string& leader_type, const std::set<std::string>& laws);

Character ConvertCharacter(const vic3::Character& source_character,
    int leader_id,
    const RoleIds& role_ids,
    const std::string& leader_type,
    const std::string& tag,
    const std::string& country_ideology,
    const std::string& sub_ideology,
    const std::set<std::string>& laws,
    const mappers::CountryMapper& country_mapper,
    const mappers::CharacterTraitMapper& character_trait_mapper,
    std::map<std::string, mappers::CultureQueue>& culture_queues);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H