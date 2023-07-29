#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H



#include <map>
#include <vector>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/vic3_world/countries/vic3_country.h"
#include <src/vic3_world/world/vic3_world.h>



namespace hoi4
{

std::map<std::string, Country> ConvertCountries(
    const vic3::World source_world,
    const commonItems::LocalizationDatabase& source_localizations,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<State>& states,
    const std::vector<mappers::TechMapping>& tech_mappings,
    std::map<int, Character>& characters,
    std::map<std::string, mappers::CultureQueue>& culture_queues,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H