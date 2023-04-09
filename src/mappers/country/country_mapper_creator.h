#ifndef SRC_MAPPERS_COUNTRY_COUNTRYMAPPERIMPORTER_H
#define SRC_MAPPERS_COUNTRY_COUNTRYMAPPERIMPORTER_H



#include <map>
#include <string_view>

#include "src/mappers/country/country_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"



namespace mappers
{

[[nodiscard]] CountryMapper CreateCountryMappings(std::string_view country_mappings_file,
    const std::map<int, vic3::Country>& countries);

}  // namespace mappers



#endif  // SRC_MAPPERS_COUNTRY_COUNTRYMAPPER_H