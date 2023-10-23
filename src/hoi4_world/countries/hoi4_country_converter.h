#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H



#include <optional>

#include "src/hoi4_world/characters/hoi4_character.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/military/convoy_distributor.h"
#include "src/hoi4_world/military/division_template.h"
#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/states/hoi4_states.h"
#include "src/mappers/character/character_trait_mapper.h"
#include "src/mappers/character/culture_queue.h"
#include "src/mappers/character/leader_type_mapper.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/mappers/ideology/ideology_mapper.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/mappers/unit/unit_mapper.h"
#include "src/vic3_world/characters/vic3_character.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/ideologies/ideologies.h"
#include "src/vic3_world/interest_groups/interest_group.h"



namespace hoi4
{

std::optional<Country> ConvertCountry(const vic3::World& source_world,
    const vic3::Country& source_country,
    const commonItems::LocalizationDatabase& source_localizations,
    const mappers::CountryMapper& country_mapper,
    const States& states,
    const mappers::IdeologyMapper& ideology_mapper,
    const mappers::UnitMapper& unit_mapper,
    const std::vector<mappers::TechMapping>& tech_mappings,
    const std::vector<EquipmentVariant>& all_legacy_ship_variants,
    const std::vector<EquipmentVariant>& all_ship_variants,
    const std::vector<EquipmentVariant>& all_plane_variants,
    const std::vector<EquipmentVariant>& all_tank_variants,
    const std::vector<DivisionTemplate>& division_templates,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper,
    const mappers::LeaderTypeMapper& leader_type_mapper,
    const mappers::CharacterTraitMapper& character_trait_mapper,
    const ConvoyDistributor& convoys,
    std::map<int, Character>& characters,
    std::map<std::string, mappers::CultureQueue>& culture_queues,
    bool debug = false);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H
