#ifndef SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H
#define SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H



#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/character/leader_type_mapper.h"
#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/interest_groups/interest_group.h"


namespace hoi4
{

std::optional<Character> ConvertCharacter(const vic3::Character& source_character,
    int prime_minister,
    const Country& country,
    const vic3::Country& source_country,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    const mappers::CountryMapper& country_mapper);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_CHARACTERS_CHARACTERCONVERTER_H