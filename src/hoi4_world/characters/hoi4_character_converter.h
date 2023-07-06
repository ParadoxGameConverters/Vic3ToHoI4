#ifndef SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H
#define SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H



#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/character/culture_queue.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

bool HasMonarchs(const std::string& leader_type, const std::set<std::string>& laws);

Character ConvertCharacter(const vic3::Character& source_character,
    int leader_id,
    const std::string& leader_type,
    const std::string& tag,
    const std::string& country_ideology,
    const vic3::Country& source_country,
    std::map<std::string, mappers::CultureQueue>& culture_queues,
    const mappers::CountryMapper& country_mapper);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H