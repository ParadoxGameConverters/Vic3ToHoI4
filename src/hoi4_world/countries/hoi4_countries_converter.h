#ifndef SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H
#define SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H



#include <map>
#include <vector>

#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/countries/hoi4_country_converter.h"
#include "src/hoi4_world/states/hoi4_states.h"
#include "src/mappers/world/world_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

std::map<std::string, Country> ConvertCountries(const vic3::World source_world,
    const mappers::WorldMapper& world_mapper,
    const commonItems::LocalizationDatabase& source_localizations,
    const States& states,
    std::map<int, Character>& characters,
    std::map<std::string, mappers::CultureQueue>& culture_queues,
    bool debug = false);

}  // namespace hoi4



#endif  // SRC_HOI4WORLD_COUNTRIES_HOI4COUNTRIESCONVERTER_H
