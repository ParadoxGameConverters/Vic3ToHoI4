#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H



#include <map>
#include <vector>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/hoi4_world/states/hoi4_state.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace hoi4
{

std::map<std::string, Country> ConvertCountries(const std::map<int, vic3::Country>& source_countries,
    const std::map<int, std::set<std::string>>& source_technologies,
    const std::map<std::string, vic3::CultureDefinition>& source_cultures,
    const commonItems::LocalizationDatabase& source_localizations,
    const mappers::CountryMapper& country_mapper,
    const std::map<int, int>& vic3_state_ids_to_hoi4_state_ids,
    const std::vector<State>& states,
    const std::vector<mappers::TechMapping>& tech_mappings,
    const std::map<int, vic3::Character>& source_characters,
    const std::map<int, vic3::InterestGroup>& igs,
    const mappers::CultureGraphicsMapper& culture_graphics_mapper,
    const std::map<int, vic3::InterestGroup>& interest_groups,
    const vic3::Ideologies& vic3_ideologies,
    std::map<int, Character>& characters,
    std::map<std::string, mappers::CultureQueue>& culture_queues);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H