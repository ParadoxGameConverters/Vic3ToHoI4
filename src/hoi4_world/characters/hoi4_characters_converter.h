#ifndef SRC_HOI4WORLD_CHARACTERS_CHARACTERSCONVERTER_H
#define SRC_HOI4WORLD_CHARACTERS_CHARACTERSCONVERTER_H



#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/mappers/character/culture_queue.h"
#include "src/mappers/character/leader_type_mapper.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/interest_groups/interest_group.h"


namespace hoi4
{

std::pair<std::vector<int>, std::set<int>> ConvertCharacters(std::map<int, Character>& characters,
    const std::map<int, vic3::Character>& source_characters,
    const std::string& tag,
    const std::string& country_ideology,
    const std::string& sub_ideology,
    const vic3::Country& source_country,
    const std::map<int, vic3::InterestGroup>& igs,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    std::map<std::string, mappers::CultureQueue>& culture_queues,
    const mappers::CountryMapper& country_mapper);

void AssignPortraits(std::map<int, Character>& characters,
    const std::map<std::string, mappers::CultureQueue>& culture_queues,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper,
    const std::map<std::string, vic3::CultureDefinition>& source_cultures,
    int playthrough_id);
}  // namespace hoi4



#endif  // SRC_HOI4WORLD_CHARACTERS_CHARACTERSCONVERTER_H