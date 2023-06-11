#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H



#include <optional>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/military/equipment_variant.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/culture/culture_graphics_mapper.h"
#include "src/mappers/ideology/ideology_mapper.h"
#include "src/mappers/technology/tech_mapping.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

std::optional<Country> ConvertCountry(const vic3::Country& source_country,
    const std::set<std::string>& source_technologies,
    const std::map<std::string, vic3::CultureDefinition>& source_cultures,
    const commonItems::LocalizationDatabase& source_localizations,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<State>& states,
    const mappers::IdeologyMapper& ideology_mapper,
    const std::vector<mappers::TechMapping>& tech_mappings,
    const std::vector<EquipmentVariant>& all_legacy_ship_variants,
    const std::vector<EquipmentVariant>& all_ship_variants,
    const std::vector<EquipmentVariant>& all_plane_variants,
    const std::vector<EquipmentVariant>& all_tank_variants,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRYCONVERTER_H